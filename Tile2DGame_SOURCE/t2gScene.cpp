#include "t2gScene.h"
#include "t2gComponent.h"
#include "t2gObject.h"
#include "t2gTransform.h"
#include "t2gApplication.h"
#include "t2gImageManager.h"
#include "t2gImageRenderer.h"
#include "t2gTileRenderer.h"

t2g::Scene::Scene()
	: mSize{ 1, 1 }
	, mObjects{}
	, mUpdateComponentsLayers{}
	, mRenderComponentsLayers{}
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
	for (const auto& componentsLayer : mRenderComponentsLayers)
	{
		for (auto components : componentsLayer)
		{
			components->Render();
		}
	}
}

void t2g::Scene::Init(SIZE sceneSize)
{
	mSize = sceneSize;
	HBITMAP tileBitmap = CreateCompatibleBitmap
	(
		GET_SINGLETON(Application).GetTileDC(),
		sceneSize.cx * Application::TileSize, sceneSize.cy * Application::TileSize
	);
	GET_SINGLETON(Application).ChangeTileBitmap(tileBitmap);
	LoadImagesOfScene();

	AddObject(eObjectType::Player);
	//AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_01, 0, 0, INT(mTiles.size() - 1));
	//AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_01, 1, 0, INT(mTiles.size() - 1));
	//AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_01, 2, 0, INT(mTiles.size() - 1));
	//AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_01, 3, 0, INT(mTiles.size() - 1));
	for (size_t i = 0; i < mSize.cx * mSize.cy; ++i)
	{
		AddTile()->AddComponent<TileRenderer>()->Init(eImageName::Tile_01, 0, 0, INT(mTiles.size() - 1));
	}

	for (auto& iter : mTiles)
	{
		SafePtr<TileRenderer> tileRender = iter->GetComponent(eComponentType::TileRenderer);
		tileRender->Render();
	}
}

SafePtr<t2g::Object> t2g::Scene::AddObject(eObjectType type)
{
	unique_ptr<Object> uptr = Object::CreateObject();
	SafePtr<Object> sptr(uptr.get());

	uptr->SetOwner(this);
	uptr->Init(type);
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
	GET_SINGLETON(ImageManager).Load(eImageName::Tile_01, L"Tile\\Outside_A2.png", 16, 12);
}

UINT t2g::Object::AccID = 0;