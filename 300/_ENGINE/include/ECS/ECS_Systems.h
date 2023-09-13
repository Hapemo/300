#pragma once
#include <pch.h>
#include <Window.hpp>

class PhysicsSystem;
class ScriptingSystem;
class AudioManager;

class SystemManager
{
	GFX::Window* mWindow;
public:
	std::unique_ptr<PhysicsSystem> mPhysicsSystem;
	std::unique_ptr<ScriptingSystem> mScriptingSystem;
	std::unique_ptr<AudioManager> mAudioSystem;
	

	SystemManager();
	~SystemManager();
	GFX::Window* GetWindow() { return mWindow; }
	void Init(GFX::Window* window);
	void Update(float dt);
	void Exit();
};

extern SystemManager* systemManager;