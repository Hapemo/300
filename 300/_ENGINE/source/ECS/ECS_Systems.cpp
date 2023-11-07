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

SystemManager *systemManager;

SystemManager::SystemManager()
{
	mIsPlay = false;
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
	mInputActionSystem.get()->Init();
	PINFO("Init Input Action System");
	mLogger.get()->InitLogging();
	PINFO("Init Logger");
	mScriptingSystem.get()->Init();
	PINFO("Init Scripting System");


	//mResourceSystem.get()->Init();
	mResourceTySystem.get()->Init();
	PINFO("Init Graphics System");

	// all the resources are loaaded here
	//mGameStateSystem.get()->Init();
	mGameStateSystem.get()->Init();
	mGraphicsSystem.get()->Init();
	mGraphicsSystem->PauseGlobalAnimation();


	mAudioSystem.get()->Init();
	PINFO("Init Game state System");
	mPhysicsSystem.get()->Init();
	PINFO("Init Physics System");
}

void SystemManager::Reset()
{
	mAudioSystem.get()->Reset();				// Using <Audio> component, must happen before clearing of entities.
	mGraphicsSystem.get()->Unload();
	mGameStateSystem.get()->Unload();
	mGameStateSystem.get()->Init();
	mPhysicsSystem.get()->Init();
	mGraphicsSystem.get()->Init();
	//mScriptingSystem.get()->ScriptReload();
	mIsPlay = false;
}

void SystemManager::ResetForChangeGS() {
	mGraphicsSystem.get()->Unload();
	mPhysicsSystem.get()->Init();
	mGraphicsSystem.get()->Init();
}

void SystemManager::Pause()
{
	mIsPlay = false; 
	mGraphicsSystem->PauseGlobalAnimation();
	mAudioSystem->Pause(); 
}

void SystemManager::Play()
{
	mIsPlay = true; 
	mPhysicsSystem.get()->Init();
	mGraphicsSystem->UnpauseGlobalAnimation();
	mGameStateSystem->mCurrentGameState.Save();
	//mAudioSystem.get()->PlayOnAwake();
	//mAudioSystem.get()->system_paused = false;
}

void SystemManager::Update(float dt)
{
	EnginePerformance::StartTrack("Graphics");
	mGraphicsSystem.get()->Update(dt);
	mGameStateSystem.get()->UpdateNextGSMState();
	if (mIsEditor)
		mGraphicsSystem.get()->EditorDraw(dt);
	mGraphicsSystem.get()->GameDraw(dt);
	if (!mIsEditor)
		mGraphicsSystem.get()->DrawGameScene();
	EnginePerformance::EndTrack("Graphics");
	EnginePerformance::UpdateSystemMs("Graphics");

	if (!mIsPlay) return;

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
	mAudioSystem.get()->Update(dt);					// [10/26] Inclusion of 3D Audio -> must always be after (Positional Update) 
	EnginePerformance::EndTrack("Audio");
	EnginePerformance::UpdateSystemMs("Audio");
	//	mResourceSystem.get()->Update();
	
	ecs->DeleteEntityUpdate();
}

void SystemManager::Exit()
{
	mPhysicsSystem.get()->Exit();
	mScriptingSystem.get()->Exit();
	mGraphicsSystem.get()->Exit();
	mResourceTySystem.get()->Exit();
	mGameStateSystem.get()->Unload();
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
