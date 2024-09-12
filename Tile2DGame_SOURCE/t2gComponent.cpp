#include "stdafx.h"
#include "t2gComponent.h"

#include "t2gScene.h"

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
	procDelegates(mUpdates);
}

void t2g::Component::Render()
{
	procDelegates(mRenders);
}