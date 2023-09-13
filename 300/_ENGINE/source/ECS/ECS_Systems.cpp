#include "Physics/PhysicsSystem.h"
#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"
#include "Audio/AudioSystem.h"

SystemManager* systemManager;

SystemManager::SystemManager()
{
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
	mAudioSystem = std::make_unique<AudioManager>();
}

SystemManager::~SystemManager()
{

}

void SystemManager::Init(GFX::Window* window)
{
	mWindow = window;
	mPhysicsSystem.get()->Init();
	mScriptingSystem.get()->Init();
	mAudioSystem.get()->Init();
}

void SystemManager::Update(float dt)
{
	mPhysicsSystem.get()->Update(dt);
	mScriptingSystem.get()->Update(dt);
	mAudioSystem.get()->Update(dt);
}

void SystemManager::Exit()
{
	mPhysicsSystem.get()->Exit();
	mScriptingSystem.get()->Exit();
	mAudioSystem.get()->Exit();
}
