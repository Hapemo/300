#include "ECS/ECS_Systems.h"

void SystemManager::Init()
{
	//mPhysicsSystem.Init();
	ScriptingSystem::Init();
}

void SystemManager::Update(float dt)
{
	//mPhysicsSystem.Update();
	ScriptingSystem::Update();
}

void SystemManager::Exit()
{
	//mPhysicsSystem.Exit();
	ScriptingSystem::Exit();
}