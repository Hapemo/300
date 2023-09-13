#include "Physics/PhysicsSystem.h"
#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"
#include "ECS/ECS.h"

SystemManager* systemManager;

SystemManager::SystemManager()
{
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
	ecs = new ECS;
}

SystemManager::~SystemManager()
{
	delete ecs;
}

void SystemManager::Init()
{
	mPhysicsSystem.get()->Init();
	mScriptingSystem.get()->Init();
}

void SystemManager::Update(float dt)
{
	mPhysicsSystem.get()->Update(dt);
	mScriptingSystem.get()->Update(dt);
}

void SystemManager::Exit()
{
	mPhysicsSystem.get()->Exit();
	mScriptingSystem.get()->Exit();
}

PhysicsSystem* SystemManager::GetPhysicsPointer()
{
	return mPhysicsSystem.get();
}
