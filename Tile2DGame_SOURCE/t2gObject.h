#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

#include "t2gInterfaces.h"
#include "t2gComponent.h"
#include "t2gEnums.h"
#include "t2gSafePtr.h"

using std::wstring;
using std::unique_ptr;
using std::make_unique;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::function;

using t2g::enums::eComponentType;
using t2g::enums::eObjectType;
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
		void Init(eObjectType type);

	public:
		UINT GetID() { return mID; }

	public:
		template<typename T>
		decltype(auto) AddComponent();

	private:
		void BindComponentToScene(SafePtr<Component> component);
		void SyncComponents();

	public:
		const wstring& GetName() const { return mName; }
		void SetName(const wstring& name) { mName = name; }

		SafePtr<Scene> GetOwner() { return mOwner; }
		void SetOwner(SafePtr<Scene> scene) { mOwner = scene; }

		const SafePtr<Component> GetComponent(eComponentType type);

	private:
		UINT mID;

		wstring mName;
		SafePtr<Scene> mOwner;

		SafeComponentSet mUpdateComponents;
		SafeComponentSet mRenderComponents;

		UniqueComponentMap mComponents;

		SafePtr<Object> mAttacher;
		vector<SafePtr<Object>> mAttachedObjects;
	};
}


namespace t2g
{
	template<typename T>
	decltype(auto) Object::AddComponent()
	{
		unique_ptr<T> uptr = Component::CreateComponent<T>();
		uptr->SetOwner(this);
		BindComponentToScene(uptr.get());

		SafePtr<T> result = uptr.get();

		eComponentType type = uptr->GetComponentType();
		mComponents.emplace(type, std::move(uptr));

		return result;
	}
}

