#pragma once
#include <memory>
#include <functional>
#include <unordered_set>
#include <vector>

#include "t2gInterfaces.h"
#include "t2gSafePtr.h"

using std::unique_ptr;
using std::make_unique;
using std::function;
using std::unordered_set;

using t2g::SafePtr;

namespace t2g
{
	class Object;

	class Component
	{
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
		void Update() { update(); }
		void Render() { render(); }
	private:
		virtual void update() = 0;
		virtual void render() = 0;

	public:
		virtual eComponentType GetComponentType() const = 0;
		virtual eUpdateLayer GetUpdateLayer() const = 0;
		virtual eRenderLayer GetRenderLayer() const = 0;
		virtual void SyncBindings() = 0;

	public:
		SafePtr<Object> GetOwner() const { return mOwner; }
		void SetOwner(SafePtr<Object> pObj) { mOwner = pObj; }

		bool IsActive() const { return mIsActive; }

	private:
		SafePtr<Object> mOwner;
		bool mIsActive;
	};
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

