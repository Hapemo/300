#include "ECS/ECS_Systems.h"
#include "Audio/AudioSystem.h"
#include "ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"

void SystemManager::Init()
{
	PhysicsSystem::GetInstance()->Init();
	ScriptingSystem::GetInstance()->Init();
	//AudioManager::GetInstance()->Init();
}

void SystemManager::Update(float dt)
{
	PhysicsSystem::GetInstance()->Update(dt);
	ScriptingSystem::GetInstance()->Update(dt);
}

void SystemManager::Exit()
{

	ScriptingSystem::GetInstance()->Exit();
}