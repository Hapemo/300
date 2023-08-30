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
#include "Editor/Editor.h"
#include "GLFW/glfw3.h"
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
void real_main(); 

/*!*****************************************************************************
 \brief
 The master main file that runs a child main file and catches exceptions that it
 throws
 
 \int main int
*******************************************************************************/
int main() {
    try {
        Misc::MemLeakCheck();
        real_main();
    }
    catch (const std::exception& e) {
        std::string exc = e.what();
        std::cout << exc << std::endl;
    }
}

void real_main() {
    //std::signal(SIGABRT, HandleSignal);
    //std::signal(SIGTERM, HandleSignal);
    //std::signal(SIGINT, HandleSignal);
    //std::signal(SIGSEGV, HandleSignal);
    //std::signal(SIGILL, HandleSignal);
    //std::signal(SIGFPE, HandleSignal);

    // Part 1
    EditorWindow::Mainwindow = new Editor;

    Application::Init();
    EditorWindow::Mainwindow->UIinit();
    
    // Part 2
    while (!glfwWindowShouldClose(/*Application::getWindow()*/EditorWindow::Mainwindow->_Editorwindow)) {
        Application::MainUpdate();

        EditorWindow::Mainwindow->UIupdate();
        //EditorWindow::Mainwindow->WindowUpdate();
        EditorWindow::Mainwindow->UIdraw(EditorWindow::Mainwindow->_Editorwindow);
        EditorWindow::Mainwindow->WindowUpdate();

        //GameStateManager::GetInstance()->Update();
        //if (GameStateManager::GetInstance()->GetNextGS() == E_GS::EXIT) break;
    }
    EditorWindow::Mainwindow->UIend();

    // Part 3
    Application::Exit();
}
