#pragma once
#include <pch.h>
#include <Window.hpp>

class PhysicsSystem;
class ScriptingSystem;
class ECS;

class SystemManager
{
public:
	std::unique_ptr<PhysicsSystem> mPhysicsSystem;
	std::unique_ptr<ScriptingSystem> mScriptingSystem;
	
	GFX::Window* mWindow;
	ECS* ecs;

	SystemManager();
	~SystemManager();
	void Init();
	void Update(float dt);
	void Exit();
};

extern SystemManager* systemManager;