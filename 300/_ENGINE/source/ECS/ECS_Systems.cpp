#include "Physics/PhysicsSystem.h"
#include "ECS/ECS_Systems.h"
#include "ScriptingSystem.h"
#include "rttr/registration.h"

SystemManager* systemManager;

static void f() { std::cout << "Hello World" << std::endl; }

RTTR_REGISTRATION
{
		using namespace rttr;
		registration::method("f", &f);
}

SystemManager::SystemManager()
{
	mPhysicsSystem = std::make_unique<PhysicsSystem>();
	mScriptingSystem = std::make_unique<ScriptingSystem>();
}

SystemManager::~SystemManager()
{

}

void SystemManager::Init()
{
	mPhysicsSystem.get()->Init();
	mScriptingSystem.get()->Init();
	rttr::type::invoke("f", {});
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
