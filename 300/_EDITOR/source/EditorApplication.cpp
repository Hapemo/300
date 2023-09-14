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
#include "EditorApplication.h"
#include "FPSManager.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Input.h"
#include "SingletonManager.h"
#include "ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"

// Static variables
GFX::DebugRenderer* EditorApplication::mRenderer;
GFX::Window EditorApplication::mWindow;
std::string EditorApplication::title;
Editor EditorApplication::mMaineditor;

void EditorApplication::Init()
{
    StartUp();
    SystemInit();

}

void EditorApplication::StartUp()
{
    //gfx glew and glfw startup
    GFX::Window::InitializeSystem();
    mWindow = GFX::Window({ 1920, 1080 });
    mWindow.SetWindowTitle("Editor");
    systemManager = new SystemManager();



}

void EditorApplication::SystemInit()
{
    systemManager->Init(true, &mWindow);
    FPSManager::Init();
    Input::Init();
    //gfx init

    //Editor init

}

void EditorApplication::MainUpdate()
{

    mMaineditor.UIinit(mWindow.GetHandle());


    while (!glfwWindowShouldClose(mWindow.GetHandle())) {
        FirstUpdate();
        SystemUpdate();

        // To remove (Script test with entities)
        //ScriptTestUpdate();

        // Graphics update

        mMaineditor.UIupdate(mWindow.GetHandle());
        //mMaineditor.WindowUpdate(mWindow.GetHandle());
        mMaineditor.UIdraw(mWindow.GetHandle());

        // To remove (Script test with entities)
        systemManager->mScriptingSystem->ScriptingUpdateTest();

        SecondUpdate(); // This should always be the last

        mWindow.Update();

    }
}

void EditorApplication::FirstUpdate()
{
    FPSManager::Update();
    mWindow.PollEvents();
}

void EditorApplication::SystemUpdate()
{
    systemManager->Update(1.f /*insert dt here*/);
}

void EditorApplication::SecondUpdate()
{
    Input::UpdatePrevKeyStates();
}

void EditorApplication::Exit()
{
    mMaineditor.UIend();
    systemManager->Exit();
    systemManager->ecs->DeleteAllEntities();
    SingletonManager::destroyAllSingletons();
    mWindow.DestroySystem();
    delete systemManager;
}