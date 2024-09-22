#include "stdafx.h"
#include "t2gComponent.h"

#include "t2gScene.h"

t2g::Component::Component()
	: mChildren{}
	, mUpdates{}
	, mRenders{}
	, mOwnerObj{}
	, mOwnerComponent{}
	, mChildID(-1)
	, mChildLayer(eChildrenLayer::EnumEnd)
	, mUpdateLayer(eUpdateLayer::EnumEnd)
	, mRenderLayer(eRenderLayer::EnumEnd)
	, mIsActive(true)
{
}

void t2g::Component::BindToScene(SafePtr<Scene> scene)
{
	scene->BindComponent(this);
}

void t2g::Component::procDelegates(MultiDynamicDelegate& delegates)
{
	for (auto iter = delegates.begin(); iter != delegates.end();)
	{
		switch ((this->*(*iter))())
		{
		case eDelegateResult::Erase:
			iter = delegates.erase(iter);
			continue;
		case eDelegateResult::Return:
			return;
		}
		++iter;
	}
}


void t2g::Component::Update()
{
	for (auto& child : mChildren[eChildrenLayer::Before])
		child->Update();
	procDelegates(mUpdates);
	for (auto& child : mChildren[eChildrenLayer::After])
		child->Update();
}

void t2g::Component::Render()
{
	for (auto& child : mChildren[eChildrenLayer::Before])
		child->Render();
	procDelegates(mRenders);
	for (auto& child : mChildren[eChildrenLayer::After])
		child->Render();
}