#include "GameManager.h"

#include "crtdbg.h"
#include "Profiler.h"

#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

namespace US {
    
    GameManager::GameManager() :
        mRunning(false) {
    }

    GameManager::~GameManager() {
    }

    void GameManager::Run() {
        Start();
        
        if (mRunning) return;

        mRunning = true;
        float accumulator = 0;

        while (mRunning) {

            Update();
            mGlobalClock.Update();
            
        }
        
        End();

    }

    void GameManager::Stop() {
        mRunning = false;
    }

    bool GameManager::IsRunning() {
        return mRunning;
    }

    Clock const & GameManager::GetClock() const {
        return mGlobalClock;
    }
} // US