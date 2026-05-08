#pragma once

#include <cstdint>
#include <CustomFunction.h>
#include <Windows.h>

namespace US {
    class Thread;

    enum ThreadStatusCode
    {
        SUCESS		= -1,
        IN_PROGRESS =  0,
        FAILED		=  1
    };

    template<typename T>
    class ThreadHandle
    {
        /* === Enter mCS === */
        CRITICAL_SECTION    mCS;
        const char*         mName;
        const Thread*       mpSelf;

        ThreadStatusCode    mStatusCode;
        bool                mShouldTerminate;
        bool                mIsPaused;
        bool                mHasExited;

        T*                  mData;
        uint32_t            mByteSize;
        uint32_t            mElementCount;
        bool                mInitAsNew;

        void InitDataAsNew(uint32_t elementCount = 1);
        void InitData(T* pData, uint32_t elementCount = 1);

        /* === Leave mCS === */
    public:
        ThreadHandle(const char* name, Thread* pHold, bool startPaused = false);
        ~ThreadHandle();

        const Thread* GetCurrentThread() const;
        const char* GetName() const;
        bool TerminationStatus();
        bool IsPaused();
        bool IsExited();
        ThreadStatusCode GetStatus();

        void SafeTerminate();
        void SafePause();
        void SafeResume();

        T* UnsafeData();
        uint32_t GetDataByteSize();
        uint32_t GetElementCount();
        bool IsCreated();

        /// <summary>
        /// Always call this from curent thread, if you want to exit from an other, call SafeTerminate()
        /// </summary>
        DWORD ExitWithStatus(ThreadStatusCode status);

        //void ChangeStatus(ThreadStatusCode status);

        friend class Thread;
    };

}

#include "ThreadHandle.hpp"