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
#include "Input/Input.h"
#include "SingletonManager.h"
#include "Object/ObjectFactory.h"
#include "ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
//#include "Serialization/SerializationTemp.h"
#include "GameState/GameStateManager.h"
#include "Debug/Logger.h"
#include "ConfigManager.h"

#include "Example.h"
#include "Input/Input.h"
#include "Physics/Accumulator.h"

// Static variables
GFX::DebugRenderer* Application::mRenderer;
GFX::Window Application::mWindow;
std::string Application::title;
//uint32_t eee;

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
    ConfigManager::Init("../assets/config.txt");
    systemManager = new SystemManager();
}

void Application::SystemInit()
{

#pragma region AudioComponent Test
    Entity entAudio = systemManager->ecs->NewEntity();
    entAudio.AddComponent<Audio>({ "../assets/Audio" , "farm_ambience.wav", AUDIO_BGM, false});
    Audio& audio_component = entAudio.GetComponent<Audio>();

    // Test Edit <Audio> Component
    //entAudio.GetComponent<Audio>().mFileName = "farm_ambience.wav";
    /*entAudio.GetComponent<Audio>().mIsPlay = false;
    entAudio.GetComponent<Audio>().mIsPlaying = false;
    entAudio.GetComponent<Audio>().mIsPlay = false;
    entAudio.GetComponent<Audio>().mIsPlaying = false;*/

    Entity radioAudio = systemManager->ecs->NewEntity();
    radioAudio.AddComponent<Audio>({ "../assets/Audio" , "tuning-radio-7150.wav" , AUDIO_SFX, false });

    Entity npcAudio = systemManager->ecs->NewEntity();
    npcAudio.AddComponent<Audio>({ "../assets/Audio" , "NPC_Greeting.wav" , AUDIO_SFX, true });

    Entity non_existent_audio = systemManager->ecs->NewEntity();
    non_existent_audio.AddComponent<Audio>();

#pragma endregion

#pragma region testphysics
    //Entity e1 = systemManager->ecs->NewEntity();
    //Entity e2 = systemManager->ecs->NewEntity();
    //Entity e3 = systemManager->ecs->NewEntity();

    //e1.AddComponent<RigidBody>();
    //e1.GetComponent<RigidBody>().mDensity = 10.f;
    //e1.GetComponent<RigidBody>().mMaterial = MATERIAL::CONCRETE;
    //e1.GetComponent<RigidBody>().mMotion = MOTION::STATIC;
    //e1.AddComponent<PlaneCollider>();

    //e2.AddComponent<RigidBody>();
    //e2.GetComponent<RigidBody>().mDensity = 10.f;
    //e2.GetComponent<RigidBody>().mMaterial = MATERIAL::CONCRETE;
    //e2.GetComponent<RigidBody>().mMotion = MOTION::DYNAMIC;
    //e2.AddComponent<SphereCollider>();
    //e2.GetComponent<Transform>().mTranslate.y = 400.f;

    //e3.AddComponent<RigidBody>();
    //e3.GetComponent<RigidBody>().mDensity = 10.f;
    //e3.GetComponent<RigidBody>().mMaterial = MATERIAL::CONCRETE;
    //e3.GetComponent<RigidBody>().mMotion = MOTION::STATIC;
    //e3.AddComponent<SphereCollider>();
    //e3.GetComponent<Transform>().mTranslate = glm::vec3(300.f, 105, 300.f);
    //eee = (uint32_t)e2.id;

#pragma endregion testphysics
  /*  systemManager->Init(false, &mWindow);*/ // Moved this after Audio Component Test

    systemManager->Init(false, &mWindow);
    FPSManager::Init();
    Input::Init();
#pragma region testserialization
    //ObjectFactory::DeserializeScene("../assets/Scenes/test.json");

    //Entity ent1 = systemManager->ecs->NewEntity();
    //Entity ent2 = systemManager->ecs->NewEntity();
    //Entity ent3 = systemManager->ecs->NewEntity();

    //ent1.GetComponent<General>().name = "Testing";
    //ent1.GetComponent<General>().isActive = true;
    //ent1.GetComponent<General>().tag = TAG::PLAYER;
    //ent1.GetComponent<General>().subtag = SUBTAG::ACTIVE;
    //ent1.AddComponent<Scripts>();
    //Script test, test2;
    //test.scriptFile = "../assets/Scripts/Run.lua";
    //test.env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };
    //test2.scriptFile = "../assets/Scripts/Fly.lua";
    //test2.env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };
    //ent1.GetComponent<Scripts>().scriptsContainer.push_back(test);
    //ent1.GetComponent<Scripts>().scriptsContainer.push_back(test2);

    //ent2.AddComponent<Audio>();

    //ent2.GetComponent<General>().name = "Other";
    //ent2.GetComponent<General>().isActive = true;
    //ent2.GetComponent<General>().tag = TAG::UNKNOWN;
    //ent2.GetComponent<General>().subtag = SUBTAG::BACKGROUND;

    //ent3.GetComponent<General>().name = "Apple";
    //ent3.GetComponent<General>().isActive = false;
    //ent3.GetComponent<General>().tag = TAG::UNKNOWN;
    //ent3.GetComponent<General>().subtag = SUBTAG::ACTIVE;
    //


    //ObjectFactory::SerializeScene("../assets/Scenes/test.json");

    //systemManager->mGameStateSystem->NewGameState("MAMAMA");
    //systemManager->mGameStateSystem->GetCurrentGameState()->AddScene("test");
    //systemManager->mGameStateSystem->GetCurrentGameState()->Save();

    //auto view = systemManager->ecs->GetEntitiesWith<General, Transform>();
    //int size = view.size();
    //for (Entity e : view)
    //{
    //    e.GetComponent<Transform>();
    //}
#pragma endregion testserialization
#pragma region testparentchild

    //loop over children of an entity
    //Entity ent4 = systemManager->ecs->NewEntity();
    //Entity ent5 = systemManager->ecs->NewEntity();
    //Entity ent6 = systemManager->ecs->NewEntity();
    //Entity ent7 = systemManager->ecs->NewEntity();

    //ent4.AddChild(ent5);
    //int s1 = ent4.GetComponent<Children>().mNumChildren;

    //ent4.AddChild(ent6);
    //ent4.AddChild(ent7);
    //s1 = ent4.GetComponent<Children>().mNumChildren;


    //auto viewtemp1 = systemManager->ecs->GetEntitiesWith<Parent>();
    //int size1 = viewtemp1.size();
    //auto viewtemp2 = systemManager->ecs->GetEntitiesWith<Parent, Children>();
    //bool e4 = ent4.HasAllOfComponents<Parent, Children>();
    //bool e5 = ent5.HasAllOfComponents<Parent, Children>();
    //bool e6 = ent6.HasAllOfComponents<Parent, Children>();
    //std::vector<Entity> children = ent5.GetAllChildren();
    //Entity ent5parent = ent5.GetParent();
    //Entity ent6parent = ent6.GetParent();
    //ent4.RemoveChild(ent7);
    //auto v1 = systemManager->ecs->GetEntitiesWith<Parent>();
    //assert(ent5.GetComponent<Parent>().mNextSibling == ent5.GetComponent<Parent>().mPrevSibling);
    //assert(ent6.GetComponent<Parent>().mNextSibling == ent6.GetComponent<Parent>().mPrevSibling);

    //s1 = ent4.GetComponent<Children>().mNumChildren;
    //assert( == 2);

#pragma endregion testparentchild
#pragma region testprefab
    //Entity ent00 = systemManager->ecs->NewEntity();
    //ent00.GetComponent<General>().name = "testPrefab";
    //ent00.AddComponent<RigidBody>();
    //ent00.AddComponent<PlaneCollider>();
    //systemManager->ecs->NewPrefab(ent00);

    //
    //Entity ent01 = systemManager->ecs->NewEntityFromPrefab("testPrefab");
    //systemManager->ecs->UnlinkPrefab(ent01);
    //ent01.RemoveComponent<RigidBody>();
    //ent01.RemoveComponent<PlaneCollider>();
    //systemManager->ecs->NewPrefab(ent01);

#pragma endregion testprefab





}

void Application::MainUpdate() 
{
    while (!systemManager->mGameStateSystem->Exited())
    {
        FirstUpdate();
        SystemUpdate();
        // To remove (Script test with entities)
        systemManager->mScriptingSystem->ScriptingUpdateTest();
        // To remove after ScriptStart and ScriptUpdate are functions are working
        systemManager->mScriptingSystem->TestSSSU();
        SecondUpdate(); // This should always be the last

        // Graphics update
        mWindow.Update();

        if (glfwWindowShouldClose(mWindow.GetHandle())) 
          systemManager->mGameStateSystem->Exit();
    }
}

void Application::FirstUpdate() 
{
    FPSManager::Update();
    Accumulator::Update(FPSManager::dt);
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
    systemManager->Update(FPSManager::dt);
}

void Application::SecondUpdate() 
{
    Input::UpdatePrevKeyStates();
    FPSManager::LimitFPS(0);
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