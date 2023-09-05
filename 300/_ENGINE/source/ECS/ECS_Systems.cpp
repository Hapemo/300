#include "ECS/ECS_Systems.h"

void SystemManager::Init()
{
	//mPhysicsSystem.Init();
	ScriptingSystem::GetInstance()->Init();
}

void SystemManager::Update(float dt)
{
	//mPhysicsSystem.Update();
	ScriptingSystem::GetInstance()->Update(dt);
}

void SystemManager::Exit()
{
	//mPhysicsSystem.Exit();
	ScriptingSystem::GetInstance()->Exit();
}