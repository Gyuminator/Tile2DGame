#pragma once
#include "t2gInterfaces.h"
#include "t2gMacro.h"
#include <cassert>

namespace t2g
{
	template<typename T>
	class SingletonBase : public IGameLoop
	{
		static_assert(std::is_class<T>::value == true, "SingletonBase<T>: T must be class!");
	public:
		static T& GetInst()
		{
			if (inst == nullptr)
				inst = new T;
			return *inst;
		}
		static void DeleteInst()
		{
			SAFE_DELETE(inst)
		}

	private:
		static T* inst;

	protected:
		SingletonBase<T>() { assert(inst == nullptr && "SingletonBase<T>: Instance already exists!"); }
		virtual ~SingletonBase<T>() {}
	private:
		SingletonBase<T>(const SingletonBase<T>&) = delete;
		SingletonBase<T>& operator=(const SingletonBase<T>&) = delete;
	};

	template<typename T>
	T* SingletonBase<T>::inst = nullptr;
}