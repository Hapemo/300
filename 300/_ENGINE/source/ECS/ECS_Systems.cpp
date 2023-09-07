#include "ECS/ECS_Systems.h"
#include "Audio/AudioSystem.h"
void SystemManager::Init()
{
	ScriptingSystem::GetInstance()->Init();
	AudioManager::GetInstance()->Init();
}

void SystemManager::Update(float dt)
{
	ScriptingSystem::GetInstance()->Update(dt);
	AudioManager::GetInstance()->Update();
}

void SystemManager::Exit()
{
	ScriptingSystem::GetInstance()->Exit();
}