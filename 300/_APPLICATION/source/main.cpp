/*!*****************************************************************************
\file main.cpp
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Memory Leak Studios
\date 15/05/2023
\brief
main file of the whole program
*******************************************************************************/
#include "Application.h"
#include "Misc.h"
#include "pch.h"
// Extension loader library's header must be included before GLFW's header!!!
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
void RealMain(); 

/*!*****************************************************************************
 \brief
 The master main file that runs a child main file and catches exceptions that it
 throws
 
 \int main int
*******************************************************************************/
int main() {
    try 
    {
        Misc::MemLeakCheck();
        RealMain();
    }
    catch (const std::exception& e) 
    {
        std::string exc = e.what();
        std::cout << exc << std::endl;
    }
}

void RealMain() 
{
    Application::Init();
    Application::MainUpdate();
    Application::Exit();
}
