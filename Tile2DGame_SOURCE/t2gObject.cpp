#include "t2gObject.h"
#include "t2gScene.h"
#include "t2gTransform.h"
#include "t2gPlayerController.h"
#include "t2gImageRenderer.h"

t2g::Object::Object()
	: mName{}
	, mAttacher{}
	, mAttachedObjects{}
{
}

t2g::Object::~Object()
{

}

void t2g::Object::Init(eObjectType type)
{
	switch (type)
	{
	case eObjectType::Player:
	{
		AddComponent<Transform>();
		AddComponent<PlayerController>();
		AddComponent<ImageRenderer>();
		SafePtr<ImageRenderer> imageRenderer = GetComponent(eComponentType::ImageRenderer);
		imageRenderer->SetImageName(eImageName::Player);
		imageRenderer->SetFrame(3, 4, 0, 0);
		break;
	}
	default:
		break;
	}

	SyncComponents();
}

void t2g::Object::BindComponentToScene(SafePtr<Component> component)
{
	if (mOwner.IsEmpty())
		return;

	mOwner->BindComponent(component);
}

void t2g::Object::SyncComponents()
{
	for (auto& pair : mComponents)
	{
		pair.second->SyncBindings();
	}
}

const SafePtr<Component> t2g::Object::GetComponent(eComponentType type)
{
	const auto& pair = mComponents.find(type);
	if (pair == mComponents.end())
		return SafePtr<Component>(nullptr);

	return pair->second.get();
}

