#pragma once

namespace t2g
{
	template<typename T>
	class SafePtr
	{
	public:
		SafePtr() : ptr(nullptr) {}
		SafePtr(T* p) : ptr(p) {}

	public:
		T& operator*() { return *ptr; }
		T* operator->() { return ptr; }

	public:
		bool IsValid() { return ptr != nullptr; }

	private:
		T* ptr;
	};
}

