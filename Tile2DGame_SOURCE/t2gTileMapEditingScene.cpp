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

using namespace t2g::rect;

t2g::TileMapEditingScene::TileMapEditingScene()
	: mCurFileName{}
	, mMapPath(L"..\\Resource\\Map\\")
	, mToolTiles{}
	, mTileToolDC(nullptr)
	, mSelectedTile{}
	, mWriteTileMarker{}
	, mMainViewCamera{}
	, mTileViewCamera{}
{
}

void t2g::TileMapEditingScene::init()
{
	LoadSprites();

	CreateTileToolBuffer();
	CreateToolTiles();
	DrawTileToolBuffer();

	/*for (size_t i = 0; i < GetSize().cx * GetSize().cy; ++i)
	{
		AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_Dungeon_A2_png, 0, 0, INT(GetTiles().size() - 1));
	}*/

	wstring prevFileName = LoadPrevEditInfo();
	if (prevFileName.empty())
	{
		for (size_t i = 0; i < GetSize().cx * GetSize().cy; ++i)
		{
			AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_Dungeon_A2_png, 0, 0, INT(GetTiles().size() - 1));
		}
	}
	else
	{
		LoadMap(prevFileName);
	}

	CameraSetting();

	mWriteTileMarker = AddObject(eObjectTag::UI);
	mWriteTileMarker->AddComponent<Transform>()->Init(Vector3::Zero(), Vector3::Zero(), Vector3::One());
	mWriteTileMarker->AddComponent<ShapeRenderer>()->Init(eShapeName::Ractangle,
		{ func::GetTileSize(), func::GetTileSize() }, Color(255, 0, 255));
	mWriteTileMarker->GetComponent<ShapeRenderer>(eComponentType::ShapeRenderer)->SetAnchor({ 0.f,0.f });
	mWriteTileMarker->SyncComponents();
	mWriteTileMarker->BindComponentsToScene();
}

void t2g::TileMapEditingScene::SaveMap(const wstring& fileName)
{
	if (func::CheckKey(eKeys::Ctrl, eKeyState::Pressed) &&
		func::CheckKey(eKeys::S, eKeyState::Down))
	{
		wstring path = mMapPath;
		path += fileName;

		std::ofstream out(path, std::ios::binary);

		if (out.is_open())
		{
			SIZE sceneSize = GetSize();
			out.write((char*)(&sceneSize), sizeof(sceneSize));
			auto& tiles = GetTiles();
			for (auto& tile : tiles)
			{
				SafePtr<TileRenderer> tileRender = tile->GetComponent<TileRenderer>(eComponentType::TileRenderer);
				Point srcPos = tileRender->GetSrcPos();
				eImageName eImage = tileRender->GetImageName();
				out.write((char*)(&srcPos), sizeof(srcPos));
				out.write((char*)(&eImage), sizeof(eImage));
			}

			SavePrevEditInfo(fileName);
		}
	}

}

void t2g::TileMapEditingScene::LoadMap(const wstring& fileName)
{
	wstring path = mMapPath;
	path += fileName;

	std::ifstream inMap(path, std::ios::binary);

	if (inMap.is_open())
	{
		SIZE sceneSize;
		inMap.read((char*)(&sceneSize), sizeof(sceneSize));
		SetSize(sceneSize);

		for (size_t i = 0; i < GetSize().cx * GetSize().cy; ++i)
		{
			Point srcPos;
			inMap.read((char*)(&srcPos), sizeof(srcPos));
			eImageName eImage;
			inMap.read((char*)(&eImage), sizeof(eImage));

			AddTile()->AddComponent<TileRenderer>()->Init(eImage, srcPos.X, srcPos.Y,
				INT(GetTiles().size() - 1));
		}

		mCurFileName = fileName;
	}
}

void t2g::TileMapEditingScene::SavePrevEditInfo(const wstring& fileName)
{
	wstring path = mMapPath;
	path += L"prevEditInfo.editinfo";

	std::ofstream outInfo(path, std::ios::binary);

	if (outInfo.is_open())
	{
		INT sizeOfPrevFileName = fileName.size() * 2 + 2; // null 문자 + 2
		outInfo.write((char*)(&sizeOfPrevFileName), sizeof(sizeOfPrevFileName));
		outInfo.write((char*)(fileName.c_str()), sizeOfPrevFileName);
	}
}

const wstring t2g::TileMapEditingScene::LoadPrevEditInfo()
{
	wstring path = mMapPath;
	path += L"prevEditInfo.editinfo";

	wstring fileName;

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

void t2g::TileMapEditingScene::cbSceneController()
{
	mMainViewCamera;
}

void t2g::TileMapEditingScene::CameraSetting()
{
	Rect wndRect = MakeRectByRECT(GET_SINGLETON(Application).GetWindowRect());

	// 메인 카메라
	SafePtr<Object> mainCameraObj = AddObject(eObjectTag::Camera);
	mainCameraObj->AddComponent<Transform>()->Init(Vector3::Zero(), Vector3::Zero(), Vector3::One());
	Rect mainViewRect = MakeRectByAnchors(wndRect, { TileViewAnchorX, 0.f }, { 1.f, 1.0f });
	mainCameraObj->AddComponent<Camera>()->Init(mainViewRect);
	auto mainCameraCom = mainCameraObj->GetComponent<Camera>(eComponentType::Camera);
	mainCameraCom->SetAnchor({ 0.f, 0.f });

	// 메인 타일 클릭 이벤트 추가
	mainCameraObj->BindBackEvent(eEventCallPoint::cbSyncCameraView,
		[mainCameraCom, this]()
		{
			ClickEventMainTileView(mainCameraCom);
			SaveMap(L"map_00.map");
			return eDelegateResult::OK;
		}
	);

	mainCameraObj->SyncComponents();
	mainCameraObj->BindComponentsToScene();

	// 타일 도구 카메라
	SafePtr<Object> toolCameraObj = AddObject(eObjectTag::TileToolCamera);
	toolCameraObj->AddComponent<Transform>()->Init(Vector3::Zero(), Vector3::Zero(), Vector3::One());
	Rect tileToolViewRect = MakeRectByAnchors(wndRect, { 0.f, 0.05f }, { TileViewAnchorX, 1.f });
	toolCameraObj->AddComponent<Camera>()->Init(tileToolViewRect);
	auto toolCameraCom = toolCameraObj->GetComponent<Camera>(eComponentType::Camera);
	toolCameraCom->SetAnchor({ 0.f, 0.f });
	toolCameraCom->SetDistance(1.25f);
	toolCameraCom->SetTargetTileDC(mTileToolDC);
	toolCameraCom->InsertExcludeTag(eObjectTag::UI);

	// 타일 도구 클릭 이벤트 추가
	toolCameraObj->BindBackEvent(eEventCallPoint::cbSyncCameraView,
		[toolCameraCom, this]()
		{
			ClickEventToolTileView(toolCameraCom);
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

void t2g::TileMapEditingScene::CreateToolTiles()
{
	auto sprite = GET_SINGLETON(ImageManager).FindImage(eImageName::Tile_Dungeon_A2_png);
	INT numOfX = sprite->GetImage().GetWidth() / sprite->GetFrameHeight();
	INT numOfY = sprite->GetImage().GetHeight() / sprite->GetFrameHeight();
	INT rx = 0;
	for (INT y = 0; y < NumOfTileOfToolY; ++y)
	{
		for (INT x = 0; x < NumOfTileOfToolX; ++x)
		{
			AddToolTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_Dungeon_A2_png, rx + x, y, INT(GetToolTiles().size() - 1));

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
	mSelectedTile = mToolTiles[0]->GetComponent<TileRenderer>(eComponentType::TileRenderer);
}

void t2g::TileMapEditingScene::DrawTileToolBuffer()
{
	for (auto& toolTileObj : mToolTiles)
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
			if (ti >= 0 && ti < mToolTiles.size())
			{
				mToolTiles[ti]->GetComponent<TileRenderer>(eComponentType::TileRenderer)
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
			if (tileIndex != -1 && tileIndex < mToolTiles.size())
			{
				if (mSelectedTile.IsValid())
					DrawNearToolTiles(mSelectedTile);

				mSelectedTile = mToolTiles[tileIndex]->GetComponent<TileRenderer>(eComponentType::TileRenderer);
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
					tileRenderer->SetSrcPos(mSelectedTile->GetSrcPos());
					tileRenderer->DrawTileToHDC(func::GetTileDC(), { GetSize().cx, GetSize().cy });
				}
			}
		}
	}
}


SafePtr<t2g::Object> t2g::TileMapEditingScene::AddToolTile()
{
	unique_ptr<Object> uptr = Object::CreateObject();
	SafePtr<Object> sptr(uptr.get());

	uptr->SetOwnerScene(this);
	uptr->SetTag(eObjectTag::ToolTile);
	mToolTiles.emplace_back(std::move(uptr));

	return sptr;
}
