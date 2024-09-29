#pragma once

namespace t2g
{
	template<typename T>
	class SafePtr
	{
	public:
		SafePtr() : ptr(nullptr) {}
		SafePtr(T* p) : ptr(p) {}
		template<typename T1>
		SafePtr(SafePtr<T1> other) : ptr(static_cast<T*>((T1*)other.get())) {}

	public:
		T& operator*() const { return *ptr; }
		T* operator->() const { return ptr; }
		bool operator==(const SafePtr& other) const
		{
			return this->ptr == other.ptr;
		}

	public:
		bool IsEmpty() const { return ptr == nullptr; }
		bool IsValid() const { return ptr != nullptr; }
		T* get() const { return ptr; }

	private:
		T* ptr;
	};
}

template <typename T>
struct std::hash<t2g::SafePtr<T>>
{
	size_t operator()(const t2g::SafePtr<T>& t) const
	{
		hash<T*> hash_func;

		return hash_func(t.get());
	}
};
