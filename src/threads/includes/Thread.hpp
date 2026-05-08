#include "Thread.h"

namespace US {

    template<typename T>
    void Thread::Start(US::CustomFunction<DWORD, ThreadHandle<T> *> *pFunc, uint32_t tElementCount) {
        if (mHandle == nullptr) {
            ThreadHandle<T>* data = static_cast<ThreadHandle<T>*>(mhThread);
            if (!data->IsCreated())
                data->InitDataAsNew(tElementCount);
            mHandle = CreateThread(
                NULL,
                0,
                (LPTHREAD_START_ROUTINE)(pFunc->Ptr()),
                mhThread,
                (getHandlePtr()->IsPaused() ? CREATE_SUSPENDED : 0),
                (DWORD*)&mID
            );
        }
    }

    template<typename T>
    void Thread::Start(US::CustomFunction<DWORD, ThreadHandle<T> *> *pFunc, T *pData, uint32_t tElementCount) {
        if (mHandle == nullptr) {
            static_cast<ThreadHandle<T>*>(mhThread)->InitData(pData, tElementCount);
            mHandle = CreateThread(
                NULL,
                0,
                (LPTHREAD_START_ROUTINE)(pFunc->Ptr()),
                mhThread,
                (getHandlePtr()->IsPaused() ? CREATE_SUSPENDED : 0),
                (DWORD*)&mID
            );
        }
    }

    template<typename T>
    int Thread::TryCopy(T **dst) {

        if (dst != nullptr) return -1;

        ThreadHandle<T>* data = static_cast<ThreadHandle<T>*>(mhThread);

        if (!data->IsExited()) return -1;

        *dst = new T[data->GetElementCount()];
        memcpy(*dst, data->UnsafeData(), data->GetDataByteSize());

        return data->GetElementCount();
    }
}
