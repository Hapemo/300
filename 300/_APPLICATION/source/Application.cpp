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



#include "Application.h"
#include "FPSManager.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
//#include "Input/Input.h"
#include "SingletonManager.h"
#include "Object/ObjectFactory.h"
#include "ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
//#include "Serialization/SerializationTemp.h"
#include "GameState/GameStateManager.h"
#include "Debug/Logger.h"
#include "ConfigManager.h"
#include "Example.h"
#include "Physics/Accumulator.h"

#include <Windows.h>
#include <WinUser.h>
#include <Graphics/GraphicsSystem.h>

// Static variables
GFX::DebugRenderer* Application::mRenderer;
GFX::Window Application::mWindow;
std::string Application::title;
//uint32_t eee;

void Application::Init() 
{

    //SetCursorPos(1, 1);

    StartUp();
    SystemInit();
    //MultithreadExample();

    systemManager->Play();
}

void Application::StartUp() 
{
    //gfx glew and glfw startup
    GFX::Window::InitializeSystem();
    mWindow = GFX::Window({ 1920, 1080 });
    mWindow.SetWindowTitle("Application");
    ConfigManager::Init("../assets/config.txt");
    systemManager = new SystemManager();
}

void Application::SystemInit()
{
    Input::mIsEditor = false;
    //RegisterAllComponents();

  /*  systemManager->Init(false, &mWindow);*/ // Moved this after Audio Component Test

    systemManager->Init(false, &mWindow);
    FPSManager::Init();
    Input::Init(false);

    systemManager->mGraphicsSystem->SetFullscreen(true);




}

void Application::MainUpdate() 
{
    while (!systemManager->mGameStateSystem->Exited())
    {
        std::string title{ "TroubleShooter | FPS: " };
        title += std::to_string((int)FPSManager::fps);
        systemManager->GetWindow()->SetWindowTitle(title.c_str());
        FirstUpdate();
        SystemUpdate();
        SecondUpdate(); // This should always be the last
        

        // Graphics update
        mWindow.Update();

        if (glfwWindowShouldClose(mWindow.GetHandle())) 
          systemManager->mGameStateSystem->Exit();

        if (systemManager->IsQuit())
            break;
    }
}

void Application::FirstUpdate() 
{
    FPSManager::Update();
    Accumulator::Update(static_cast<float>(FPSManager::dt));
    mWindow.PollEvents();
}

void Application::SystemUpdate() 
{
    //if (Input::CheckKey(E_STATE::HOLD, E_KEY::A))
    //    systemManager->mPhysicsSystem->SetVelocity(Entity(eee), glm::vec3(50.f, 0, 0));
    //if (Input::CheckKey(E_STATE::HOLD, E_KEY::D))
    //    systemManager->mPhysicsSystem->SetVelocity(Entity(eee), glm::vec3(-50.f, 0, 0));
    //if (Input::CheckKey(E_STATE::HOLD, E_KEY::W))
    //    systemManager->mPhysicsSystem->SetVelocity(Entity(eee), glm::vec3(0.f, 0, 50));
    //if (Input::CheckKey(E_STATE::HOLD, E_KEY::S))
    //    systemManager->mPhysicsSystem->SetVelocity(Entity(eee), glm::vec3(0.f, 0, -50));
    systemManager->Update(static_cast<float>(FPSManager::dt));
}

void Application::SecondUpdate() 
{
    Input::UpdatePrevKeyStates();
    FPSManager::LimitFPS(60);
}

void Application::Exit() 
{
    systemManager->Exit();
    systemManager->ecs->DeleteAllEntities();
    SingletonManager::destroyAllSingletons();
    systemManager->mLogger->EndLogging();
    mWindow.DestroySystem();
    delete systemManager;
}