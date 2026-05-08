#pragma once
#include <cstdint>
#include <memory>
#include <Windows.h>
#include <CustomFunction.h>

namespace US {


	template<typename T>
	class UnorderedArray {
		T* mpData;
		uint64_t mSize;
		uint64_t mCapacity;
		CRITICAL_SECTION mCS;

		/* === INTERNAL (NO CRITICAL SECTION) === */
		void		InternalRealloc(uint64_t cap);
		void		InternalAdd(const T& value);
		void		InternalRemoveAt(uint64_t index);
		void		InternalExecute(US::CustomFunction<void, uint64_t, T&>& function);
		T&			InternalAccess(uint64_t index);
		uint64_t	InternalSize() const;

	public:
		UnorderedArray();
		~UnorderedArray();

		void Add(const T& value);
		void RemoveAt(uint64_t index);
		void Execute(US::CustomFunction<void, uint64_t, T&>& function);

		T& operator[](uint64_t index);

		uint64_t Size();
	};
	 

}

#include "UnorderedArray.hpp"