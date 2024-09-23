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

using namespace t2g::rect;

t2g::TileMapEditingScene::TileMapEditingScene()
	: mCurFilePath{}
	, mMapPath(L"..\\Resource\\Map\\")
	, mCurLayer(0)
	, mToolTilesetIdx(0)
	, mTileToolDC(nullptr)
	, mSelectedTile{}
	, mWriteTileMarker{}
	, mMainViewCamera{}
	, mTileViewCamera{}
	, mEditMode(eEditMode::Tiling)
{
}

void t2g::TileMapEditingScene::init()
{
	LoadSprites();

	CreateTileToolBuffer();
	CreateToolTileset(eImageName::Tile_Dungeon_A1_png, 0);
	CreateToolTileset(eImageName::Tile_Dungeon_A2_png, 1);
	CreateToolTileset(eImageName::Tile_Outside_A1_png, 2);
	CreateToolTileset(eImageName::Tile_Outside_A2_png, 3);

	ChangeTileset(0);

	DrawTextCurrentLayer();

	wstring prevFileName = LoadPrevEditInfo();
	if (prevFileName.empty())
	{
		SetSize({ 19, 12 });
		for (size_t i = 0; i < GetSize().cx * GetSize().cy; ++i)
		{
			AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_Dungeon_A2_png, 0, 0, INT(GetTiles().size() - 1));
		}
	}
	else
	{
		LoadMap(prevFileName);
	}

	//ChangeMapSize(42, 28);

	CameraSetting();

	CreateMarkerObj();
	//CreateBorderObj();
	//SyncBorderObjSize();
}

void t2g::TileMapEditingScene::ChangeMapSize(INT x, INT y)
{
	SIZE prevSize = GetSize();
	SetSize({ x, y });
	GET_SINGLETON(Application).ChangeTileBitmapSize(GetSize());
	SyncTilesToSceneSize(prevSize);
	mMainViewCamera->ClearViewport({ 0, 0, 0 });
	//SyncBorderObjSize();
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
		SIZE sceneSize = GetSize();
		out.write((char*)(&sceneSize), sizeof(sceneSize));
		auto& tiles = GetTiles();
		for (auto& tile : tiles)
		{
			SafePtr<TileRenderer> tileRender = tile->GetComponent<TileRenderer>(eComponentType::TileRenderer);

			INT layerSize = tileRender->GetLayerSize();
			out.write((char*)(&layerSize), sizeof(layerSize));
			for (INT i = 0; i < layerSize; ++i)
			{
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
	wchar_t fileName[256] = {};
	wchar_t fileTitle[256] = {};

	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = func::GetHWnd();
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	// 필터 정의
	ofn.lpstrFilter = L"ALL\0*.*\0TileMap\0*.tilemap\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = fileTitle;
	ofn.nMaxFileTitle = sizeof(fileTitle);
	ofn.lpstrInitialDir = mMapPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(func::GetAppPath());
		SaveMap(fileName);
	}
}

void t2g::TileMapEditingScene::LoadMap(const wstring& filePath)
{
	std::ifstream inMap(filePath, std::ios::binary);

	if (inMap.is_open())
	{
		ClearTile();

		SIZE sceneSize;
		inMap.read((char*)(&sceneSize), sizeof(sceneSize));
		SetSize(sceneSize);

		for (size_t i = 0; i < GetSize().cx * GetSize().cy; ++i)
		{
			INT layerSize;
			inMap.read((char*)(&layerSize), sizeof(layerSize));

			unique_ptr<Object> tileObj = CreateTileObj();
			SafePtr<TileRenderer> tileCom = tileObj->GET_COMPONENT(TileRenderer);
			tileCom->Init(eImageName::EnumEnd, 0, 0, 0);
			tileCom->SetTileIndex(GetTiles().size());
			PushTileObj(std::move(tileObj));

			for (INT i = 0; i < layerSize; ++i)
			{
				Point srcPos;
				inMap.read((char*)(&srcPos), sizeof(srcPos));
				tileCom->SetSrcPos(srcPos, i);
				eImageName eImage;
				inMap.read((char*)(&eImage), sizeof(eImage));
				tileCom->SetImageName(eImage, i);
			}
		}

		SetCurFilePath(filePath);
	}
}

void t2g::TileMapEditingScene::LoadMapOtherName()
{
	wchar_t fileName[256] = {};
	wchar_t fileTitle[256] = {};

	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = func::GetHWnd();
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	// 필터 정의
	ofn.lpstrFilter = L"ALL\0*.*\0TileMap\0*.tlm\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = fileTitle;
	ofn.nMaxFileTitle = sizeof(fileTitle);
	ofn.lpstrInitialDir = mMapPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(func::GetAppPath());
		LoadMap(fileName);
		GET_SINGLETON(Application).ChangeTileBitmapSize(GetSize());
		DrawTiles();
	}
}

void t2g::TileMapEditingScene::SavePrevEditInfo(const wstring& filePath)
{
	wstring path = mMapPath;
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
	wstring path = mMapPath;
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
	DrawTextCurrentLayer();
}

void t2g::TileMapEditingScene::ChangeModeController()
{
	if (func::CheckKey(eKeys::F1, eKeyState::Down))
	{
		mEditMode = eEditMode::Tiling;
	}
	else if (func::CheckKey(eKeys::F2, eKeyState::Down))
	{
		mEditMode = eEditMode::Blocking;
	}
}

void t2g::TileMapEditingScene::CameraSetting()
{
	Rect wndRect = MakeRectByRECT(GET_SINGLETON(Application).GetWindowRect());

	// 메인 카메라
	SafePtr<Object> mainCameraObj = AddObject(eObjectTag::Camera);
	mainCameraObj->AddComponent<Transform>()->Init(Vector3::Zero(), Vector3::Zero(), Vector3::One());
	Rect mainViewRect = MakeRectByAnchors(wndRect, { TileViewAnchorX, 0.f }, { 1.f, 1.0f });
	mainCameraObj->AddComponent<Camera>()->Init(mainViewRect, func::GetTileDC());
	mMainViewCamera = mainCameraObj->GetComponent<Camera>(eComponentType::Camera);
	mMainViewCamera->SetAnchor({ 0.f, 0.f });
	mMainViewCamera->SetDistance(1.f);
	mMainViewCamera->SetOutsideColor({ 32, 32, 32 });

	// 메인 카메라 이벤트 cbSyncCameraView
	mainCameraObj->BindBackEvent(eEventCallPoint::cbSyncCameraView,
		[this]()
		{
			ClickEventMainTileView(mMainViewCamera);
			CameraMoveController();
			CameraDistanceController();
			ChangeTilesetController();
			ChangeLayerController();

			SaveMapController();
			return eDelegateResult::OK;
		}
	);

	mainCameraObj->SyncComponents();
	mainCameraObj->BindComponentsToScene();

	// 타일 도구 카메라
	SafePtr<Object> toolCameraObj = AddObject(eObjectTag::TileToolCamera);
	toolCameraObj->AddComponent<Transform>()->Init(Vector3::Zero(), Vector3::Zero(), Vector3::One());
	Rect tileToolViewRect = MakeRectByAnchors(wndRect, { 0.f, 0.06f }, { TileViewAnchorX, 1.f });
	toolCameraObj->AddComponent<Camera>()->Init(tileToolViewRect, mTileToolDC);
	mTileViewCamera = toolCameraObj->GetComponent<Camera>(eComponentType::Camera);
	mTileViewCamera->SetAnchor({ 0.f, 0.f });
	mTileViewCamera->SetDistance(1.25f);
	mTileViewCamera->InsertExcludeTag(eObjectTag::UI);

	// 타일 도구 클릭 이벤트 추가
	toolCameraObj->BindBackEvent(eEventCallPoint::cbSyncCameraView,
		[this]()
		{
			ClickEventToolTileView(mTileViewCamera);
			return eDelegateResult::OK;
		}
	);

	toolCameraObj->SyncComponents();
	toolCameraObj->BindComponentsToScene();
}

void t2g::TileMapEditingScene::CreateTileToolBuffer()
{
	mTileToolDC = CreateCompatibleDC(GET_SINGLETON(Application).GetBackDC());
	INT tileSize = GET_SINGLETON(Application).TileSize;
	HBITMAP bitmap = CreateCompatibleBitmap(GET_SINGLETON(Application).GetBackDC(),
		tileSize * NumOfTileOfToolX, tileSize * NumOfTileOfToolY);

	HBITMAP oldBitmap = (HBITMAP)SelectObject(mTileToolDC, bitmap);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mTileToolDC, GetStockObject(HOLLOW_BRUSH));
	HPEN oldPen = (HPEN)SelectObject(mTileToolDC, CreatePen(PS_SOLID, 3, RGB(255, 0, 255)));

	DeleteObject(oldBrush);
	DeleteObject(oldPen);
	DeleteObject(oldBitmap);
}

void t2g::TileMapEditingScene::LoadSprites()
{
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_Outside_A1_png, L"Tile\\Outside_A1.png", 16, 12);
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_Outside_A2_png, L"Tile\\Outside_A2.png", 16, 12);
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_Dungeon_A1_png, L"Tile\\Dungeon_A1.png", 16, 12);
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_Dungeon_A2_png, L"Tile\\Dungeon_A2.png", 16, 12);
}

void t2g::TileMapEditingScene::CreateToolTileset(eImageName eImgName, UINT8 tilesetIdx)
{
	auto sprite = GET_SINGLETON(ImageManager).FindImage(eImgName);
	vector<unique_ptr<Object>>& targetToolTileset = mToolTilesets[tilesetIdx];
	INT numOfX = sprite->GetImage().GetWidth() / sprite->GetFrameHeight();
	INT numOfY = sprite->GetImage().GetHeight() / sprite->GetFrameHeight();
	INT rx = 0;
	for (INT y = 0; y < NumOfTileOfToolY; ++y)
	{
		for (INT x = 0; x < NumOfTileOfToolX; ++x)
		{
			unique_ptr<Object> uptr = CreateToolTileObj();
			uptr->AddComponent<TileRenderer>()->Init(eImgName, rx + x, y, INT(targetToolTileset.size()));
			targetToolTileset.push_back(std::move(uptr));

			if (x == numOfX - 1) // 기본 X축 프레임은 8개 단위지만 규격이 다른 경우를 맞추기 위함.
				break;
		}
		if (y == numOfY - 1) // 기본 Y축 프레임은 24개 단위지만 규격이 다른 경우를 맞추기 위함.
		{
			if (numOfX > NumOfTileOfToolX)
			{
				y = -1;
				numOfX -= NumOfTileOfToolX;
				rx += NumOfTileOfToolX;
			}
			else
				break;
		}
	}
}

void t2g::TileMapEditingScene::DrawTileToolBuffer()
{
	for (auto& toolTileObj : GetCurTileset())
	{
		toolTileObj->GetComponent<TileRenderer>(eComponentType::TileRenderer)
			->DrawTileToHDC(mTileToolDC, { NumOfTileOfToolX , NumOfTileOfToolY });
	}
}

void t2g::TileMapEditingScene::DrawNearToolTiles(SafePtr<TileRenderer> tile)
{
	INT ti = tile->GetTileIndex() - NumOfTileOfToolX - 1;
	for (INT i = 0; i < 3; ++i)
	{
		for (INT j = 0; j < 3; ++j)
		{
			if (ti >= 0 && ti < GetCurTileset().size())
			{
				GetCurTileset()[ti]->GetComponent<TileRenderer>(eComponentType::TileRenderer)
					->DrawTileToHDC(mTileToolDC, { NumOfTileOfToolX , NumOfTileOfToolY });
			}
			++ti;
		}
		ti += NumOfTileOfToolX - 3;
	}
}

void t2g::TileMapEditingScene::ClickEventToolTileView(SafePtr<Camera> camera)
{
	if (func::CheckKey(eKeys::LBtn, eKeyState::Down))
	{
		const Point mousePos = func::GetMousePos();
		if (camera->GetViewportRect().Contains(mousePos))
		{
			Point pos = camera->GetPosToCameraView(mousePos);
			INT tileIndex = func::GetTileIndex(NumOfTileOfToolX, pos);
			if (tileIndex != -1 && tileIndex < GetCurTileset().size())
			{
				if (mSelectedTile.IsValid())
					DrawNearToolTiles(mSelectedTile);

				mSelectedTile = GetCurTileset()[tileIndex]->GetComponent<TileRenderer>(eComponentType::TileRenderer);
				Rect rect = func::GetTileRectByIndex(NumOfTileOfToolX, mSelectedTile->GetTileIndex());
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
		Point pos = camera->GetPosToCameraView(mousePos);
		INT tileIndex = func::GetTileIndex(GetSize().cx, pos);
		if (tileIndex != -1 && tileIndex < GetTiles().size())
		{
			SafePtr<Transform> tr = mWriteTileMarker->GetComponent<Transform>(eComponentType::Transform);
			Rect rect = func::GetTileRectByIndex(GetSize().cx, tileIndex);
			tr->SetLocation({ float(rect.X), float(rect.Y), 0.f });
			if (func::CheckKey(eKeys::LBtn, eKeyState::Pressed))
			{
				auto tileRenderer =
					GetTiles()[tileIndex]->GetComponent<TileRenderer>(eComponentType::TileRenderer);
				if (mSelectedTile.IsValid())
				{
					tileRenderer->SetLayerSize(mCurLayer + 1);
					tileRenderer->SetImageName(mSelectedTile->GetImageName(), mCurLayer);
					tileRenderer->SetSrcPos(mSelectedTile->GetSrcPos(), mCurLayer);
					tileRenderer->DrawTileToHDC(func::GetTileDC(), { GetSize().cx, GetSize().cy });
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

void t2g::TileMapEditingScene::DrawTextCurrentLayer()
{
	wstring str = L"Current Layer: ";
	str += std::to_wstring(mCurLayer + 1);
	RECT rc(0, 40, 300, 60);
	Rect rect = MakeRectByRECT(rc);
	Graphics g(func::GetBackDC());
	SolidBrush b({ 0, 0, 0 });
	g.FillRectangle(&b, MakeRectByRECT(rc));
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
		{ (float)GetSize().cx, (float)GetSize().cy, 1.f });
}

void t2g::TileMapEditingScene::ChangeTileset(UINT8 idx)
{
	mToolTilesetIdx = idx;
	mSelectedTile = GetCurTileset()[0]->GetComponent<TileRenderer>(eComponentType::TileRenderer);
	DrawTileToolBuffer();
}


unique_ptr<t2g::Object> t2g::TileMapEditingScene::CreateToolTileObj()
{
	unique_ptr<Object> uptr = Object::CreateObject();

	uptr->SetOwnerScene(this);
	uptr->SetTag(eObjectTag::ToolTile);

	return std::move(uptr);
}
