#include <Sand.h>

#include "Debugger.h"
#include "Profiler.h"

#include "CellManager.h"

int main(int argc, char *argv[]) {

    DBG_INIT(US::DBG_ALL, US::OUTPUT_CONSOLE);
    
    srand (static_cast <unsigned> (time(0)));
    
    DWORD MAX_PROCCESSOR_COUNT =  GetMaximumProcessorCount(ALL_PROCESSOR_GROUPS);

    LOG_INFO << "MAX PROCCESSOR COUNT : " << (uint32_t)MAX_PROCCESSOR_COUNT << ENDL;

    US::CellManager* manager = new US::CellManager(1080, 720);
    manager->SetThreadCount(MAX_PROCCESSOR_COUNT);
    manager->Run();

    /*
    * Test sur un proc avec 32 threads avec lanc� de simulation vide :
        1  thread => 40-45   fps
        2  thread => 60-65   fps ~(+50%)
        3  thread => 80-90   fps ~(+100%)
        4  thread => 90-100  fps ~(+125%)
        5  thread => 100-110 fps ~(+150%)
        6  thread => 120-125 fps ~(+200%)
        7  thread => 125-130 fps ~(+213%)
        8  thread => 130-140 fps ~(+225%)
        9  thread => 135-140 fps ~(+238%)
        10 thread => 150-160 fps ~(+275%)
        20 thread => 180-200 fps ~(+350%)
        30 thread => 190-200 fps ~(+375%)
        32 thread => 190-200 fps ~(+375%)
        64 thread => 190-200 fps ~(+375%)
    */

    /*
    * Test sur un proc avec 32 threads avec lanc� de simulation pleine :
        1  thread => ~16        fps
        2  thread => ~20        fps
        3  thread => ~30        fps
        4  thread => ~35        fps
        5  thread => ~40        fps
        6  thread => ~45        fps
        7  thread => ~55        fps
        8  thread => ~55        fps
        9  thread => ~65        fps
        10 thread => ~70        fps
        20 thread => ~95        fps
        30 thread => ~105       fps
        32 thread => ~105       fps
        64 thread => ~105       fps
    */

    delete manager;
    
    DBG_UNINIT();

    _CrtDumpMemoryLeaks();
    
}
