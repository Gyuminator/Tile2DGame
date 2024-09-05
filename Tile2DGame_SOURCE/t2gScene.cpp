#include "t2gScene.h"
#include "t2gComponent.h"
#include "t2gObject.h"

t2g::Scene::Scene()
	: mObjects{}
	, mUpdateComponentsLayers{}
	, mRenderComponentsLayers{}
{
	Init();
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

void t2g::Scene::Init()
{
	AddObject(eObjectType::Player);
}

void t2g::Scene::AddObject(eObjectType type)
{
	unique_ptr<Object> uptr = Object::CreateObject();
	uptr->SetOwner(this);
	uptr->Init(type);
	mObjects.insert(std::move(uptr));
}

void t2g::Scene::BindComponent(SafePtr<Component> component)
{
	const eUpdateLayer ulayer = component->GetUpdateLayer();
	if (ulayer != eUpdateLayer::EnumEnd)
		mUpdateComponentsLayers[(UINT)ulayer].insert(component);

	const eRenderLayer rlayer = component->GetRenderLayer();
	if (rlayer != eRenderLayer::EnumEnd)
		mUpdateComponentsLayers[(UINT)rlayer].insert(component);
}
