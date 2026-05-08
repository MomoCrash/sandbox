#pragma once

#include <cstdint>
#include <Windows.h>

namespace US {

    template<typename T>
    struct Node {
        T Value;
        Node<T>* pNext;
        Node<T>* pPrevious;
    };

    template<typename T>
    class SafeQueue {
        Node<T>* mpFirst;
        Node<T>* mpLast;
        uint64_t mSize;
        CRITICAL_SECTION mCS;
    public:
        SafeQueue();
        ~SafeQueue();
        uint64_t Size();

        void EnQueue(const T& value);
        T DeQueue();
    };

} // US

#include "SafeQueue.hpp"
