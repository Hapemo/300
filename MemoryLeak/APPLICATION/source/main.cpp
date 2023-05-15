/*!*****************************************************************************
\file main.cpp
\author Jazz Teoh Yu Jue, Chen Jia Wen
\par DP email: j.teoh\@digipen.edu, c.jiawen\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
main file of the whole program
*******************************************************************************/

// Extension loader library's header must be included before GLFW's header!!!

#include <Helper.h>
#include "pch.h"
#include "Application.h"
#include "GameStateManager.h"
#include "SignalHandler.h"
#include "SparseSet.h"

// To use dedicated gpu
#ifdef __cplusplus
extern "C" {
#endif

  __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif

/*  _________________________________________________________________________ */
/*! main

@param none

@return int

Indicates how the program existed. Normal exit is signaled by a return value of
0. Abnormal termination is signaled by a non-zero return value.
Note that the C++ compiler will insert a return 0 statement if one is missing.
*/

/*!*****************************************************************************
 \brief
 child main file that runs the application
*******************************************************************************/
void real_main() {
    std::signal(SIGABRT, HandleSignal);
    std::signal(SIGTERM, HandleSignal);
    std::signal(SIGINT, HandleSignal);
    std::signal(SIGSEGV, HandleSignal);
    std::signal(SIGILL, HandleSignal);
    std::signal(SIGFPE, HandleSignal);

    // Part 1
    Application::init();

    // Part 2
    while (!glfwWindowShouldClose(Application::getWindow())) {
      Application::MainUpdate();
        //GameStateManager::GetInstance()->Update();
        //if (GameStateManager::GetInstance()->GetNextGS() == E_GS::EXIT) break;
    }

    // Part 3
    Application::exit();
}

/*!*****************************************************************************
 \brief
 The master main file that runs a child main file and catches exceptions that it
 throws
 
 \int main int
*******************************************************************************/
int main() {
    try {
        Util::EnableMemoryLeakChecking();
        real_main();
    }
    catch (const std::exception& e) {
        std::string exc = e.what();
        LOG_CRASH("The program has caught an exception: " + exc);
        Logger::GetInstance()->EndRun();
    }
}
