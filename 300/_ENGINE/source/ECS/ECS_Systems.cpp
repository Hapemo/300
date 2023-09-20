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

SystemManager* systemManager;

SystemManager::SystemManager()
{
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
	mGraphicsSystem = std::make_unique<GraphicsSystem>();
	mGameStateSystem = std::make_unique<GameStateManager>();
	mResourceTySystem = std::make_unique<ResourceTy>();
	mResourceSystem		= std::make_unique<Resource>();
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
	mGraphicsSystem.get()->Init();
	mResourceSystem.get()->Init();			// all the resources are loaaded here
	mGameStateSystem.get()->Init();
}

void SystemManager::Update(float dt)
{
	mPhysicsSystem.get()->Update(dt);
	mScriptingSystem.get()->Update(dt);
	mGraphicsSystem.get()->Update(dt);
//	mResourceSystem.get()->Update();
}

void SystemManager::Exit()
{
	mPhysicsSystem.get()->Exit();
	mScriptingSystem.get()->Exit();
	mGraphicsSystem.get()->Exit();
//	mResourceSystem.get()->Exit();
	mGameStateSystem.get()->Unload();
}

PhysicsSystem* SystemManager::GetPhysicsPointer()
{
	return mPhysicsSystem.get();
}

ScriptingSystem* SystemManager::GetScriptingPointer() {
	return mScriptingSystem.get();
}
