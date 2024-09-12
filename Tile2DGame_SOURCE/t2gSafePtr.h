#pragma once

namespace t2g
{
	template<typename T>
	class SafePtr
	{
		friend size_t std::hash<SafePtr<T>>::operator()(const SafePtr<T>& t) const;

	public:
		SafePtr() : ptr(nullptr) {}
		SafePtr(T* p) : ptr(p) {}
		template<typename T1>
		SafePtr(SafePtr<T1> other) : ptr(static_cast<T*>((T1*)other.GetKey())) {}

	public:
		T& operator*() { return *ptr; }
		T* operator->() { return ptr; }
		bool operator==(const SafePtr& other) const
		{
			return this->ptr == other.ptr;
		}

	public:
		bool IsEmpty() { return ptr == nullptr; }
		bool IsValid() { return ptr != nullptr; }
		long long GetKey() { return (long long)ptr; }

	private:
		T* ptr;
	};
}

namespace std
{
	using namespace t2g;

	template <typename T>
	struct std::hash<SafePtr<T>>
	{
		size_t operator()(const SafePtr<T>& t) const {
			hash<T*> hash_func;

			return hash_func(t.ptr);
		}
	};
}
