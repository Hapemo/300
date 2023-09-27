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
#include "ECS/ECS_Systems.h"
#include "Input/Input.h"
#include "SingletonManager.h"
#include "ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
#include "ResourceManager.h"
#include "ResourceManagerTy.h"
#include "Graphics/GraphicsSystem.h"
#include "SceneWindow.h"
#include "Guid.h"
#include "Physics/Accumulator.h"
#include "ConfigManager.h"
#include "GameState/GameStateManager.h"

// Static variables
GFX::DebugRenderer* EditorApplication::mRenderer;
GFX::Window EditorApplication::mWindow;
std::string EditorApplication::title;
Editor EditorApplication::mMaineditor;

void EditorApplication::Init()
{
    StartUp();
    SystemInit();

    // create new entity
    /*Entity mEntity = systemManager->ecs->NewEntity();
    mEntity.GetComponent<General>().name = "DefaultTestObj";
    mEntity.GetComponent<MeshRenderer>().mMeshPath = "../compiled_geom/Skull_textured.geom";*/
}

void EditorApplication::StartUp()
{
    //gfx glew and glfw startup
    GFX::Window::InitializeSystem();
    mWindow = GFX::Window({ 1920, 1080 });
    mWindow.SetWindowTitle("Editor");
    ConfigManager::Init("../assets/config.txt");
    systemManager = new SystemManager();


}

void EditorApplication::SystemInit()
{
    systemManager->Init(true, &mWindow);
    FPSManager::Init();
    Input::Init();
    //gfx init

    mMaineditor.UIinit(mWindow.GetHandle());
    //Editor init

}

void EditorApplication::MainUpdate()
{



    while (!glfwWindowShouldClose(mWindow.GetHandle())) 
    {
        FirstUpdate();
        SystemUpdate();

        // To remove (Script test with entities)
        //ScriptTestUpdate();

        // Graphics update

        auto windowSize = static_cast<SceneWindow*>(mMaineditor.mWindowlist["Editscene"])->winSize;
            
        systemManager->mGraphicsSystem->SetCameraSize(CAMERA_TYPE::CAMERA_TYPE_EDITOR, windowSize);
        mMaineditor.UIupdate(mWindow.GetHandle());

        mMaineditor.UIdraw(mWindow.GetHandle());



        SecondUpdate(); // This should always be the last

        mWindow.Update();   // This is required for IMGUI draws as well

        
    }
}

void EditorApplication::FirstUpdate()
{
    FPSManager::Update();
    Accumulator::Update(FPSManager::dt);
    mWindow.PollEvents();
}

void EditorApplication::SystemUpdate()
{
    systemManager->Update(FPSManager::dt);
}

void EditorApplication::SecondUpdate()
{
    Input::UpdatePrevKeyStates();
    FPSManager::LimitFPS(60);
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