#include "stdafx.h"
#include "t2gObject.h"

#include "t2gScene.h"
#include "t2gTransform.h"
#include "t2gPlayerController.h"
#include "t2gImageRenderer.h"

t2g::Object::Object()
	: mUpdateComponents{}
	, mRenderComponents{}
	, mComponents{}
	, mEvents{}
	, mName{}
	, mAttachedObjects{}
	, mAttacher{}
	, mOwnerScene{}
	, mID(AccID++)
	, mTag(eObjectTag::EnumEnd)
{
}

t2g::Object::~Object()
{

}

void t2g::Object::Init(eObjectTag type)
{
}

void t2g::Object::BindComponentsToScene()
{
	assert(mOwnerScene.IsValid() && "Object::BindComponentsToScene: Empty mOwnerScene");

	for (auto& pair : mComponents)
	{
		pair.second->BindToScene(mOwnerScene);
	}
}

void t2g::Object::SyncComponents()
{
	for (auto& pair : mComponents)
	{
		pair.second->SyncWithOtherComponents();
	}
}

void t2g::Object::EventProc(eEventCallPoint callPoint)
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

const SafePtr<Component> t2g::Object::GetComponent(eComponentType type)
{
	const auto& pair = mComponents.find(type);
	if (pair == mComponents.end())
		return SafePtr<Component>(nullptr);

	return pair->second.get();
}

