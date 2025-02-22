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
class AIManager;
class PathfinderManager;
class ButtonSystem;
struct Entity;

class SystemManager {
	GFX::Window *mWindow;
	bool mIsEditor;
	bool mIsQuit;
public:
	// changing pause states in script according to button press
	bool mIsInGamePause;
	bool mIsDialogue;
	bool mIsPlay;

	std::unique_ptr<PhysicsSystem> mPhysicsSystem;
	std::unique_ptr<ScriptingSystem> mScriptingSystem;
	std::unique_ptr<GraphicsSystem> mGraphicsSystem;
	std::unique_ptr<ResourceTy> mResourceTySystem;
	std::unique_ptr<GameStateManager> mGameStateSystem;
	std::unique_ptr<AudioSystem> mAudioSystem;
	std::unique_ptr<Logger> mLogger;
	std::unique_ptr<InputMapSystem> mInputActionSystem;
	std::unique_ptr<AIManager> mAISystem;
	std::unique_ptr<PathfinderManager> mPathfinderSystem;
	std::unique_ptr<ButtonSystem> mButtonSystem;

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
	void ResetForChangeGS();
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
	void Quit();
	/*!*****************************************************************************
	Cleaning up before exiting.
	*******************************************************************************/
	void Exit();

	bool isSystemPaused() {return !mIsPlay; }

	bool IsQuit() { return mIsQuit; }

	void SetIsPause(bool pauseanot) { mIsInGamePause = pauseanot; }

	int GetWindowWidth() { return mWindow->GetScreenWidth(); }
	
	void DeleteEntity(Entity e);
	PhysicsSystem *GetPhysicsPointer();
	ScriptingSystem* GetScriptingPointer();
	AudioSystem* GetAudioPointer();
	InputMapSystem* GetInputMapSystemPointer();
	GameStateManager* GetGameStateSystem();
	GraphicsSystem* GetGraphicsSystem();
	AIManager* GetAIManager();
	PathfinderManager* GetPathfinderManager();
};

extern SystemManager *systemManager;