#include "t2gObject.h"

t2g::Object::Object()
	: mName{}
	, mLayerTag(eLayerTag::END)
	, mComponents{}
	, mAttacher{}
	, mAttachedObjects{}
{
	Init();
}

t2g::Object::~Object()
{
	Release();
}

void t2g::Object::Init()
{
	
}

void t2g::Object::Update()
{
	for (eComponentType type : mComponentKeys)
	{
		mComponents[type];
	}
}

void t2g::Object::Render()
{
}

void t2g::Object::Release()
{
}

