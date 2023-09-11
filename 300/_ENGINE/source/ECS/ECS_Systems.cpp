#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"

SystemManager* systemManager;

SystemManager::SystemManager()
{
	scriptingSystem = std::make_unique<ScriptingSystem>();
}

void SystemManager::Init()
{
	scriptingSystem.get()->Init();
}

void SystemManager::Update(float dt)
{
	scriptingSystem.get()->Update(dt);

}

void SystemManager::Exit()
{
	scriptingSystem.get()->Exit();

}