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
#include "Object/ObjectFactory.h"
#include "ScriptingSystem.h"
#include "Example.h"

// Static variables
GFX::DebugRenderer* Application::mRenderer;
GFX::Window Application::mWindow;
std::string Application::title;
SystemManager Application::systemManager;

void Application::Init() 
{
    StartUp();
    SystemInit();
    //MultithreadExample();
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
    // 
    // test serialization
    Entity ent1 = ECS::GetInstance()->NewEntity();
    Entity ent2 = ECS::GetInstance()->NewEntity();
    Entity ent3 = ECS::GetInstance()->NewEntity();

    ent1.GetComponent<General>().name = "Testing";
    ent1.GetComponent<General>().isActive = true;
    ent1.GetComponent<General>().tag = TAG::PLAYER;
    ent1.GetComponent<General>().subtag = SUBTAG::ACTIVE;

    ent2.GetComponent<General>().name = "Other";
    ent2.GetComponent<General>().isActive = true;
    ent2.GetComponent<General>().tag = TAG::UNKNOWN;
    ent2.GetComponent<General>().subtag = SUBTAG::BACKGROUND;

    ent3.GetComponent<General>().name = "Apple";
    ent3.GetComponent<General>().isActive = false;
    ent3.GetComponent<General>().tag = TAG::UNKNOWN;
    ent3.GetComponent<General>().subtag = SUBTAG::ACTIVE;

    ObjectFactory::SerializeScene("../resources/Scenes/test.json");
    // end test serialization
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

void Application::FirstUpdate() 
{
    FPSManager::Update();
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

void Application::Exit() 
{
    systemManager.Exit();
    ECS::GetInstance()->DeleteAllEntities();
    SingletonManager::destroyAllSingletons();
    mWindow.DestroySystem();
}