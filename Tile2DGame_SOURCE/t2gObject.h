#pragma once

#include "t2gInterfaces.h"
#include "t2gComponent.h"
#include "t2gEnums.h"
#include "t2gSafePtr.h"
#include "t2gTypes.h"

using t2g::enums::eComponentType;
using t2g::enums::eObjectTag;
using t2g::enums::eRenderLayer;
using t2g::enums::eUpdateLayer;
using t2g::SafePtr;
using t2g::Component;

namespace t2g
{
	class Scene;

	class Object
	{
	private:
		static UINT AccID;

	public:
		typedef unordered_map<eComponentType, unique_ptr<Component>> UniqueComponentMap;
		typedef unordered_set<SafePtr<Component>> SafeComponentSet;

	public:
		static unique_ptr<Object> CreateObject()
		{
			unique_ptr<Object> uptr(new Object);

			return std::move(uptr);
		}

	private:
		Object();
	public:
		~Object();

	public:
		void Init(eObjectTag type);

	public:

	public:
		template<typename T>
		decltype(auto) AddComponent();

		void BindComponentsToScene();
		void SyncComponents();

		void BindBackEvent(eEventCallPoint callPoint, function<eDelegateResult()> func) { mEvents[callPoint].push_back(func); }
		void BindFrontEvent(eEventCallPoint callPoint, function<eDelegateResult()> func) { mEvents[callPoint].push_front(func); }
		void EventProc(eEventCallPoint callPoint);

	private:

	public:
		const wstring& GetName() const { return mName; }
		SafePtr<Scene> GetOwnerScene() { return mOwnerScene; }
		UINT GetID() { return mID; }
		eObjectTag GetTag() { return mTag; }

		void SetName(const wstring& name) { mName = name; }
		void SetOwnerScene(SafePtr<Scene> scene) { mOwnerScene = scene; }
		void SetTag(eObjectTag tag) { mTag = tag; }


		const SafePtr<Component> GetComponent(eComponentType type);
		template<typename T>
		SafePtr<T> GetComponent(eComponentType type);

	private:
		SafeComponentSet mUpdateComponents;
		SafeComponentSet mRenderComponents;

		UniqueComponentMap mComponents;

		Events mEvents;

		wstring mName;

		vector<SafePtr<Object>> mAttachedObjects;
		SafePtr<Object> mAttacher;

		SafePtr<Scene> mOwnerScene;

		UINT mID;
		eObjectTag mTag;
	};

	template<typename T>
	SafePtr<T> Object::GetComponent(eComponentType type)
	{
		const auto& pair = mComponents.find(type);
		if (pair == mComponents.end())
			return SafePtr<T>(nullptr);

		return static_cast<SafePtr<T>>(SafePtr<Component>(pair->second.get()));
	}

	template<typename T>
	decltype(auto) Object::AddComponent()
	{
		unique_ptr<T> uptr = Component::CreateComponent<T>();
		uptr->SetOwnerObj(this);

		SafePtr<T> sptr = uptr.get();

		eComponentType type = uptr->GetComponentType();
		mComponents.emplace(type, std::move(uptr));

		return sptr;
	}
}


