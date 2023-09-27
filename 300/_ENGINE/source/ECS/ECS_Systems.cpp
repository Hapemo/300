#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"
#include "ECS/ECS.h"
#include "Guid.h"
#include "ResourceManager.h"
#include "ResourceManagerTy.h"
#include "ECS/ECS_Components.h"
#include "GameState/GameStateManager.h"
#include "Audio/AudioSystem.h"
#include "Debug/Logger.h"
#include "../../../_TOOLS/include/Input/InputMapSystem.h"

SystemManager* systemManager;

SystemManager::SystemManager()
{
	mIsPlay = false;
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
	mGameStateSystem = std::make_unique<GameStateManager>();
	mGraphicsSystem = std::make_unique<GraphicsSystem>();
	mResourceTySystem = std::make_unique<ResourceTy>();
	mResourceSystem	= std::make_unique<Resource>();
	mAudioSystem = std::make_unique<AudioSystem>();
	mLogger = std::make_unique<Logger>();
	mInputActionSystem = std::make_unique<InputMapSystem>();
	ecs = new ECS();
}

SystemManager::~SystemManager()
{
	delete ecs;
}

void SystemManager::Init(bool isEditor, GFX::Window* window)
{
	mIsEditor = isEditor;
	mWindow = window;
	mInputActionSystem.get()->Init();
	PINFO("Init Input Action System");
	mLogger.get()->InitLogging();
	PINFO("Init Logger");
	mScriptingSystem.get()->Init();
	PINFO("Init Scripting System");

	mGameStateSystem.get()->Init();

	mResourceSystem.get()->Init();
	mResourceTySystem.get()->Init();
	PINFO("Init Graphics System");

	// all the resources are loaaded here
	//mGameStateSystem.get()->Init();
	mGraphicsSystem.get()->Init();

	mAudioSystem.get()->Init();
	PINFO("Init Game state System");
	mPhysicsSystem.get()->Init();
	PINFO("Init Physics System");
}

void SystemManager::Reset()
{
	mGameStateSystem.get()->Unload();
	mGameStateSystem.get()->Init();
	mPhysicsSystem.get()->Init();
	mIsPlay = false;
}

void SystemManager::Pause()
{
	mIsPlay = false; 
	mGraphicsSystem->PauseGlobalAnimation();
}

void SystemManager::Play()
{
	mIsPlay = true; 
	mGraphicsSystem->UnpauseGlobalAnimation();
}

void SystemManager::Update(float dt)
{
	mGraphicsSystem.get()->Update(dt);
	mGameStateSystem.get()->UpdateNextGSMState();
	mGraphicsSystem.get()->EditorDraw(dt);
	mGraphicsSystem.get()->GameDraw(dt);
	if (!mIsPlay) return;

	mInputActionSystem.get()->Update();
	mPhysicsSystem.get()->Update(dt);
	mScriptingSystem.get()->Update(dt);
	mAudioSystem.get()->Update(dt);

//	mResourceSystem.get()->Update();
}

void SystemManager::Exit()
{
	mPhysicsSystem.get()->Exit();
	mScriptingSystem.get()->Exit();
	mGraphicsSystem.get()->Exit();
	mResourceTySystem.get()->Exit();
	mGameStateSystem.get()->Unload();
	mAudioSystem.get()->Exit();

}

PhysicsSystem* SystemManager::GetPhysicsPointer()
{
	return mPhysicsSystem.get();
}

ScriptingSystem* SystemManager::GetScriptingPointer() {
	return mScriptingSystem.get();
}

