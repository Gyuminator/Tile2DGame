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

t2g::Scene::Scene()
	: mObjects{}
	, mUpdateComponentsLayers{}
	, mRenderComponentsLayers{}
	, mTiles{}
	, mCameras{}
	, mSize{ 1, 1 }
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
}

void t2g::Scene::Init(SIZE sceneSize)
{
	mSize = sceneSize;

	for (size_t i = 0; i < mSize.cx * mSize.cy; ++i)
	{
		AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_01, 0, 0, INT(mTiles.size() - 1));
	}

	SafePtr<Object> player = AddObject();
	player->AddComponent<Transform>()->Init
	(
		Vector3(1000.f, 1000.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(1.f, 1.f, 0.f)
	);
	player->AddComponent<PlayerController>()->Init();

	RECT wRc = GET_SINGLETON(Application).GetWindowRect();
	player->AddComponent<Camera>()->Init(Rect(wRc.left, wRc.top,
		wRc.right - wRc.left, wRc.bottom - wRc.top));

	//player->AddComponent<ImageRenderer>()->Init(eImageName::Player, 0, 2);
	player->AddComponent<AnimationRenderer>()->Init(eImageName::Player);
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

	SafePtr<Object> enemy = AddObject();
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
		)
	);

	SafePtr<Camera> enemyCamera = enemy->GetComponent(eComponentType::Camera);
	enemyCamera->SetDistance(0.5f);

	enemy->AddComponent<AnimationRenderer>()->Init(eImageName::Plant_00);
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

	SafePtr<Object> camera3 = AddObject();
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
		)
	);
	SafePtr<Camera> camera3Camera = camera3->GetComponent(eComponentType::Camera);
	camera3Camera->SetDistance(2.f);

	camera3->SyncComponents();
	camera3->BindComponentsToScene();
}

void t2g::Scene::Exit()
{

}

void t2g::Scene::Enter()
{
	GET_SINGLETON(Application).ChangeTileBitmapSize(mSize);
	LoadImagesOfScene();

	for (auto& iter : mTiles)
	{
		SafePtr<TileRenderer> tileRender = iter->GetComponent(eComponentType::TileRenderer);
		tileRender->Render();
	}
}

SafePtr<t2g::Object> t2g::Scene::AddObject()
{
	unique_ptr<Object> uptr = Object::CreateObject();
	SafePtr<Object> sptr(uptr.get());

	uptr->SetOwner(this);
	mObjects.emplace(sptr->GetID(), std::move(uptr));

	return sptr;
}

SafePtr<t2g::Object> t2g::Scene::AddTile()
{
	unique_ptr<Object> uptr = Object::CreateObject();
	SafePtr<Object> sptr(uptr.get());

	uptr->SetOwner(this);
	mTiles.emplace_back(std::move(uptr));

	return sptr;
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

void t2g::Scene::LoadImagesOfScene()
{
	GET_SINGLETON(ImageManager).Load(eImageName::Player, L"Character\\plant.png", 3, 4);
	GET_SINGLETON(ImageManager).Load(eImageName::Plant_00, L"Character\\plant_00.png", 3, 4);
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_01, L"Tile\\Outside_A2.png", 16, 12);
}

UINT t2g::Object::AccID = 0;