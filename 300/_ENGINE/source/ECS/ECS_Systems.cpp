#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"
#include "Audio/AudioSystem2.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "GameState/GameStateManager.h"

SystemManager* systemManager;

SystemManager::SystemManager()
{
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
	mAudioSystem = std::make_unique<AudioSystem>();
	mGraphicsSystem = std::make_unique<GraphicsSystem>();
	mGameStateSystem = std::make_unique<GameStateManager>();
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
	mPhysicsSystem.get()->Init();
	mScriptingSystem.get()->Init();
	mAudioSystem.get()->Init();
	mGraphicsSystem.get()->Init();
	mGameStateSystem.get()->Init();
}

void SystemManager::Update(float dt)
{
	mPhysicsSystem.get()->Update(dt);
	mScriptingSystem.get()->Update(dt);
	mGraphicsSystem.get()->Update(dt);
	mAudioSystem.get()->Update(dt);
}

void SystemManager::Exit()
{
	mPhysicsSystem.get()->Exit();
	mScriptingSystem.get()->Exit();
	mGraphicsSystem.get()->Exit();
	mAudioSystem.get()->Exit();
	mGameStateSystem.get()->Unload();
}

PhysicsSystem* SystemManager::GetPhysicsPointer()
{
	return mPhysicsSystem.get();
}

ScriptingSystem* SystemManager::GetScriptingPointer() {
	return mScriptingSystem.get();
}
