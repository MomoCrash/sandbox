#pragma once

#include <CustomFunction.h>
#include <Thread.h>
#include <vector>

namespace US {

	template<typename Data>
	struct ThreadInfo {
		int			Layer;
		US::Thread* pThread;
		Data*		pData;

		ThreadInfo();
	};

	template<typename Data>
	class Scheduler {

	protected:

		struct ThreadProcDescriptor {
			Scheduler* pScheduler;
			int8_t index;
		};
		
		US::CustomFunction<bool, ThreadInfo<Data>*>*	ProcessPtr;
		
		bool		mIsTerminated;
	        
		uint8_t     mCurrentLayer;
		uint8_t     mLayerCount;
		uint8_t     mMaxThreadCount;
		
		std::vector<ThreadInfo<Data>>		mThreadInfo;
		std::vector<ThreadProcDescriptor*>	mThreadProcInfo;

		void ResumeCurrentLayer();
	
	public:
		Scheduler(US::CustomFunction<bool, ThreadInfo<Data>*>* pr, std::vector<ThreadInfo<Data>> data);
		~Scheduler();

	    void Update();
		ThreadInfo<Data>* GetThreadInfo(uint32_t index);
		bool IsLayerFinished();

		/// Swap layers 
		/// @return If is the last layer of the scheduler
		bool SwapLayer();

		void TerminateWait();
		
		template<typename Data>
		static DWORD Function(ThreadHandle<ThreadProcDescriptor>* pHandle) {
			ThreadProcDescriptor* data = reinterpret_cast<ThreadProcDescriptor *>(pHandle->UnsafeData());
			Scheduler* pScheduler = data->pScheduler;;
        
			while (!pHandle->TerminationStatus()) {
				
				if (pHandle->IsPaused()) continue;

				(*pScheduler->ProcessPtr)(pScheduler->GetThreadInfo(data->index));

				pHandle->SafePause();
				
			}

			return pHandle->ExitWithStatus(SUCESS);
		}
		
	};
	
}

#include "Scheduler.hpp"
