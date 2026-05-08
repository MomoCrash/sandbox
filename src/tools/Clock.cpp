#include "Clock.h"

namespace US {
    Clock::Clock() {
        mLastFrame = std::chrono::high_resolution_clock::now();
    }

    void Clock::Update() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        unsigned long long elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - mLastFrame).count();
        m_deltaTime =  (elapsedTime / 1000000000.0f);
        mLastFrame = currentTime;
    }

    float Clock::GetDeltaTime() const {
        return m_deltaTime;
    }

    float Clock::GetFPS() const {
        return 1.0f / GetDeltaTime();
    }
}
