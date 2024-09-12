#include "stdafx.h"
#include "t2gObject.h"

#include "t2gScene.h"
#include "t2gTransform.h"
#include "t2gPlayerController.h"
#include "t2gImageRenderer.h"

t2g::Object::Object()
	: mID(AccID++)
	, mName{}
	, mAttacher{}
	, mAttachedObjects{}
{
}

t2g::Object::~Object()
{

}

void t2g::Object::Init(eObjectType type)
{


}

void t2g::Object::BindComponentsToScene()
{
	assert(mOwner.IsValid() && "Object::BindComponentsToScene: Empty mOwner");

	for (auto& pair : mComponents)
	{
		pair.second->BindToScene(mOwner);
	}
}

void t2g::Object::SyncComponents()
{
	for (auto& pair : mComponents)
	{
		pair.second->SyncWithOtherComponents();
	}
}

const SafePtr<Component> t2g::Object::GetComponent(eComponentType type)
{
	const auto& pair = mComponents.find(type);
	if (pair == mComponents.end())
		return SafePtr<Component>(nullptr);

	return pair->second.get();
}

