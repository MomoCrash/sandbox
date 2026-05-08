#include "ThreadHandle.h"

namespace US {

    template<typename T>
    void ThreadHandle<T>::InitDataAsNew(uint32_t elementCount) {
        mData           = new T[elementCount];
        mByteSize       = elementCount * sizeof(T);
        mElementCount   = elementCount;
        mInitAsNew = true;
    }

    template<typename T>
    void ThreadHandle<T>::InitData(T *pData, uint32_t elementCount) {
        if (mInitAsNew)
            delete[] mData;

        mData           = pData;
        mByteSize       = elementCount * sizeof(T);
        mElementCount   = elementCount;
        mInitAsNew = false;
    }

    template<typename T>
    ThreadHandle<T>::ThreadHandle(const char* name, Thread* pHold, bool startPaused)
        : mName(name), mpSelf(pHold), mStatusCode(), mData(nullptr), mByteSize(0), mElementCount(0), mInitAsNew(false) {
        mShouldTerminate = false;
        mIsPaused = startPaused;
        mHasExited = false;
        InitializeCriticalSection(&mCS);
    }

    template<typename T>
    ThreadHandle<T>::~ThreadHandle()
    {
        if (mInitAsNew)
            delete[] mData;
        DeleteCriticalSection(&mCS);
    }

    template<typename T>
    const Thread* ThreadHandle<T>::GetCurrentThread() const
    {
        return mpSelf;
    }

    template<typename T>
    const char* ThreadHandle<T>::GetName() const
    {
        return mName;
    }

    template<typename T>
    bool ThreadHandle<T>::TerminationStatus()
    {
        bool Result;
        EnterCriticalSection(&mCS);
        Result = mShouldTerminate;
        LeaveCriticalSection(&mCS);
        return Result;
    }

    template<typename T>
    bool ThreadHandle<T>::IsPaused()
    {
        bool Result;
        EnterCriticalSection(&mCS);
        Result = mIsPaused;
        LeaveCriticalSection(&mCS);
        return Result;
    }

    template<typename T>
    bool ThreadHandle<T>::IsExited() {
        return mHasExited;
    }

    template<typename T>
    ThreadStatusCode ThreadHandle<T>::GetStatus()
    {
        ThreadStatusCode Result;
        EnterCriticalSection(&mCS);
        Result = mStatusCode;
        LeaveCriticalSection(&mCS);
        return Result;
    }

    template<typename T>
    void ThreadHandle<T>::SafeTerminate()
    {
        EnterCriticalSection(&mCS);
        mShouldTerminate = true;
        LeaveCriticalSection(&mCS);
    }

    template<typename T>
    void ThreadHandle<T>::SafePause()
    {
        EnterCriticalSection(&mCS);
        if (mIsPaused == false) {
            mIsPaused = true;
            LeaveCriticalSection(&mCS);
            SuspendThread(mpSelf->GetHandle());
            return;
        }
        LeaveCriticalSection(&mCS);
    }

    template<typename T>
    void ThreadHandle<T>::SafeResume()
    {
        EnterCriticalSection(&mCS);
        if (mIsPaused) {
            ResumeThread(mpSelf->GetHandle());
            mIsPaused = false;
        }
        LeaveCriticalSection(&mCS);
    }

    template<typename T>
    T* ThreadHandle<T>::UnsafeData() {
        return mData;
    }

    template<typename T>
    uint32_t ThreadHandle<T>::GetDataByteSize() {
        return mByteSize;
    }

    template<typename T>
    uint32_t ThreadHandle<T>::GetElementCount() {
        return mElementCount;
    }

    template<typename T>
    bool ThreadHandle<T>::IsCreated() {
        return mInitAsNew;
    }

    template<typename T>
    DWORD ThreadHandle<T>::ExitWithStatus(ThreadStatusCode status)
    {
        EnterCriticalSection(&mCS);
        mHasExited = true;
        mStatusCode = status;
        LeaveCriticalSection(&mCS);
        return status;
    }
}
