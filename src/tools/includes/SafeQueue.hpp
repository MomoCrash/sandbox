#pragma once
#include "SafeQueue.h"

namespace US 
{
    template<typename T>
    inline SafeQueue<T>::SafeQueue() : mSize(0), mpFirst(nullptr), mpLast(nullptr), mCS(nullptr)
    {
        InitializeCriticalSection(&mCS);
    }

    template<typename T>
    inline SafeQueue<T>::~SafeQueue()
    {
        EnterCriticalSection(&mCS);
        Node<T>* pNode;
        for (; mpFirst != nullptr; ) {
            pNode = mpFirst;
            mpFirst = mpFirst->pNext;
            delete pNode;
        }
        LeaveCriticalSection(&mCS);
        DeleteCriticalSection(&mCS);
    }

    template<typename T>
    inline uint64_t SafeQueue<T>::Size()
    {
        uint64_t result;
        EnterCriticalSection(&mCS);
        result = mSize;
        LeaveCriticalSection(&mCS);
        return result;
    }

    template<typename T>
    inline void SafeQueue<T>::EnQueue(const T& value)
    {
        Node<T>* pNode = new Node<T>();
        pNode->Value = value;
        pNode->pPrevious = nullptr;

        EnterCriticalSection(&mCS);
        pNode->pNext = mpFirst;
        if (mpFirst == nullptr) {
            mpLast = pNode;
        }
        else {
            mpFirst->pPrevious = pNode;
        }
        mpFirst = pNode;
        mSize++;
        LeaveCriticalSection(&mCS);
    }

    template<typename T>
    inline T SafeQueue<T>::DeQueue()
    {
        Node<T>* pNode;

        EnterCriticalSection(&mCS);
        pNode = mpLast;
        if (pNode->pPrevious == nullptr) {
            mpFirst = nullptr;
        }
        else {
            mpLast->pNext = nullptr;
        }
        mpLast = pNode->pPrevious;
        T result = pNode->Value;
        mSize--;
        LeaveCriticalSection(&mCS);

        delete pNode;
        return result;
    }
}