#include "UnorderedArray.h"

namespace US {

	template<typename T>
	inline void UnorderedArray<T>::InternalRealloc(uint64_t cap)
	{
		mCapacity = cap;
		T* temp = new T[mCapacity];
		memcpy(temp, mpData, mSize * sizeof(T));
		delete[] mpData;
		mpData = temp;
	}

	template<typename T>
	inline void UnorderedArray<T>::InternalAdd(const T& value)
	{
		mpData[mSize++] = value;
		if (mSize == mCapacity) {
			InternalRealloc(mCapacity * 2);
		}
	}

	template<typename T>
	inline void UnorderedArray<T>::InternalRemoveAt(uint64_t index)
	{
		mpData[index] = mpData[--mSize];
	}

	template<typename T>
	inline void UnorderedArray<T>::InternalExecute(US::CustomFunction<void, uint64_t, T&>& function)
	{
		for (uint64_t i = 0; i < InternalSize(); ++i) {
			function(i, InternalAccess(i));
		}
	}

	template<typename T>
	inline T& UnorderedArray<T>::InternalAccess(uint64_t index)
	{
		return mpData[index];
	}

	template<typename T>
	inline uint64_t UnorderedArray<T>::InternalSize() const
	{
		return mSize;
	}

	template<typename T>
	inline UnorderedArray<T>::UnorderedArray()
	{
		mpData = new T[32];
		mSize = 0;
		mCapacity = 32;
		InitializeCriticalSection(&mCS);
	}

	template<typename T>
	inline UnorderedArray<T>::~UnorderedArray()
	{
		EnterCriticalSection(&mCS);
		delete[] mpData;
		mpData = nullptr;
		mSize = 0;
		mCapacity = 0;
		LeaveCriticalSection(&mCS);
		DeleteCriticalSection(&mCS);
	}

	template<typename T>
	inline void UnorderedArray<T>::Add(const T& value)
	{
		EnterCriticalSection(&mCS);
		InternalAdd(value);
		LeaveCriticalSection(&mCS);
	}

	template<typename T>
	inline void UnorderedArray<T>::RemoveAt(uint64_t index)
	{
		EnterCriticalSection(&mCS);
		InternalRemoveAt(index);
		LeaveCriticalSection(&mCS);
	}

	template<typename T>
	inline void UnorderedArray<T>::Execute(US::CustomFunction<void, uint64_t, T&>& function)
	{
		EnterCriticalSection(&mCS);
		InternalExecute(function);
		LeaveCriticalSection(&mCS);
	}

	template<typename T>
	inline T& UnorderedArray<T>::operator[](uint64_t index)
	{
		EnterCriticalSection(&mCS);
		T& value = InternalAccess(index);
		LeaveCriticalSection(&mCS);
		return value;
	}

	template<typename T>
	inline uint64_t UnorderedArray<T>::Size()
	{
		uint64_t size;
		EnterCriticalSection(&mCS);
		size = InternalSize();
		LeaveCriticalSection(&mCS);
		return size;
	}
}