#pragma once
#include <pch.h>
#include <Window.hpp>

class PhysicsSystem;
class ScriptingSystem;

class SystemManager
{
public:
	std::unique_ptr<PhysicsSystem> mPhysicsSystem;
	std::unique_ptr<ScriptingSystem> mScriptingSystem;
	
	GFX::Window* mWindow;

	SystemManager();
	~SystemManager();
	void Init();
	void Update(float dt);
	void Exit();
};

extern SystemManager* systemManager;