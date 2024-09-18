#pragma once
#include "t2gInterfaces.h"
#include "t2gSafePtr.h"
#include "t2gEnums.h"

using t2g::SafePtr;

namespace t2g
{
	class Object;
	class Scene;

	class Component
	{
	public:
		typedef eDelegateResult(Component::* DynamicDelegate)();
		typedef DynamicDelegate DD;
		//typedef function<eDelegateResult(Component&)> DynamicDelegate;
		typedef list<DynamicDelegate> MultiDynamicDelegate;
		typedef MultiDynamicDelegate MDD;

	public:
		template<typename T>
		static unique_ptr<T> CreateComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Component::CreateComponent: T must derived Component");
			unique_ptr<T> uptr(new T);
			return std::move(uptr);
		}

	protected:
		Component() : mOwner(nullptr), mIsActive(true) {}
	public:
		virtual ~Component() { }

	public:
		void Update();
		void Render();

	public:
		virtual eComponentType GetComponentType() const = 0;
		virtual eUpdateLayer GetUpdateLayer() const = 0;
		virtual eRenderLayer GetRenderLayer() const = 0;
		virtual void SyncWithOtherComponents() = 0;

		virtual void BindToScene(SafePtr<Scene> scene);

	public:
		SafePtr<Object> GetOwner() const { return mOwner; }
		void SetOwner(SafePtr<Object> pObj) { mOwner = pObj; }

		bool IsActive() const { return mIsActive; }

	public:
		template<typename T>
		void BindBackToUpdates(eDelegateResult(T::* dd)());
		template<typename T>
		void BindFrontToUpdates(eDelegateResult(T::* dd)());
		template<typename T>
		void BindBackToRenders(eDelegateResult(T::* dd)());
		template<typename T>
		void BindFrontToRenders(eDelegateResult(T::* dd)());

	private:
		void procDelegates(MultiDynamicDelegate& dd);

	private:
		MDD mUpdates;
		MDD mRenders;

		SafePtr<Object> mOwner;
		bool mIsActive;
	};

	template<typename T>
	void Component::BindBackToUpdates(eDelegateResult(T::* dd)())
	{
		DD castedDD = static_cast<DD>(dd);
		mUpdates.push_back(castedDD);
	}
	template<typename T>
	void Component::BindFrontToUpdates(eDelegateResult(T::* dd)())
	{
		DD castedDD = static_cast<DD>(dd);
		mUpdates.push_front(castedDD);
	}
	template<typename T>
	void Component::BindBackToRenders(eDelegateResult(T::* dd)())
	{
		DD castedDD = static_cast<DD>(dd);
		mRenders.push_back(castedDD);
	}
	template<typename T>
	void Component::BindFrontToRenders(eDelegateResult(T::* dd)())
	{
		DD castedDD = static_cast<DD>(dd);
		mRenders.push_front(castedDD);
	}
}






#pragma region MyRegion
/*
#include <Windows.h>
#include <tuple>

#include "t2gInterfaces.h"
#include "t2gSafePtr.h"

using std::tuple;
using std::tuple_element_t;
using std::get;

class Object;

namespace t2g
{
	template<typename... T>
	class Component : public IComponent
	{
	public:
		Component<T...>() {}
		virtual ~Component<T...>() {}

	public:
		template<UINT8 I>
		void setData(tuple_element_t<I, tuple<T...>> data) { get<I>(mDatas) = data; }

		template<UINT8 I>
		tuple_element_t<I, tuple<T...>> getData() { return get<I>(mDatas); }

	private:
		SafePtr<Object> mOwner;
		tuple<T...> mDatas;

	};
}
*/
#pragma endregion

