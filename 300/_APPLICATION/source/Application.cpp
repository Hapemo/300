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
#include "Physics/PhysicsSystem.h"

#include "Example.h"

// Static variables
GFX::DebugRenderer* Application::mRenderer;
GFX::Window Application::mWindow;
std::string Application::title;

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
    systemManager = new SystemManager();
}

void Application::SystemInit() 
{
    systemManager->Init(false, &mWindow);
    FPSManager::Init();
    Input::Init();
    // To remove (Script test with entities)
    systemManager->mScriptingSystem->ScriptingInitTest();
    //gfx init
    // 
    // test serialization
    Entity ent1 = systemManager->ecs->NewEntity();
    Entity ent2 = systemManager->ecs->NewEntity();
    Entity ent3 = systemManager->ecs->NewEntity();

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

    auto view = systemManager->ecs->GetEntitiesWith<General, Transform>();
    int size = view.size();
    for (Entity e : view)
    {
        e.GetComponent<Transform>();
    }
    // end test serialization
}

void Application::MainUpdate() 
{
    while (!glfwWindowShouldClose(mWindow.GetHandle())) 
    {
        FirstUpdate();
        SystemUpdate();
        // To remove (Script test with entities)
        systemManager->mScriptingSystem->ScriptingUpdateTest();
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
    systemManager->Update(FPSManager::dt);
}

void Application::SecondUpdate() 
{
    Input::UpdatePrevKeyStates();
    //FPSManager::LimitFPS(0);
}

void Application::Exit() 
{
    systemManager->Exit();
    systemManager->ecs->DeleteAllEntities();
    SingletonManager::destroyAllSingletons();
    mWindow.DestroySystem();
    delete systemManager;
}