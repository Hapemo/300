#include "ECS/ECS_Systems.h"

void SystemManager::Init()
{
	//mPhysicsSystem.Init();
	mScriptingSystem.Init();
}

void SystemManager::Update(float dt)
{
	//mPhysicsSystem.Update();
	mScriptingSystem.Update();
}

void SystemManager::Exit()
{
	//mPhysicsSystem.Exit();
	mScriptingSystem.Exit();
}