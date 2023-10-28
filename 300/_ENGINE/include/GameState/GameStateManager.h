/*!*****************************************************************************
\file GameStateManager.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Manages the gamestate flow, control which game state is running or will be 
running.

In editor mode, gamestate manager init will not load in any gamestate
Editor has to call ChangeGameState("gamestate name") to get a gamestate out.
If editor wants to close a gamestate and have blank state, call
ChangeGameState(E_GSMSTATE::NONE). Create a new empty gamestate 
with NewGameState("name")

M_GSMSTATE::EXIT is only used for closing the game in GAMEMODE only.
*******************************************************************************/
#pragma once
#include "pch.h"
#include "GameState.h"

class GameStateManager {
public:
	enum class E_GSMSTATE {
		EXIT,
		RESTART,
		RUNNING,
		CHANGING,
		NONE // Currently no use, just informative
	};

	GameStateManager();
	~GameStateManager() {};

	/*!*****************************************************************************
	Initialise all the game states that will be ran and the starting game state
	*******************************************************************************/
	void Init();

	/*!*****************************************************************************
	Main game update loop. Runs application's updates and current game state's 
	update. 
	*******************************************************************************/
	//void Update(); //(DEPRECATED) VI Engine now uses script to run game logic

	/*!*****************************************************************************
	Unload all game states befor exiting the game. Should be called in application
	exit
	*******************************************************************************/
	void Unload();

	/*!*****************************************************************************
	Controls the next gamestate to run with input keys. Should be called after
	current game state.
	*******************************************************************************/
	//static void GSControlPanel();

	/*!*****************************************************************************
	Change the current game state to another indicated game state.

	\param std::string const&
	- Name of the next game state
	*******************************************************************************/
	void ChangeGameState(std::string const& _name);

	/*!*****************************************************************************
	Change the current game state to another indicated game state. E_GSMSTATE to
	switch to EXIT or RESTART, or NONE if in editor mode

	\param E_GSMSTATE const&
	- Game state to change to
	*******************************************************************************/
	void ChangeGameState(E_GSMSTATE const& _state);

	/*!*****************************************************************************
	Change the current game state to another indicated game state. Use defined
	constant EXIT or RESTART to exit or restart the game.
	This is being called before end of every loop to check if there is changes

	\param std::string const&
	- Name of the next game state
	*******************************************************************************/
	void UpdateNextGSMState();

	/*!*****************************************************************************
	Get a loaded scene from a current game state

	\param std::string const&
	- Name of the scene
	*******************************************************************************/
	Scene& SelectScene(std::string const& _name);

	/*!*****************************************************************************
	Update the gamestate to EXIT, basically calling ChangeGameState(EXIT);
	*******************************************************************************/
	void Exit();

	/*!*****************************************************************************
	Get an entity from current gamestate using it's name, it'll get the first one it 
	found. if scene is specified, it will only select from that scene

	\param std::string const&
	- Name of the entity to get

	\param std::string const&
	- Scene name to search from

	\return Entity
	- Entity found. Returns error entity with id 0 if not found.
	*******************************************************************************/
	Entity GetEntity(std::string const& _entityName, std::string const& _sceneName = "");

	//----------------------------------
	// Editor only functionalities
	//----------------------------------

	/*!*****************************************************************************
	Set the next game state path

	\param std::string const&
	- Next game state's path
	*******************************************************************************/
	void SetNextGSPath(std::string const& _path) { mNextGSName = _path; }

	/*!*****************************************************************************
	Check if game loop should close
	*******************************************************************************/
	bool Exited() const { return mGSMState == E_GSMSTATE::EXIT; }

	/*!*****************************************************************************
	Create a new empty gamestate. If there is another gamestate opened, Unload it
	and replace with new empty gamestate
	*******************************************************************************/
	void NewGameState(std::string const& _name);

	/*!*****************************************************************************
	Transfer the entity from one scene to another in current gamestate

	\param _srcName or _src or _srcID
	- String name of scene or pointer to scene or ID position in gamestate's scene container, source of entity

	\param _dstName or _dst or _dstID
	- String name of scene or pointer to scene or ID position in gamestate's scene container, destination of entity

	\return bool
	- true if successfully transferred, otherwise false
	*******************************************************************************/
	bool TransferEntity(std::string const& _srcName, std::string const& _dstName, Entity _e);
	bool TransferEntity(Scene* _src, Scene* _dst, Entity _e);
	bool TransferEntity(int _srcID, int _dstID, Entity _e);

	/*!*****************************************************************************
	Delete an entity from existing scene

	\param _sceneID
	- ID position in gamestate's scene container

	\param _e
	- Entity to delete
	*******************************************************************************/
	void DeleteEntityFromScene(int _sceneID, Entity _e);

	/*!*****************************************************************************
	Delete an entity from gamestate

	\param _e
	- Entity to delete
	*******************************************************************************/
	void DeleteEntity(Entity _e);

	/*!*****************************************************************************
	Adds scene to current game state. If scene can be found in scene folder in
	asset, add that scene.
	
	\param _name
	- Name of scene to add
	*******************************************************************************/
	void AddScene(std::string const& _name);

	/*!*****************************************************************************
	Restart the editor's game running
	*******************************************************************************/
	void EditorRestartGameState();

	/*!*****************************************************************************
	Getter for current game state

	\return GameState*
	- current game state
	*******************************************************************************/
	GameState* GetCurrentGameState();

	/*!*****************************************************************************
	Getter for the error scene

	\return Scene*
	- error scene
	*******************************************************************************/
	Scene* GetErrorScene();

	/*!*****************************************************************************
	Checks if game state file exists in the gamestate folder in assets

	\param _name
	- Name of gamestate file
	*******************************************************************************/
	static bool GameStateFileExist(std::string const& _name);
	
	/*!*****************************************************************************
	Checks if scene file exists in the scene folder in assets

	\param _name
	- Name of scene file
	*******************************************************************************/
	static bool SceneFileExist(std::string const& _name);
	
	/*!*****************************************************************************
	Checks if the scene exists in the current game state

	\param _name
	- Name of scene
	*******************************************************************************/
	bool SceneExists(std::string const& _name);

	Scene mErrorScene;						// Error Scene, to show that some error occured when getting scene
	E_GSMSTATE mGSMState;					// The state of the gamestate
	GameState mCurrentGameState;	// Current game state that is operated on

private:
	std::string mNextGSName;			// This path has to be set initially in config file
};

