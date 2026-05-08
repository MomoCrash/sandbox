#pragma once
#include <chrono>

namespace US {
    class Clock {

        std::chrono::high_resolution_clock::time_point mLastFrame;
        float m_deltaTime;


    public:
        Clock();

        void    Update();
        float   GetDeltaTime() const;
        float   GetFPS() const;

        static constexpr float FixedDeltaTime = 1.f / 60.f;
        
    };
}