#pragma once
#include <pch.h>
#include <Window.hpp>

class PhysicsSystem;
class ScriptingSystem;
class mGraphicsSystem;

class SystemManager
{
	GFX::Window* mWindow;
	bool mIsEditor;
public:
	std::unique_ptr<PhysicsSystem> mPhysicsSystem;
	std::unique_ptr<ScriptingSystem> mScriptingSystem;
	std::unique_ptr<GraphicsSystem> mGraphicsSystem;
	

	SystemManager();
	~SystemManager();
	GFX::Window* GetWindow() { return mWindow; }
	bool IsEditor() { return mIsEditor; }
	void Init(bool isEditor, GFX::Window* window);
	void Update(float dt);
	void Exit();
};

extern SystemManager* systemManager;