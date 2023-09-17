#pragma once
#include <pch.h>
#include <Window.hpp>

class PhysicsSystem;
class ScriptingSystem;
class AudioSystem;
//class AudioManager;
class GraphicsSystem;
class ECS;

class SystemManager
{
	GFX::Window *mWindow;
	bool mIsEditor;

public:
	std::unique_ptr<PhysicsSystem> mPhysicsSystem;
	std::unique_ptr<ScriptingSystem> mScriptingSystem;
	//std::unique_ptr<AudioManager> mAudioSystem;
	std::unique_ptr<AudioSystem> mAudioSystem;
	
	std::unique_ptr<GraphicsSystem> mGraphicsSystem;

	ECS *ecs;

	SystemManager();
	~SystemManager();
	GFX::Window *GetWindow() { return mWindow; }
	bool IsEditor() { return mIsEditor; }
	void Init(bool isEditor, GFX::Window *window);
	void Update(float dt);
	void Exit();
	PhysicsSystem *GetPhysicsPointer();
};

extern SystemManager *systemManager;