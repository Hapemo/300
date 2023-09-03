#include "ECS/ECS_Systems.h"

void SystemManager::Init()
{
	mPhysicsSystem.Init();
}

void SystemManager::Update(float dt)
{
	mPhysicsSystem.Update(dt);
}

void SystemManager::Exit()
{
	mPhysicsSystem.Exit();
}