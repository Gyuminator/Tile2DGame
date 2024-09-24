#include "stdafx.h"
#include "t2gScene.h"

#include "t2gComponent.h"
#include "t2gObject.h"
#include "t2gTransform.h"
#include "t2gApplication.h"
#include "t2gImageManager.h"
#include "t2gImageRenderer.h"
#include "t2gTileRenderer.h"
#include "t2gPlayerController.h"
#include "t2gAnimationRenderer.h"
#include "t2gCamera.h"
#include "t2gFunc.h"
#include "t2gMacro.h"
#include "t2gSceneManager.h"
#include "t2gCollider.h"

t2g::Scene::Scene()
	: mObjects{}
	, mUpdateComponentsLayers{}
	, mRenderComponentsLayers{}
	, mTiles{}
	, mCameras{}
	, mSize{}
	, mIsInited(false)
	, mTileMapFileName{}
{
}

void t2g::Scene::Update()
{
	for (const auto& componentsLayer : mUpdateComponentsLayers)
	{
		for (auto components : componentsLayer)
		{
			components->Update();
		}
	}
	update();
}

void t2g::Scene::Render()
{
	for (auto camera : mCameras)
	{
		mCurCamera = camera;
		camera->Update();
		for (const auto& componentsLayer : mRenderComponentsLayers)
		{
			for (auto components : componentsLayer)
			{
				components->Render();
			}
		}
		camera->Render();
	}
	render();
}

void t2g::Scene::Init(SIZE sceneSize)
{
	mSize = sceneSize;
	init();
}

void t2g::Scene::Init(const wstring tileMapFileName)
{
	mTileMapFileName = tileMapFileName;
	init();
}

void t2g::Scene::Exit()
{

}

bool t2g::Scene::LoadMap(const wstring& filePath)
{
	std::ifstream inMap(filePath, std::ios::binary);
	return LoadMap(inMap);
}

bool t2g::Scene::LoadMap(std::ifstream& inMap)
{
	bool isSuccess = inMap.is_open();

	if (isSuccess)
	{
		ClearTile();

		SIZE sceneSize;
		inMap.read((char*)(&sceneSize), sizeof(sceneSize));
		SetSize(sceneSize);

		for (size_t i = 0; i < GetSize().cx * GetSize().cy; ++i)
		{
			unique_ptr<Object> tileObj = CreateTileObj();
			SafePtr<TileRenderer> tileCom = tileObj->GET_COMPONENT(TileRenderer);
			tileCom->Init(eImageName::EnumEnd, 0, 0, 0);
			tileCom->SetTileIndex(GetTiles().size());
			PushTileObj(std::move(tileObj));

			bool isBlocking;
			inMap.read((char*)(&isBlocking), sizeof(isBlocking));
			tileCom->SetIsBlocking(isBlocking);

			INT layerSize;
			inMap.read((char*)(&layerSize), sizeof(layerSize));

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
	}
	return isSuccess;
}

void t2g::Scene::Enter()
{
	GET_SINGLETON(Application).ChangeTileBitmapSize(mSize);
	LoadImagesOfScene();

	DrawTiles();
}

SafePtr<t2g::Object> t2g::Scene::AddObject(eObjectTag tag)
{
	unique_ptr<Object> uptr = Object::CreateObject();
	SafePtr<Object> sptr(uptr.get());

	uptr->SetOwnerScene(this);
	uptr->SetTag(tag);
	mObjects.emplace(sptr->GetID(), std::move(uptr));

	return sptr;
}

SafePtr<t2g::Object> t2g::Scene::AddTile()
{
	unique_ptr<Object> uptr = Object::CreateObject();
	SafePtr<Object> sptr(uptr.get());

	uptr->SetOwnerScene(this);
	uptr->SetTag(eObjectTag::Tile);
	mTiles.emplace_back(std::move(uptr));

	return sptr;
}

unique_ptr<t2g::Object> t2g::Scene::CreateTileObj()
{
	unique_ptr<Object> uptr = Object::CreateObject();

	uptr->SetOwnerScene(this);
	uptr->SetTag(eObjectTag::Tile);
	uptr->AddComponent<TileRenderer>();

	return uptr;
}

void t2g::Scene::PushTileObj(unique_ptr<t2g::Object> tileObj)
{
	mTiles.emplace_back(std::move(tileObj));
}

void t2g::Scene::PushCamera(SafePtr<t2g::Camera> camera)
{
	for (auto item : mCameras)
	{
		// 카메라 중복 방지
		if (item.GetKey() == camera.GetKey())
			return;
	}

	mCameras.push_back(camera);
}

void t2g::Scene::BindComponent(SafePtr<Component> component)
{
	const eUpdateLayer ulayer = component->GetUpdateLayer();
	if (ulayer != eUpdateLayer::EnumEnd)
		mUpdateComponentsLayers[(UINT)ulayer].insert(component);

	const eRenderLayer rlayer = component->GetRenderLayer();
	if (rlayer != eRenderLayer::EnumEnd)
		mRenderComponentsLayers[(UINT)rlayer].insert(component);
}

void t2g::Scene::EventProc(eEventCallPoint callPoint)
{
	for (auto iter = mEvents[callPoint].begin(); iter != mEvents[callPoint].end();)
	{
		switch ((*iter)())
		{
		case eDelegateResult::Erase:
			iter = mEvents[callPoint].erase(iter);
			continue;
		case eDelegateResult::Return:
			return;
		}
		++iter;
	}
}

void t2g::Scene::SyncTilesToSceneSize(SIZE prevSize)
{
	struct TileDatas
	{
		Point srcPos = { 0, 0 };
		eImageName imageName = eImageName::EnumEnd;
	};
	int dx = mSize.cx - prevSize.cx;
	int dy = mSize.cy - prevSize.cy;
	UINT width = (dx > 0) ? prevSize.cx : mSize.cx;
	UINT height = (dy > 0) ? prevSize.cy : mSize.cy;
	int ti = 0;
	unordered_map<UINT, TileDatas> tileDatas;
	for (UINT y = 0; y < height; ++y)
	{
		for (UINT x = 0; x < width; ++x)
		{
			auto& tileObj = mTiles[ti + x];
			auto tileCom = tileObj->GET_COMPONENT(TileRenderer);
			TileDatas datas;
			datas.srcPos = tileCom->GetSrcPos();
			datas.imageName = tileCom->GetImageName();
			UINT index = tileCom->GetTileIndex();
			index += (index / prevSize.cx) * dx;
			tileDatas.insert(make_pair(index, datas));
		}
		ti += prevSize.cx;
	}
	/*for (auto& tileObj : mTiles)
	{
		auto tileCom = tileObj->GET_COMPONENT(TileRenderer);
		TileDatas datas;
		datas.srcPos = tileCom->GetSrcPos();
		datas.imageName = tileCom->GetImageName();
		UINT index = tileCom->GetTileIndex();
		int dx = mSize.cx - prevSize.cx;
		index += (index / prevSize.cx) * dx;
		if (tileDatas.find(index) == tileDatas.end())
		{
			tileDatas.insert(make_pair(index, datas));
		}
	}*/
	mTiles.resize(mSize.cx * mSize.cy);
	for (UINT i = 0; i < mTiles.size(); ++i)
	{
		if (mTiles[i] == nullptr)
		{
			mTiles[i] = CreateTileObj();
		}
		const auto iter = tileDatas.find(i);
		if (iter == tileDatas.end())
		{
			mTiles[i]->GET_COMPONENT(TileRenderer)->Init(
				eImageName::EnumEnd, 0, 0, i);
		}
		else
		{
			const TileDatas& datas = iter->second;
			mTiles[i]->GET_COMPONENT(TileRenderer)->Init(
				datas.imageName, datas.srcPos.X, datas.srcPos.Y, i);
		}
	}

	/*if (prevSize.cx < mSize.cx)
	{
		INT ix = prevSize.cx;
		INT dx = mSize.cx - prevSize.cx;

		if (prevSize.cy < mSize.cy)
		{
			INT dy = mSize.cy - prevSize.cy;

			for (INT i = 0; i < prevSize.cy; ++i)
			{
				for (INT j = 0; j < dx; ++j)
				{
					InsertTile(ix + j)->AddComponent<TileRenderer>()->
						Init(eImageName::EnumEnd, 0, 0, 0);
				}
				ix += mSize.cx;
			}
			for (INT i = 0; i < mSize.cx * dy; ++i)
			{
				AddTile()->AddComponent<TileRenderer>()->
					Init(eImageName::EnumEnd, 0, 0, 0);
			}
		}
		else
		{
			for (INT i = 0; i < mSize.cy; ++i)
			{
				for (INT j = 0; j < dx; ++j)
				{
					InsertTile(ix + j)->AddComponent<TileRenderer>()->
						Init(eImageName::EnumEnd, 0, 0, 0);
				}
				ix += mSize.cx;
			}
		}
	}
	else if (prevSize.cx > mSize.cx)
	{
		INT ix = mSize.cx;
		INT dx = prevSize.cx - mSize.cx;

		if (prevSize.cy < mSize.cy)
		{
			INT dy = mSize.cy - prevSize.cy;

			for (INT i = 0; i < mSize.cy; ++i)
			{
				mTiles.erase(mTiles.begin() + ix, mTiles.begin() + ix + dx);
				ix += mSize.cx;
			}
			for (INT i = 0; i < mSize.cx * dy; ++i)
			{
				AddTile()->AddComponent<TileRenderer>()->
					Init(eImageName::EnumEnd, 0, 0, 0);
			}
		}
		else if (prevSize.cy < mSize.cy)
		{
			for (INT i = 0; i < mSize.cy; ++i)
			{
				mTiles.erase(mTiles.begin() + ix, mTiles.begin() + ix + dx);
				ix += mSize.cx;
			}
		}
	}
	mTiles.resize(mSize.cx * mSize.cy);
	for (UINT i = 0; i < mTiles.size(); ++i)
	{
		mTiles[i]->GetComponent<TileRenderer>(eComponentType::TileRenderer)->SetTileIndex(i);
	}*/

	DrawTiles();
}

void t2g::Scene::DrawTiles()
{
	for (auto& iter : mTiles)
	{
		SafePtr<TileRenderer> tileRender = iter->GetComponent(eComponentType::TileRenderer);
		tileRender->Render();
	}
}

void t2g::Scene::init()
{
	wstring filePath = GET_SINGLETON(SceneManager).GetMapDirPath() + mTileMapFileName;
	std::ifstream inMap(filePath, std::ios::binary);
	LoadMap(inMap);

	/*for (size_t i = 0; i < mSize.cx * mSize.cy; ++i)
	{
		AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_Outside_A2_png, 0, 0, INT(mTiles.size() - 1));
	}*/

	SafePtr<Object> player = AddObject(eObjectTag::Player);
	player->AddComponent<Collider>()->Init({ 48, 48 }, { 0.5f, 0.5f }, { 0, 0 });
	player->AddComponent<Transform>()->Init
	(
		Vector3(1000.f, 1000.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(1.f, 1.f, 0.f)
	);
	player->AddComponent<PlayerController>()->Init();

	RECT wRc = GET_SINGLETON(Application).GetWindowRect();
	player->AddComponent<Camera>()->Init(Rect(wRc.left, wRc.top,
		wRc.right - wRc.left, wRc.bottom - wRc.top), func::GetTileDC());

	//player->AddComponent<ImageRenderer>()->Init(eImageName::Player, 0, 2);
	player->AddComponent<AnimationRenderer>()->Init(eImageName::Player);
	player->GetComponent<AnimationRenderer>(eComponentType::AnimationRenderer)
		->SetAnchor(PointF(0.5f, 0.5f));
	SafePtr<AnimationRenderer> playerAnimRender = player->GetComponent(eComponentType::AnimationRenderer);
	playerAnimRender->AddFrame(eAnimState::Idle_Left, { 1, 1 });
	playerAnimRender->AddFrame(eAnimState::Idle_Left, { 0, 1 });
	playerAnimRender->AddFrame(eAnimState::Idle_Left, { 1, 1 });
	playerAnimRender->AddFrame(eAnimState::Idle_Left, { 2, 1 });
	playerAnimRender->AddFrame(eAnimState::Idle_Right, { 1, 2 });
	playerAnimRender->AddFrame(eAnimState::Idle_Right, { 0, 2 });
	playerAnimRender->AddFrame(eAnimState::Idle_Right, { 1, 2 });
	playerAnimRender->AddFrame(eAnimState::Idle_Right, { 2, 2 });
	playerAnimRender->AddFrame(eAnimState::Idle_Up, { 1, 3 });
	playerAnimRender->AddFrame(eAnimState::Idle_Up, { 0, 3 });
	playerAnimRender->AddFrame(eAnimState::Idle_Up, { 1, 3 });
	playerAnimRender->AddFrame(eAnimState::Idle_Up, { 2, 3 });
	playerAnimRender->AddFrame(eAnimState::Idle_Down, { 1, 0 });
	playerAnimRender->AddFrame(eAnimState::Idle_Down, { 0, 0 });
	playerAnimRender->AddFrame(eAnimState::Idle_Down, { 1, 0 });
	playerAnimRender->AddFrame(eAnimState::Idle_Down, { 2, 0 });
	playerAnimRender->BindStateChanger(eAnimState::Idle_Left, &AnimationRenderer::scChangeDirectionByRotation);
	playerAnimRender->BindStateChanger(eAnimState::Idle_Right, &AnimationRenderer::scChangeDirectionByRotation);
	playerAnimRender->BindStateChanger(eAnimState::Idle_Up, &AnimationRenderer::scChangeDirectionByRotation);
	playerAnimRender->BindStateChanger(eAnimState::Idle_Down, &AnimationRenderer::scChangeDirectionByRotation);
	playerAnimRender->SetCurState(eAnimState::Idle_Right);
	player->SyncComponents();
	player->BindComponentsToScene();

	SafePtr<Object> enemy = AddObject(eObjectTag::Enemy);
	enemy->AddComponent<Transform>()->Init
	(
		Vector3(1000.f, 1000.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(1.f, 1.f, 0.f)
	);

	INT wWidth = wRc.right - wRc.left;
	INT wHeight = wRc.bottom - wRc.top;
	enemy->AddComponent<Camera>()->Init
	(
		Rect
		(
			INT(FLOAT(wWidth) * 0.6f), INT(FLOAT(wHeight) * 0.6f),
			INT(FLOAT(wWidth) * 0.4f), INT(FLOAT(wHeight) * 0.4f)
		),
		func::GetTileDC()
	);

	SafePtr<Camera> enemyCamera = enemy->GetComponent(eComponentType::Camera);
	enemyCamera->SetDistance(0.5f);

	enemy->AddComponent<AnimationRenderer>()->Init(eImageName::Plant_00);
	enemy->GetComponent<AnimationRenderer>(eComponentType::AnimationRenderer)
		->SetAnchor(PointF(0.5f, 0.5f));
	SafePtr<AnimationRenderer> enemyAnimRender = enemy->GetComponent(eComponentType::AnimationRenderer);
	enemyAnimRender->AddFrame(eAnimState::Idle_Left, { 1, 1 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Left, { 0, 1 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Left, { 1, 1 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Left, { 2, 1 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Right, { 1, 2 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Right, { 0, 2 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Right, { 1, 2 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Right, { 2, 2 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Up, { 1, 3 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Up, { 0, 3 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Up, { 1, 3 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Up, { 2, 3 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Down, { 1, 0 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Down, { 0, 0 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Down, { 1, 0 });
	enemyAnimRender->AddFrame(eAnimState::Idle_Down, { 2, 0 });
	enemyAnimRender->BindStateChanger(eAnimState::Idle_Left, &AnimationRenderer::scChangeDirectionByRotation);
	enemyAnimRender->BindStateChanger(eAnimState::Idle_Right, &AnimationRenderer::scChangeDirectionByRotation);
	enemyAnimRender->BindStateChanger(eAnimState::Idle_Up, &AnimationRenderer::scChangeDirectionByRotation);
	enemyAnimRender->BindStateChanger(eAnimState::Idle_Down, &AnimationRenderer::scChangeDirectionByRotation);
	enemyAnimRender->SetCurState(eAnimState::Idle_Right);
	enemy->SyncComponents();
	enemy->BindComponentsToScene();

	SafePtr<Object> camera3 = AddObject(eObjectTag::Camera);
	camera3->AddComponent<Transform>()->Init
	(
		Vector3(900.f, 900.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(1.f, 1.f, 0.f)
	);

	camera3->AddComponent<Camera>()->Init
	(
		Rect
		(
			INT(FLOAT(wWidth) * 0.0f), INT(FLOAT(wHeight) * 0.0f),
			INT(FLOAT(wWidth) * 0.3f), INT(FLOAT(wHeight) * 0.3f)
		),
		func::GetTileDC()
	);
	SafePtr<Camera> camera3Camera = camera3->GetComponent(eComponentType::Camera);
	camera3Camera->SetDistance(2.f);

	camera3->SyncComponents();
	camera3->BindComponentsToScene();

}

void t2g::Scene::LoadImagesOfScene()
{
	GET_SINGLETON(ImageManager).Load(eImageName::Player, L"Character\\plant.png", 3, 4);
	GET_SINGLETON(ImageManager).Load(eImageName::Plant_00, L"Character\\plant_00.png", 3, 4);
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_Outside_A1_png, L"Tile\\Outside_A1.png", 16, 12);
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_Outside_A2_png, L"Tile\\Outside_A2.png", 16, 12);
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_Dungeon_A1_png, L"Tile\\Dungeon_A1.png", 16, 12);
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_Dungeon_A2_png, L"Tile\\Dungeon_A2.png", 16, 12);
}

SafePtr<t2g::Object> t2g::Scene::InsertTile(INT index)
{
	unique_ptr<Object> uptr = Object::CreateObject();
	SafePtr<Object> sptr(uptr.get());

	uptr->SetOwnerScene(this);
	uptr->SetTag(eObjectTag::Tile);
	mTiles.emplace(mTiles.begin() + index, std::move(uptr));

	return sptr;
}

UINT t2g::Object::AccID = 0;