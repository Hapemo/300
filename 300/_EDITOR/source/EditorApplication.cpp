/*!*****************************************************************************
\file main.cpp
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Pepe Production
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
#include "ResourceManagerTy.h"
#include "Graphics/GraphicsSystem.h"
#include "SceneWindow.h"
#include "Physics/Accumulator.h"
#include "ConfigManager.h"
#include "GameState/GameStateManager.h"
#include "Debug/EnginePerformance.h"
#include "Audio/AudioSystem.h"

// Static variables
GFX::DebugRenderer *EditorApplication::mRenderer;
GFX::Window EditorApplication::mWindow;
std::string EditorApplication::title;
Editor EditorApplication::mMaineditor;

void EditorApplication::Init()
{
    StartUp();
    SystemInit();

    Entity trigger = systemManager->ecs->NewEntity();
    RigidBody& rbod = trigger.AddComponent<RigidBody>();
    rbod.mMotion = MOTION::DYNAMIC;
    BoxCollider& box = trigger.AddComponent<BoxCollider>();
    box.mIsTrigger = false;
    trigger.GetComponent<Transform>().mScale = glm::vec3(60, 40, 60);
    trigger.GetComponent<Transform>().mRotate = glm::vec3(0);
    trigger.GetComponent<Transform>().mTranslate = glm::vec3(100, 200, 100);

    //BoxCollider& col = trigger.AddComponent<BoxCollider>();
    //col.mIsTrigger = true;


    //Entity testTrigger = systemManager->ecs->NewEntity();
    //rbod = testTrigger.AddComponent<RigidBody>();
    //rbod.mMotion = MOTION::DYNAMIC;
    //box = testTrigger.AddComponent<CapsuleCollider>();
    //box.mIsTrigger = false;
    //box.mHalfHeight = 10.f;
    //box.mRadius = 20.f;
    //testTrigger.GetComponent<Transform>().mScale = glm::vec3(60, 40, 60);
    //testTrigger.GetComponent<Transform>().mRotate = glm::vec3(0);
    //testTrigger.GetComponent<Transform>().mTranslate = glm::vec3(70, 200, 70);
    //SphereCollider& sphere = trigger.AddComponent<SphereCollider>();
    //sphere.mIsTrigger = false;
    //sphere.mScaleOffset = 0.5f;
};
    // create new entity
    /*Entity mEntity = systemManager->ecs->NewEntity();
    mEntity.GetComponent<General>().name = "DefaultTestObj";
    mEntity.GetComponent<MeshRenderer>().mMeshPath = "../compiled_geom/Skull_textured.geom";*/
void EditorApplication::StartUp()
{
    // gfx glew and glfw startup
    GFX::Window::InitializeSystem();
    mWindow = GFX::Window({1920, 1080});
    mWindow.SetWindowTitle("Editor");
    ConfigManager::Init("../assets/config.txt");
    systemManager = new SystemManager();
}

void EditorApplication::SystemInit()
{
    systemManager->Init(true, &mWindow);
    FPSManager::Init();
    Input::Init();
    // gfx init

    mMaineditor.UIinit(mWindow.GetHandle());
    // Editor init
}

void EditorApplication::MainUpdate()
{
    while (!glfwWindowShouldClose(mWindow.GetHandle()))
    {
        EnginePerformance::StartTrack("Editor");
        FirstUpdate();
        SystemUpdate();

        // To remove (Script test with entities)
        // ScriptTestUpdate();

        // Graphics update

        auto editorwindowsize = static_cast<SceneWindow *>(mMaineditor.mWindowlist["Editscene"])->winSize;
        systemManager->mGraphicsSystem->SetCameraSize(CAMERA_TYPE::CAMERA_TYPE_EDITOR, editorwindowsize);
        auto gamewindowsize = static_cast<SceneWindow *>(mMaineditor.mWindowlist["GameScene"])->winSize;
        systemManager->mGraphicsSystem->SetCameraSize(CAMERA_TYPE::CAMERA_TYPE_GAME, gamewindowsize);
        EnginePerformance::EndTrack("Editor");
        EnginePerformance::TotalTime("Editor");
        mMaineditor.UIupdate(mWindow.GetHandle());
        // mMaineditor.WindowUpdate(mWindow.GetHandle());
        EnginePerformance::StartTrack("Editor");
        mMaineditor.UIdraw(mWindow.GetHandle());

        SecondUpdate(); // This should always be the last

        mWindow.Update(); // This is required for IMGUI draws as well

        EnginePerformance::EndTrack("Editor");
        EnginePerformance::StoreTime("Editor");
    }
}

void EditorApplication::FirstUpdate()
{
    FPSManager::Update(0.01f);
    Accumulator::Update(static_cast<float>(FPSManager::dt));
    mWindow.PollEvents(); // win manafer
}

void EditorApplication::SystemUpdate()
{
    systemManager->Update(static_cast<float>(FPSManager::dt));
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