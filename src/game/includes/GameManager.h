#pragma once

#include "Clock.h"

namespace US {
    class GameManager {

        Clock               mGlobalClock;

        bool mRunning;
        
    public:

        void Run();
        void Stop();
        
        bool IsRunning();
        Clock const& GetClock() const;

    protected:
        GameManager();
        ~GameManager();
        
        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void End()   = 0; 

    };
}
