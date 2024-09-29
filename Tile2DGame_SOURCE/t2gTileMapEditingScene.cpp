#include "stdafx.h"
#include "t2gTileMapEditingScene.h"

#include "t2gTileRenderer.h"
#include "t2gCamera.h"
#include "t2gMacro.h"
#include "t2gApplication.h"
#include "t2gRect.h"
#include "t2gTransform.h"
#include "t2gImageManager.h"
#include "t2gFunc.h"
#include "t2gShapeRenderer.h"
#include "t2gTime.h"
#include "t2gSceneManager.h"
#include "t2gTextRenderer.h"

#include "..\\Tile2DGame_Engine\\resource.h"

using namespace t2g::rect;

t2g::TileMapEditingScene::TileMapEditingScene()
	: mCurFilePath{}
	, mMapDirPath(L"..\\Resource\\Map\\")
	, mCurLayer(0)
	, mToolTilesetIdx(0)
	, mTileToolDC(nullptr)
	, mSelectedTile{}
	, mWriteTileMarker{}
	, mMainViewCamera{}
	, mTileViewCamera{}
	, mEditMode(eEditMode::Tiling)
	, mCurBlocking(false)
{
}

void t2g::TileMapEditingScene::init()
{
	LoadSprites();

	CreateTileToolBuffer();
	CreateToolTilesets();
	ChangeTileset(0);

	TryLoadPrevEditInfo();

	DrawTextCurrentInfo();

	CameraSetting();

	CreateMarkerObj();
}

void t2g::TileMapEditingScene::update()
{
	ClickEventMainTileView(mMainViewCamera);
	ClickEventToolTileView(mTileViewCamera);
	CameraMoveController();
	CameraDistanceController();
	ChangeTilesetController();
	ChangeLayerController();
	ChangeBlockingController();
	ChangeModeController();
	SaveMapController();
}

void t2g::TileMapEditingScene::ChangeMapSize(INT x, INT y)
{
	SIZE prevSize = GetSIZE();
	SetSize({ x, y });
	GET_SINGLETON(Application).ChangeTileBitmapSize(GetSIZE());
	SyncTilesToSceneSize(prevSize);
	mMainViewCamera->ClearViewport({ 0, 0, 0 });
	
	switch (mEditMode)
	{
	case eEditMode::Blocking:
	{
		DrawBlocking();
	}
	break;
	case eEditMode::ShowIndex:
	{
		DrawIndex();
	}
	break;
	}
}

void t2g::TileMapEditingScene::SaveMapController()
{
	if (func::CheckKey(eKeys::Ctrl, eKeyState::Pressed) &&
		func::CheckKey(eKeys::S, eKeyState::Down))
	{
		if (mCurFilePath.empty())
			SaveMapOtherName();
		else
			SaveMap(mCurFilePath);
	}
}

void t2g::TileMapEditingScene::SaveMap(const wstring& filePath)
{
	std::ofstream out(filePath, std::ios::binary);

	if (out.is_open())
	{
		// 씬 크기(타일 단위) 저장
		SIZE sceneSIZE = GetSIZE();
		out.write((char*)(&sceneSIZE), sizeof(sceneSIZE));

		const auto& tiles = GetTiles();
		for (const auto& tile : tiles)
		{
			const SafePtr<TileRenderer> tileRender = tile->GetComponent<TileRenderer>(eComponentType::TileRenderer);

			// 블로킹 정보 저장
			bool isBlocking = tileRender->GetIsBlocking();
			out.write((char*)(&isBlocking), sizeof(isBlocking));

			// 레이어 개수 저장
			INT layerSize = tileRender->GetLayerSize();
			out.write((char*)(&layerSize), sizeof(layerSize));

			for (INT i = 0; i < layerSize; ++i)
			{
				// 각 레이어의 이미지 정보 저장
				Point srcPos = tileRender->GetSrcPos(i);
				out.write((char*)(&srcPos), sizeof(srcPos));
				eImageName eImage = tileRender->GetImageName(i);
				out.write((char*)(&eImage), sizeof(eImage));
			}
		}

		SavePrevEditInfo(filePath);

		SetCurFilePath(filePath);
	}
}

void t2g::TileMapEditingScene::SaveMapOtherName()
{
	wchar_t fileName[256] = L"";
	wchar_t fileTitle[256] = L"";

	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = func::GetHWnd();
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName) / sizeof(fileName[0]);
	ofn.lpstrFilter = L"ALL\0*.*\0TileMap(*.tlm)\0*.tlm\0";			// 필터 정의
	ofn.nFilterIndex = 2;											// 기본 필터를 TileMap 으로 설정.
	ofn.lpstrFileTitle = fileTitle;									// 파일명과 확장자만 저장하는 버퍼.
	ofn.nMaxFileTitle = sizeof(fileTitle) / sizeof(fileTitle[0]);
	ofn.lpstrInitialDir = mMapDirPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn))
	{
		SaveMap(fileName);
	}
}

bool t2g::TileMapEditingScene::LoadMap(const wstring& filePath)
{
	bool isSuccess = Scene::LoadMap(filePath);
	if (isSuccess)
		SetCurFilePath(filePath);

	return isSuccess;
}

void t2g::TileMapEditingScene::LoadMapOtherName()
{
	wchar_t fileName[256] = L"";
	wchar_t fileTitle[256] = L"";

	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = func::GetHWnd();
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName) / sizeof(fileName[0]);
	ofn.lpstrFilter = L"ALL\0*.*\0TileMap(*.tlm)\0*.tlm\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = fileTitle;
	ofn.nMaxFileTitle = sizeof(fileTitle) / sizeof(fileTitle[0]);
	ofn.lpstrInitialDir = mMapDirPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn))
	{
		LoadMap(fileName);
		GET_SINGLETON(Application).ChangeTileBitmapSize(GetSIZE());
		DrawTiles();
	}
}

void t2g::TileMapEditingScene::SavePrevEditInfo(const wstring& filePath)
{
	wstring path = mMapDirPath;
	//wstring path = L"C:\\Users\\user-pc\\source\\repos\\Tile2DGame_2\\Resource\Map\\";
	path += L"prevEditInfo.editinfo";

	std::ofstream outInfo(path, std::ios::binary);

	if (outInfo.is_open())
	{
		INT sizeOfPrevFilePath = (INT)filePath.size() * 2 + 2; // null 문자 + 2
		outInfo.write((char*)(&sizeOfPrevFilePath), sizeof(sizeOfPrevFilePath));
		outInfo.write((char*)(filePath.c_str()), sizeOfPrevFilePath);
	}
}

const wstring t2g::TileMapEditingScene::LoadPrevEditInfo()
{
	wstring path = mMapDirPath;
	path += L"prevEditInfo.editinfo";

	wstring fileName{};

	std::ifstream inInfo(path, std::ios::binary);

	if (inInfo.is_open())
	{
		INT sizeOfPrevFileName;
		inInfo.read((char*)(&sizeOfPrevFileName), sizeof(sizeOfPrevFileName));

		unique_ptr<wchar_t[]> prevFileName(new wchar_t[sizeOfPrevFileName / 2]);
		inInfo.read((char*)(prevFileName.get()), sizeOfPrevFileName);

		fileName = prevFileName.get();
	}

	return fileName;
}

void t2g::TileMapEditingScene::CameraMoveController()
{
	SafePtr<Transform> tr = mMainViewCamera->GetOwnerObj()->GetComponent<Transform>(eComponentType::Transform);

	Vector3 location = tr->GetLocation();
	Vector3 rotation = tr->GetRotation();

	float speed = 500.f;

	if (func::CheckKey(eKeys::W, eKeyState::Pressed))
	{
		location.y -= speed * GET_SINGLETON(Time).GetDT();
		rotation.z = 180.f;
	}
	if (func::CheckKey(eKeys::A, eKeyState::Pressed))
	{
		location.x -= speed * GET_SINGLETON(Time).GetDT();
		rotation.z = 0.f;
	}
	if (func::CheckKey(eKeys::S, eKeyState::Pressed))
	{
		location.y += speed * GET_SINGLETON(Time).GetDT();
		rotation.z = 270.f;
	}
	if (func::CheckKey(eKeys::D, eKeyState::Pressed))
	{
		location.x += speed * GET_SINGLETON(Time).GetDT();
		rotation.z = 90.f;
	}

	tr->SetLocation(location);
	tr->SetRotation(rotation);
}

void t2g::TileMapEditingScene::CameraDistanceController()
{
	const Point mousePos = func::GetMousePos();
	if (mMainViewCamera->GetViewportRect().Contains(mousePos))
	{
		if (func::CheckKey(eKeys::Wheel, eKeyState::Up))
		{
			FLOAT distance = mMainViewCamera->GetDistance();
			distance -= 0.1f;
			mMainViewCamera->SetDistance(distance);
		}
		else if (func::CheckKey(eKeys::Wheel, eKeyState::Down))
		{
			FLOAT distance = mMainViewCamera->GetDistance();
			distance += 0.1f;
			mMainViewCamera->SetDistance(distance);
		}
	}
}

void t2g::TileMapEditingScene::ChangeTilesetController()
{
	INT idx = -1;
	if (func::CheckKey(eKeys::_1, eKeyState::Down))
		idx = 0;
	else if (func::CheckKey(eKeys::_2, eKeyState::Down))
		idx = 1;
	else if (func::CheckKey(eKeys::_3, eKeyState::Down))
		idx = 2;
	else if (func::CheckKey(eKeys::_4, eKeyState::Down))
		idx = 3;

	if (idx < 0)
		return;

	if (mToolTilesetIdx != idx)
	{
		ChangeTileset(idx);
		mTileViewCamera->cbRenderTileOnce();
		mTileViewCamera->DrawOutsideTileBuffer
		(
			{
				NumOfTileToolX * func::GetTileSize(),
				((LONG)mToolTilesets[idx].size() / NumOfTileToolX) * func::GetTileSize()
			}
		);
	}
}

void t2g::TileMapEditingScene::ChangeLayerController()
{
	INT idx = -1;
	if (func::CheckKey(eKeys::n1, eKeyState::Down))
		idx = 0;
	else if (func::CheckKey(eKeys::n2, eKeyState::Down))
		idx = 1;
	else if (func::CheckKey(eKeys::n3, eKeyState::Down))
		idx = 2;

	if (idx < 0)
		return;

	mCurLayer = idx;
	DrawTextCurrentInfo();
}

void t2g::TileMapEditingScene::ChangeModeController()
{
	eEditMode mode = eEditMode::EnumEnd;
	if (func::CheckKey(eKeys::F1, eKeyState::Down))
	{
		mode = eEditMode::Tiling;
	}
	else if (func::CheckKey(eKeys::F2, eKeyState::Down))
	{
		mode = eEditMode::Blocking;
	}
	else if (func::CheckKey(eKeys::F3, eKeyState::Down))
	{
		mode = eEditMode::ShowIndex;
	}

	if (mode == eEditMode::EnumEnd || mode == mEditMode)
		return;

	mEditMode = mode;
	switch (mEditMode)
	{
	case eEditMode::Tiling:
	{
		DrawTiles();
	}
	break;
	case eEditMode::Blocking:
	{
		DrawTiles();
		DrawBlocking();
	}
	break;
	case eEditMode::ShowIndex:
	{
		DrawTiles();
		DrawIndex();
	}
	break;
	}
	DrawTextCurrentInfo();
}

void t2g::TileMapEditingScene::ChangeBlockingController()
{
	if (func::CheckKey(eKeys::n0, eKeyState::Down))
		mCurBlocking = !mCurBlocking;

	DrawTextCurrentInfo();
}

void t2g::TileMapEditingScene::CameraSetting()
{
	Rect wndRect = MakeRectByRECT(func::GetWndRECT());

	// 메인 카메라 생성
	SafePtr<Object> mainCameraObj = AddObject(eObjectTag::Camera);

	mainCameraObj->AddComponent<Transform>()->Init(
		Vector3::Zero(), Vector3::Zero(), Vector3::One());
	// 뷰포트는 윈도우 크기의 ( 20% ~ 100%, 0% ~ 100% )
	Rect mainViewRect = MakeRectByAnchors(wndRect, { TileViewAnchorX, 0.f }, { 1.f, 1.0f });
	mainCameraObj->AddComponent<Camera>()->Init(mainViewRect, func::GetTileDC());

	mMainViewCamera = mainCameraObj->GetComponent<Camera>(eComponentType::Camera);
	mMainViewCamera->SetAnchor({ 0.f, 0.f });
	mMainViewCamera->SetOutsideColor({ 32, 32, 32 });

	mainCameraObj->SyncComponents();
	mainCameraObj->BindComponentsToScene();

	// 툴 뷰 카메라 생성
	SafePtr<Object> toolCameraObj = AddObject(eObjectTag::TileToolCamera);

	toolCameraObj->AddComponent<Transform>()->Init(
		Vector3::Zero(), Vector3::Zero(), Vector3::One());
	// 뷰포트는 윈도우 크기의 ( 0% ~ 20%, 6% ~ 100% )
	Rect toolViewRect = MakeRectByAnchors(wndRect, { 0.f, 0.06f }, { TileViewAnchorX, 1.f });
	toolCameraObj->AddComponent<Camera>()->Init(toolViewRect, mTileToolDC);

	mTileViewCamera = toolCameraObj->GetComponent<Camera>(eComponentType::Camera);
	mTileViewCamera->SetAnchor({ 0.f, 0.f });
	mTileViewCamera->SetDistance(1.25f);
	mTileViewCamera->InsertExcludeTag(eObjectTag::UI);

	toolCameraObj->SyncComponents();
	toolCameraObj->BindComponentsToScene();
}

void t2g::TileMapEditingScene::CreateTileToolBuffer()
{
	mTileToolDC = CreateCompatibleDC(GET_SINGLETON(Application).GetBackDC());
	INT tileSize = GET_SINGLETON(Application).TileSize;
	HBITMAP bitmap = CreateCompatibleBitmap(GET_SINGLETON(Application).GetBackDC(),
		tileSize * NumOfTileToolX, tileSize * NumOfTileToolY);

	HBITMAP oldBitmap = (HBITMAP)SelectObject(mTileToolDC, bitmap);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mTileToolDC, GetStockObject(HOLLOW_BRUSH));
	HPEN oldPen = (HPEN)SelectObject(mTileToolDC, CreatePen(PS_SOLID, 3, RGB(255, 0, 255)));

	DeleteObject(oldBrush);
	DeleteObject(oldPen);
	DeleteObject(oldBitmap);
}

void t2g::TileMapEditingScene::LoadSprites()
{
	LoadImagesOfScene();
}

void t2g::TileMapEditingScene::CreateToolTileset(eImageName eImgName, UINT8 tilesetIdx)
{
	auto sprite = GET_SINGLETON(ImageManager).FindImage(eImgName);
	vector<unique_ptr<Object>>& targetToolTileset = mToolTilesets[tilesetIdx];
	INT numOfFrameX = sprite->GetImage().GetWidth() / sprite->GetFrameHeight();
	INT numOfFrameY = sprite->GetImage().GetHeight() / sprite->GetFrameHeight();
	INT stdX = 0;
	for (INT y = 0; y < NumOfTileToolY; ++y)
	{
		for (INT x = 0; x < NumOfTileToolX; ++x)
		{
			INT curSize = (INT)targetToolTileset.size();
			unique_ptr<Object> uptr = CreateToolTileObj();
			uptr->AddComponent<TileRenderer>()->Init
			(
				eImgName, stdX + x, y, curSize
			);
			targetToolTileset.push_back(std::move(uptr));

			if (x == numOfFrameX - 1) // 기본 X축 프레임은 8개 단위지만 규격이 다른 경우를 맞추기 위함.
				break;
		}
		if (y == numOfFrameY - 1) // 기본 Y축 프레임은 24개 단위지만 규격이 다른 경우를 맞추기 위함.
		{
			if (numOfFrameX > NumOfTileToolX)
			{
				y = -1;
				numOfFrameX -= NumOfTileToolX;
				stdX += NumOfTileToolX;
			}
			else
				break;
		}
	}
}

void t2g::TileMapEditingScene::CreateToolTilesets()
{
	CreateToolTileset(eImageName::Tile_Outside_A5_png, 0);
	CreateToolTileset(eImageName::Tile_Dungeon_A5_png, 1);
	CreateToolTileset(eImageName::Tile_Outside_B_png, 2);
	CreateToolTileset(eImageName::Tile_Dungeon_B_png, 3);
}

void t2g::TileMapEditingScene::DrawTileToolBuffer()
{
	for (auto& toolTileObj : GetCurTileset())
	{
		toolTileObj->GetComponent<TileRenderer>(eComponentType::TileRenderer)
			->DrawTileToHDC(mTileToolDC, { NumOfTileToolX , NumOfTileToolY });
	}
}

void t2g::TileMapEditingScene::DrawNearToolTiles(SafePtr<TileRenderer> tile)
{
	INT ti = tile->GetTileIndex() - NumOfTileToolX - 1;
	for (INT i = 0; i < 3; ++i)
	{
		for (INT j = 0; j < 3; ++j)
		{
			if (ti >= 0 && ti < GetCurTileset().size())
			{
				GetCurTileset()[ti]->GetComponent<TileRenderer>(eComponentType::TileRenderer)
					->DrawTileToHDC(mTileToolDC, { NumOfTileToolX , NumOfTileToolY });
			}
			++ti;
		}
		ti += NumOfTileToolX - 3;
	}
}

void t2g::TileMapEditingScene::ClickEventToolTileView(SafePtr<Camera> camera)
{
	if (func::CheckKey(eKeys::LBtn, eKeyState::Down))
	{
		const Point mousePos = func::GetMousePos();
		if (camera->GetViewportRect().Contains(mousePos))
		{
			Point pos = camera->GetPosToCameraWorld(mousePos);
			INT tileIndex = func::GetTileIndex(NumOfTileToolX, pos);
			if (tileIndex != -1 && tileIndex < GetCurTileset().size())
			{
				if (mSelectedTile.IsValid())
					DrawNearToolTiles(mSelectedTile);

				mSelectedTile = GetCurTileset()[tileIndex]->GetComponent<TileRenderer>(eComponentType::TileRenderer);
				Rect rect = func::GetTileRectByIndex(NumOfTileToolX, mSelectedTile->GetTileIndex());
				//DrawTileToolBuffer();
				Rectangle(mTileToolDC, rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom());
				camera->cbRenderTileOnce();
			}
		}
	}
}

void t2g::TileMapEditingScene::ClickEventMainTileView(SafePtr<Camera> camera)
{
	const Point mousePos = func::GetMousePos();
	if (camera->GetViewportRect().Contains(mousePos))
	{
		Point pos = camera->GetPosToCameraWorld(mousePos);
		INT tileIndex = func::GetTileIndexSafety(GetSIZE(), pos.X, pos.Y);
		if (tileIndex != -1 && tileIndex < GetTiles().size())
		{
			SafePtr<Transform> tr = mWriteTileMarker->GetComponent<Transform>(eComponentType::Transform);
			Rect rect = func::GetTileRectByIndex(GetSIZE().cx, tileIndex);
			tr->SetLocation({ float(rect.X), float(rect.Y), 0.f });
			if (func::CheckKey(eKeys::LBtn, eKeyState::Pressed))
			{
				auto tileRenderer =
					GetTiles()[tileIndex]->GetComponent<TileRenderer>(eComponentType::TileRenderer);
				if (mSelectedTile.IsValid())
				{
					switch (mEditMode)
					{
					case eEditMode::Tiling:
					{
						tileRenderer->SetLayerSize(mCurLayer + 1);
						tileRenderer->SetImageName(mSelectedTile->GetImageName(), mCurLayer);
						tileRenderer->SetSrcPos(mSelectedTile->GetSrcPos(), mCurLayer);
						tileRenderer->DrawTileToHDC(func::GetTileDC(), { GetSIZE().cx, GetSIZE().cy });
					}
					break;
					case eEditMode::Blocking:
					{
						tileRenderer->SetIsBlocking(mCurBlocking);
						tileRenderer->DrawTileToHDC(func::GetTileDC(), { GetSIZE().cx, GetSIZE().cy });
						tileRenderer->DrawBlocking(GetSIZE().cx, func::GetTileDC());
					}
					break;
					}
				}
			}
		}
	}
}

void t2g::TileMapEditingScene::DrawTextFileName()
{
	wstring str = L"Current File: ";
	wstring fileName = mCurFilePath.substr(mCurFilePath.rfind(L"Map\\") + 4);
	str += fileName;
	RECT rc(0, 20, 300, 40);
	Rect rect = MakeRectByRECT(rc);
	Graphics g(func::GetBackDC());
	SolidBrush b({ 0, 0, 0 });
	g.FillRectangle(&b, MakeRectByRECT(rc));
	DrawText(GET_SINGLETON(Application).GetBackDC(), str.c_str(), int(str.length()), &rc, DT_LEFT | DT_TOP);
}

void t2g::TileMapEditingScene::DrawTextCurrentInfo()
{
	RECT rc(0, 40, 300, 60);
	Rect rect = MakeRectByRECT(rc);
	Graphics g(func::GetBackDC());
	SolidBrush b({ 0, 0, 0 });
	g.FillRectangle(&b, MakeRectByRECT(rc));
	wstring str;
	switch (mEditMode)
	{
	case eEditMode::Tiling:
	{
		str = L"Current Layer: ";
		str += std::to_wstring(mCurLayer + 1);
	}
	break;
	case eEditMode::Blocking:
	{
		str = L"Current Blocking: ";
		str += mCurBlocking ? L"True" : L"False";

	}
	break;
	}
	DrawText(GET_SINGLETON(Application).GetBackDC(), str.c_str(), int(str.length()), &rc, DT_LEFT | DT_TOP);
}

void t2g::TileMapEditingScene::CreateMarkerObj()
{
	mWriteTileMarker = AddObject(eObjectTag::UI);
	mWriteTileMarker->AddComponent<Transform>()->Init(Vector3::Zero(), Vector3::Zero(), Vector3::One());
	mWriteTileMarker->AddComponent<ShapeRenderer>()->Init(eShapeName::Ractangle,
		{ func::GetTileSize(), func::GetTileSize() }, Color(255, 0, 255));
	mWriteTileMarker->GET_COMPONENT(ShapeRenderer)->SetAnchor({ 0.f,0.f });
	mWriteTileMarker->SyncComponents();
	mWriteTileMarker->BindComponentsToScene();
}

void t2g::TileMapEditingScene::CreateBorderObj()
{
	mBorderObj = AddObject(eObjectTag::Static);
	mBorderObj->AddComponent<Transform>()->Init(Vector3::Zero(), Vector3::Zero(), Vector3::One());
	mBorderObj->AddComponent<ShapeRenderer>()->Init(eShapeName::RactangleBy4Line,
		{ func::GetTileSize(), func::GetTileSize() }, Color(255, 255, 255));
	mBorderObj->GET_COMPONENT(ShapeRenderer)->SetAnchor({ 0.f,0.f });
	mBorderObj->SyncComponents();
	mBorderObj->BindComponentsToScene();
}

void t2g::TileMapEditingScene::SyncBorderObjSize()
{
	mBorderObj->GET_COMPONENT(Transform)->SetScale(
		{ (float)GetSIZE().cx, (float)GetSIZE().cy, 1.f });
}

void t2g::TileMapEditingScene::ChangeTileset(UINT8 idx)
{
	mToolTilesetIdx = idx;
	mSelectedTile = GetCurTileset()[0]->GetComponent<TileRenderer>(eComponentType::TileRenderer);
	DrawTileToolBuffer();
}

void t2g::TileMapEditingScene::DrawBlocking()
{
	for (auto& tileObj : GetTiles())
	{
		tileObj->GET_COMPONENT(TileRenderer)->DrawBlocking(GetSIZE().cx, func::GetTileDC());
	}
}

void t2g::TileMapEditingScene::DrawIndex()
{
	for (auto& tileObj : GetTiles())
	{
		tileObj->GET_COMPONENT(TileRenderer)->cbDrawIndex();
	}
}

void t2g::TileMapEditingScene::TryLoadPrevEditInfo()
{
	wstring prevFileName = LoadPrevEditInfo();
	if (prevFileName.empty())
	{
		SetSize(BasicSceneSIZE);
		for (size_t i = 0; i < GetSIZE().cx * GetSIZE().cy; ++i)
		{
			AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_Outside_A5_png,
				0, 0, INT(GetTiles().size() - 1));
		}
	}
	else
	{
		LoadMap(prevFileName);
	}
}


unique_ptr<t2g::Object> t2g::TileMapEditingScene::CreateToolTileObj()
{
	unique_ptr<Object> uptr = Object::CreateObject();

	uptr->SetOwnerScene(this);
	uptr->SetTag(eObjectTag::ToolTile);

	return std::move(uptr);
}

// ==================================
// TileMap ReSize Proc
// ==================================
INT_PTR CALLBACK t2g::TileMapReSizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			int sizeX = GetDlgItemInt(hDlg, IDC_TILEMAP_SIZE_X, nullptr, false);
			int sizeY = GetDlgItemInt(hDlg, IDC_TILEMAP_SIZE_Y, nullptr, false);

			t2g::Scene* ptr = GET_SINGLETON(SceneManager).GetCurScene().get();
			t2g::TileMapEditingScene* tileScene = dynamic_cast<t2g::TileMapEditingScene*>(ptr);
			if (tileScene)
			{
				tileScene->ChangeMapSize(sizeX, sizeY);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
