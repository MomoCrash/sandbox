#pragma once

#include "ThreadHandle.h"

namespace US
{

	class Thread 
	{
		void* mhThread;
		HANDLE mHandle;
		uint32_t mID;

		ThreadHandle<char>* getHandlePtr() const;

	public:
		Thread(const char* name = "Unammed Thread", bool startPaused = false);
		~Thread();

		template<typename T>
		void Start(US::CustomFunction<DWORD, ThreadHandle<T>*>* pFunc, uint32_t tElementCount = 0);
		template<typename T>
		void Start(US::CustomFunction<DWORD, ThreadHandle<T>*>* pFunc, T* pData, uint32_t tElementCount = 0);

		HANDLE GetHandle() const;
		uint32_t GetID() const;
		ThreadStatusCode GetStatus();

		/// <summary>
		/// Pause the thread IF it hasn't been Paused else this function does nothing
		/// </summary>
		void Pause();
		
		bool IsPaused();

		/// <summary>
		/// Resume the thread IF it has been Paused else this function is ignored
		/// </summary>
		void Resume();

		/// <summary>
		///  Blocking Function Terminate
		/// </summary>
		ThreadStatusCode TerminateWait(uint32_t maxDuration = INFINITE);
		/// <summary>
		/// Non-blocking Function Terminate
		/// </summary>
		void Terminate();

		template<typename T>
		int TryCopy(T** dst);

	};
}

#include "Thread.hpp"