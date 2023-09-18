#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"
#include "Audio/AudioSystem2.h"
#include "ECS/ECS.h"
#include "Guid.h"
#include "ResourceManager.h"

SystemManager* systemManager;

SystemManager::SystemManager()
{
	
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
	mGraphicsSystem = std::make_unique<GraphicsSystem>();
	mAudioSystem = std::make_unique<AudioSystem>();
	mResourceSystem = std::make_unique<Resource>();
	ecs = new ECS();
}

SystemManager::~SystemManager()
{
	delete ecs;
}

void SystemManager::Init(bool isEditor, GFX::Window* window)
{
	mIsEditor = isEditor;
	mWindow = window;
	mPhysicsSystem.get()->Init();
	mScriptingSystem.get()->Init();
	mAudioSystem.get()->Init();
	mGraphicsSystem.get()->Init();
	mResourceSystem.get()->Init();			// all the resources are loaaded here
}

void SystemManager::Update(float dt)
{
	mPhysicsSystem.get()->Update(dt);
	mScriptingSystem.get()->Update(dt);
	mGraphicsSystem.get()->Update(dt);
//	mResourceSystem.get()->Update();
	mAudioSystem.get()->Update(dt);
}

void SystemManager::Exit()
{
	mPhysicsSystem.get()->Exit();
	mScriptingSystem.get()->Exit();
	mGraphicsSystem.get()->Exit();
	mAudioSystem.get()->Exit();
	mResourceSystem.get()->Exit();
}
}

PhysicsSystem* SystemManager::GetPhysicsPointer()
{
	return mPhysicsSystem.get();
}
