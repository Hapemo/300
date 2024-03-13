#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"
#include "ECS/ECS.h"
#include "Guid.h"
#include "ResourceManagerTy.h"
#include "ECS/ECS_Components.h"
#include "GameState/GameStateManager.h"
#include "GameState/GameState.h"
#include "Audio/AudioSystemRevamp.h"
#include "Debug/Logger.h"
#include "Input/InputMapSystem.h"
#include "Debug/EnginePerformance.h"
#include "AI/AIManager.h"
#include "AI/PathfinderManager.h"
#include "Button/ButtonSystem.h"

SystemManager *systemManager;

SystemManager::SystemManager() : mIsQuit(false)
{
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
	mGameStateSystem = std::make_unique<GameStateManager>();
	mGraphicsSystem = std::make_unique<GraphicsSystem>();
	mResourceTySystem = std::make_unique<ResourceTy>();
	//mResourceSystem = std::make_unique<Resource>();
	mAudioSystem = std::make_unique<AudioSystem>();
	mLogger = std::make_unique<Logger>();
	mInputActionSystem = std::make_unique<InputMapSystem>();
	mAISystem = std::make_unique<AIManager>();
	mPathfinderSystem = std::make_unique<PathfinderManager>();
	mButtonSystem = std::make_unique<ButtonSystem>();
	ecs = new ECS();
}

SystemManager::~SystemManager()
{
	delete ecs;
}

void SystemManager::Init(bool isEditor, GFX::Window *window)
{
	mIsEditor = isEditor;
	mWindow = window;
	mButtonSystem.get()->Init();
	mInputActionSystem.get()->Init();
	PINFO("Init Input Action System");
	mLogger.get()->InitLogging();
	PINFO("Init Logger");
	mScriptingSystem.get()->Init();
	PINFO("Init Scripting System");

	mPathfinderSystem.get()->Init();

	//mResourceSystem.get()->Init();
	mResourceTySystem.get()->Init();
	PINFO("Init Graphics System");

	// all the resources are loaaded here
	//mGameStateSystem.get()->Init();
	mGameStateSystem.get()->Init();
	mGraphicsSystem.get()->Init();
	mGraphicsSystem->PauseGlobalAnimation();

	// set fullscreen on init
	//mGraphicsSystem.get()->SetFullscreen(true);

	mAudioSystem.get()->Init();
	PINFO("Init Game state System");
	mPhysicsSystem.get()->Init();
	PINFO("Init Physics System");
}

void SystemManager::Reset()
{
	mAudioSystem.get()->Reset();				// Using <Audio> component, must happen before clearing of entities.
	mAudioSystem.get()->scene_switched = true;
	mGraphicsSystem.get()->Unload();
	mGameStateSystem.get()->Unload();
	mGameStateSystem.get()->Init();
	mPhysicsSystem.get()->Init();
	mGraphicsSystem.get()->Init();
	//mScriptingSystem.get()->ScriptReload();
	mIsPlay = false;
	mIsInGamePause = false;
	mIsDialogue = false;
}

void SystemManager::ResetForChangeGS() {
	mGraphicsSystem.get()->Unload();
	mAudioSystem.get()->scene_switched = true;
	mPhysicsSystem.get()->Init();
	mGraphicsSystem.get()->Init();
}

void SystemManager::Pause()
{
	//mIsInGamePause = true;
	mGraphicsSystem->PauseGlobalAnimation();
	if (!mAudioSystem->sys_paused)
	{
		mAudioSystem->TogglePause();
		mAudioSystem->sys_paused = true;
	}
	auto temp = systemManager->mGameStateSystem->GetEntity("MenuBackground").GetComponent<Transform>();
	
}

void SystemManager::Play()
{
	mIsPlay = true; 
	//mIsInGamePause = false;
	mPhysicsSystem.get()->Init();
	mGraphicsSystem->UnpauseGlobalAnimation();
	mGameStateSystem->mCurrentGameState.Save();
	if (mAudioSystem->sys_paused)
	{
		mAudioSystem->TogglePause();
		mAudioSystem->sys_paused = false;
	}
}

void SystemManager::Update(float dt)
{
	EnginePerformance::StartTrack("Graphics");
	mGraphicsSystem.get()->Update(dt);
	mGameStateSystem.get()->UpdateNextGSMState();
	if (mIsEditor)
		mGraphicsSystem.get()->Draw(dt, true);
	mGraphicsSystem.get()->Draw(dt);
	if (!mIsEditor)
		mGraphicsSystem.get()->DrawGameScene();
	EnginePerformance::EndTrack("Graphics");
	EnginePerformance::UpdateSystemMs("Graphics");

	TestPathfinderManager();

	if (Input::CheckKey(E_STATE::PRESS, E_KEY::EQUAL)) {
		mGraphicsSystem->HideCursor(false);
	}

	if (Input::CheckKey(E_STATE::PRESS, E_KEY::MINUS)) {
		mGraphicsSystem->HideCursor(true);
	}

	//mGraphicsSystem->HideCursor(mIsPlay && !mIsPause);

	// To Test (Uncomment)
	//EnginePerformance::StartTrack("Audio");
	//mAudioSystem.get()->Update(dt);
	//EnginePerformance::EndTrack("Audio");

	//if (mInputActionSystem->GetButtonDown("pause") && mIsPlay)
	//	mIsPause = true;

	if (mIsInGamePause) {
		auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
		for (Entity entity : scriptEntities)
		{
			entity.GetComponent<Scripts>().RunFunctionForAllScripts("PauseUpdate");
		}
		mButtonSystem.get()->Update();
		mAudioSystem.get()->Update(dt, false);
		return;
	}

	if (mIsDialogue) {
		auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
		for (Entity entity : scriptEntities)
		{
			entity.GetComponent<Scripts>().RunFunctionForAllScripts("DialogueUpdate");
		}
		mAudioSystem.get()->Update(dt, false);
		return;
	}

	if (!mIsPlay)
		return;

	mButtonSystem.get()->Update();
	EnginePerformance::StartTrack("Physics");
	mPhysicsSystem.get()->Update(dt);
	EnginePerformance::EndTrack("Physics");
	EnginePerformance::UpdateSystemMs("Physics");

	mAISystem.get()->Update(dt);

	EnginePerformance::StartTrack("Scripting");
	mScriptingSystem.get()->Update(dt);
	EnginePerformance::EndTrack("Scripting");
	EnginePerformance::UpdateSystemMs("Scripting");

	EnginePerformance::StartTrack("Audio");
	mAudioSystem.get()->Update(dt, false);					// [10/26] Inclusion of 3D Audio -> must always be after (Positional Update) 
	EnginePerformance::EndTrack("Audio");
	EnginePerformance::UpdateSystemMs("Audio");
	//	mResourceSystem.get()->Update();
	
	ecs->DeleteEntityUpdate();
}

void SystemManager::Quit()
{
	mIsQuit = true;
}

void SystemManager::Exit()
{
	mPhysicsSystem.get()->Exit();
	mScriptingSystem.get()->Exit();
	mGraphicsSystem.get()->Exit();
	mGameStateSystem.get()->Unload();
	mResourceTySystem.get()->Exit();
	//mAudioSystem.get()->Exit();
}

void SystemManager::DeleteEntity(Entity e)
{
	ecs->DeleteEntity(e);
}

PhysicsSystem *SystemManager::GetPhysicsPointer()
{
	return mPhysicsSystem.get();
}

AudioSystem* SystemManager::GetAudioPointer()
{
	return mAudioSystem.get();
}

ScriptingSystem *SystemManager::GetScriptingPointer()
{
	return mScriptingSystem.get();
}

InputMapSystem* SystemManager::GetInputMapSystemPointer()
{
	return mInputActionSystem.get();
}

GameStateManager* SystemManager::GetGameStateSystem()
{
	return mGameStateSystem.get();
}

GraphicsSystem* SystemManager::GetGraphicsSystem()
{
	return mGraphicsSystem.get();
}

AIManager* SystemManager::GetAIManager()
{
	return mAISystem.get();
}

PathfinderManager* SystemManager::GetPathfinderManager() { return mPathfinderSystem.get(); }