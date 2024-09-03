#pragma once
#include "t2gInterfaces.h"
#include "t2gSafePtr.h"

class Object;

namespace t2g
{
	class Component : public IGameLoop
	{
	public:
		Component() : mOwner(nullptr) {}
		virtual ~Component() {}

	public:
		void Init() override;
		void Update() override;
		void Render() override;
		void Release() override;

	public:
		void SetOwner(Object* pObj) { mOwner = pObj; }
		SafePtr<Object> GetOwner() { return mOwner; }

	private:
		SafePtr<Object> mOwner;

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

