#pragma once
#include "stdafx.h"

#include "t2gInterfaces.h"
#include "t2gMacro.h"

namespace t2g
{
	template<typename T>
	class SingletonBase : public IGameLoop
	{
		static_assert(std::is_class<T>::value == true, "SingletonBase<T>: T must be class");
	public:
		static T& GetInst()
		{
			static_assert(std::is_base_of<SingletonBase<T>, T>::value == true, "SingletonBase<T>: SingletonBase<T> must be Base of T");
			if (inst.get() == nullptr)
				inst.reset(new T);
			return *inst.get();
		}
		static void DeleteInst()
		{
			inst.reset();
		}

	public:

	protected:
		SingletonBase<T>() { assert(inst == nullptr && "SingletonBase<T>: Instance already exists"); }
		virtual ~SingletonBase<T>() {}

	private:
		static std::unique_ptr<T> inst;

	private:
		SingletonBase<T>(const SingletonBase<T>&) = delete;
		SingletonBase<T>& operator=(const SingletonBase<T>&) = delete;
	};

	template<typename T>
	std::unique_ptr<T> SingletonBase<T>::inst = std::make_unique<T>();
}