#pragma once

#include "Scheduler.h"

namespace US {
    template<typename Data>
    ThreadInfo<Data>::ThreadInfo() : Layer(0), pThread(nullptr), pData(nullptr) {
    }

    template<typename Data>
    void Scheduler<Data>::ResumeCurrentLayer() {
        for (auto& info : mThreadInfo) {
            if (info.Layer == mCurrentLayer) {
                info.pThread->Resume();
            }
        }
    }

    template<typename Data>
    Scheduler<Data>::Scheduler(US::CustomFunction<bool, ThreadInfo<Data>*>* func, std::vector<ThreadInfo<Data>> data)
    : mIsTerminated(false), mCurrentLayer(0), mLayerCount(0), mMaxThreadCount(0) {

        ProcessPtr = func;

        mMaxThreadCount = data.size();
        mThreadInfo = data;
        
        for (auto& tData : data) {
            if (mLayerCount < tData.Layer)
                mLayerCount = tData.Layer;
        }
        ++mLayerCount;
        
        for (int i = 0; i < data.size(); i++) {
            auto threadFunc = US::MakeFunction(Function<Data>);
            Thread* runThread = new Thread("cellThread", true);
            ThreadProcDescriptor* desc = new ThreadProcDescriptor();
            desc->index = i;
            desc->pScheduler = this;
            mThreadProcInfo.push_back(desc);
            mThreadInfo[i].pThread = runThread;
            runThread->Start<ThreadProcDescriptor>(&threadFunc, desc);
        }
    }

    template<typename Data>
    Scheduler<Data>::~Scheduler() {

        assert(mIsTerminated);
        
        for (auto info : mThreadProcInfo) {
            delete info;
        }
        
        for (auto& info : mThreadInfo) {
            delete info.pThread;
            if (info.pData != nullptr)
                delete info.pData;
        }
        
    }

    template<typename Data>
    void Scheduler<Data>::Update() {

        if (mIsTerminated) return;

        ResumeCurrentLayer();
		
        // Check if all thread have reach the end
        bool layerUpdateFinish = false;
        while (!layerUpdateFinish) {
            layerUpdateFinish = IsLayerFinished();
                
            if (!layerUpdateFinish) continue;
            
            layerUpdateFinish = SwapLayer();
            if (!layerUpdateFinish) {
                ResumeCurrentLayer();
            }
            
        }
    }

    template<typename Data>
    ThreadInfo<Data>* Scheduler<Data>::GetThreadInfo(uint32_t index) {
        return &mThreadInfo[index];
    }

    template<typename Data>
    bool Scheduler<Data>::IsLayerFinished() {
        bool layerUpdateFinish = true;
        for (auto& info : mThreadInfo) {
            if (info.Layer != mCurrentLayer) continue;
            layerUpdateFinish &= info.pThread->IsPaused();
        }
        return layerUpdateFinish;
    }

    template<typename Data>
    bool Scheduler<Data>::SwapLayer() {
        mCurrentLayer++;
        if (mCurrentLayer == mLayerCount) {
            mCurrentLayer = 0;
            return true;
        } else {
            return false;
        }
    }

    template<typename Data>
    void Scheduler<Data>::TerminateWait() {
        mIsTerminated = true;
        for (auto& info : mThreadInfo) {
            info.pThread->Resume();
            info.pThread->TerminateWait();
        }
    }
    
}
