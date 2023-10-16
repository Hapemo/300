/*!*****************************************************************************
\file ECS_Systems.h
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Pepe Production
\date 15/08/2023
\brief
Struct that contains the entire engine.
*******************************************************************************/
#pragma once
#include <pch.h>
#include <Window.hpp>


class PhysicsSystem;
class ScriptingSystem;
class GraphicsSystem;
class GameStateManager;
class ECS;
class ResourceTy;
class AudioSystem;
class Logger;
class InputMapSystem;

class SystemManager {
	GFX::Window *mWindow;
	bool mIsEditor;
	bool mIsPlay;
public:
	std::unique_ptr<PhysicsSystem> mPhysicsSystem;
	std::unique_ptr<ScriptingSystem> mScriptingSystem;
	std::unique_ptr<GraphicsSystem> mGraphicsSystem;
	std::unique_ptr<ResourceTy> mResourceTySystem;
	std::unique_ptr<GameStateManager> mGameStateSystem;
	std::unique_ptr<AudioSystem> mAudioSystem;
	std::unique_ptr<Logger> mLogger;
	std::unique_ptr<InputMapSystem> mInputActionSystem;


	ECS *ecs;

	std::vector<std::string> mComponents;

	/*!*****************************************************************************
	Constructor.
	*******************************************************************************/
	SystemManager();
	/*!*****************************************************************************
	Cleanup.
	*******************************************************************************/
	~SystemManager();
	/*!*****************************************************************************
	Returns a pointer to the window.
	*******************************************************************************/
	GFX::Window *GetWindow() { return mWindow; }
	/*!*****************************************************************************
	Flag to check if the current application is an editor.
	*******************************************************************************/
	bool IsEditor() { return mIsEditor; }
	/*!*****************************************************************************
	Initialize systems with required info.
	*******************************************************************************/
	void Init(bool isEditor, GFX::Window *window);
	/*!*****************************************************************************
	Reloads the gamestate and its scenes.
	*******************************************************************************/
	void Reset();
	/*!*****************************************************************************
	Pauses the game.
	*******************************************************************************/
	void Pause();
	/*!*****************************************************************************
	Resumes the game.
	*******************************************************************************/
	void Play();
	/*!*****************************************************************************
	Update all systems.
	*******************************************************************************/
	void Update(float dt);
	/*!*****************************************************************************
	Cleaning up before exiting.
	*******************************************************************************/
	void Exit();
	PhysicsSystem *GetPhysicsPointer();
	ScriptingSystem* GetScriptingPointer();
};

extern SystemManager *systemManager;