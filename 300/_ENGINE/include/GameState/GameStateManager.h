/*!*****************************************************************************
\file GameStateManager.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
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


	// Editor only functionalities
	///*!*****************************************************************************
	//Add new game state to game state manager's control

	//\param std::string const&
	//- Path of new game state
	//*******************************************************************************/
	//void AddGameState(std::filesystem::path const& _path = std::filesystem::path());

	///*!*****************************************************************************
	//Remove the game state specified

	//\param GameState* 
	//- Pointer to the game state to remove from GameStateManager
	//*******************************************************************************/
	//void RemoveGameState(GameState* = nullptr);

	///*!*****************************************************************************
	//Set the mCurrentGameState to specified game state

	//\param std::string const&
	//- Name of game state to set to
	//*******************************************************************************/
	//bool SetGameState(std::string const& _name);
	
	///*!*****************************************************************************
	//Rename a game state

	//\param GameState*
	//- Pointer to the game state
	//
	//\param std::string const&
	//- Game state's new name
	//*******************************************************************************/
	//void RenameGameState(GameState* _gs, std::string const& _name);

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

	void AddScene(std::string const& _name);

	void EditorRestartGameState() { mGSMState = E_GSMSTATE::CHANGING; }

	GameState* GetCurrentGameState() { return &mCurrentGameState; }

	Scene* GetErrorScene() { return &mErrorScene; }

	bool SceneExists(std::string const& _name);

	///*!*****************************************************************************
	//Find the gamestate name stated. Returns iterator position of gamestate if found,
	//returns end position if not found

	//\param std::string const&
	//- Name of game state

	//\return std::vector<GameState>::iterator
	//- Iterator position of game state if found, mGameState.end() otherwise.
	//*******************************************************************************/
	//inline std::vector<GameState>::iterator FindGameState(std::string const& _name) {
	//	return std::find_if(mGameStates.begin(), mGameStates.end(), [_name] (GameState& gs) -> bool { return gs.mName == _name; });
	//}

	//std::vector<GameState> mGameStates{}; // Only 1 in game, multi gamestates allowed during editor mode
	//std::map<std::string,std::vector<bool>> mGameStatesScenesPause; // One std::vector<bool> for each game state to keep record of their pause


	Scene mErrorScene;
	E_GSMSTATE mGSMState;
	GameState mCurrentGameState;

private:
	std::string mNextGSName; // This path has to be set initially in config file. TODO
	//std::string mNextGSPath; // This path has to be set initially in config file. TODO
};

