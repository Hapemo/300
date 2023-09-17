#pragma once
#include <pch.h>
#include <Window.hpp>

class PhysicsSystem;
class ScriptingSystem;
class GraphicsSystem;
class GameStateManager;
class ECS;

class SystemManager {
	GFX::Window *mWindow;
	bool mIsEditor;

public:
	std::unique_ptr<PhysicsSystem> mPhysicsSystem;
	std::unique_ptr<ScriptingSystem> mScriptingSystem;
	std::unique_ptr<GraphicsSystem> mGraphicsSystem;
	std::unique_ptr<GameStateManager> mGameStateSystem;

	ECS *ecs;

	std::vector<std::string> mComponents;

	SystemManager();
	~SystemManager();
	GFX::Window *GetWindow() { return mWindow; }
	bool IsEditor() { return mIsEditor; }
	void Init(bool isEditor, GFX::Window *window);
	void Update(float dt);
	void Exit();
	PhysicsSystem *GetPhysicsPointer();
	ScriptingSystem* GetScriptingPointer();
};

extern SystemManager *systemManager;