/*!*****************************************************************************
\file main.cpp
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Memory Leak Studios
\date 15/05/2023
\brief
Main application that gets called in the main loop. It handles the creation and
start up of window and game system, also runs their update functions.
*******************************************************************************/
#include "Application.h"
#include "FPSManager.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Input.h"
#include "SingletonManager.h"
#include "ScriptingSystem.h"

// Static variables
GFX::DebugRenderer* Application::mRenderer;
GFX::Window Application::mWindow;
std::string Application::title;
SystemManager Application::systemManager;

void Application::Init() 
{
    StartUp();
    SystemInit();
}

void Application::StartUp() 
{
    //gfx glew and glfw startup
    GFX::Window::InitializeSystem();
    mWindow = GFX::Window({ 1920, 1080 });
    mWindow.SetWindowTitle("Application");
}

void Application::SystemInit() 
{
    FPSManager::Init(&mWindow);
    Input::Init(&mWindow);
    systemManager.Init();
    //gfx init
}

void Application::FirstUpdate() 
{
    mWindow.PollEvents();
}

void Application::SystemUpdate() 
{
    systemManager.Update(1.f /*insert dt here*/);
}

void Application::SecondUpdate() 
{
    Input::UpdatePrevKeyStates();
}

void Application::MainUpdate() 
{
    while (!glfwWindowShouldClose(mWindow.GetHandle())) {
        FirstUpdate();
        SystemUpdate();
        // To remove (Script test with entities)
        //ScriptTestUpdate();
        SecondUpdate(); // This should always be the last

        // Graphics update
        mWindow.Update();
    }
}

void Application::Exit() 
{
    systemManager.Exit();
    ECS::GetInstance()->DeleteAllEntities();
    SingletonManager::destroyAllSingletons();
    mWindow.DestroySystem();
}