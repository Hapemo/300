#include "Physics/PhysicsSystem.h"
#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"

SystemManager* systemManager;

SystemManager::SystemManager()
{
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
}

SystemManager::~SystemManager()
{

}

void SystemManager::Init(bool isEditor, GFX::Window* window)
{
	mIsEditor = isEditor;
	mWindow = window;
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
