/*!*****************************************************************************
\file GameStateManager.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Manages the gamestate flow, control which game state is running or will be 
running.

For Milestone 3:
Added additional features for editor and game mode to manage the game state flow
*******************************************************************************/
#pragma once
#include "pch.h"
#include "Singleton.h"
#include "GameState.h"

class GameStateManager : public Singleton<GameStateManager> {
public:
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
	void Update();

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

	enum class E_GSMSTATE {
		EXIT,
		RESTART,
		RUNNING,
		CHANGING,
		STARTING // Currently no use, just informative
	};

	static const std::string EXIT;
	static const std::string RESTART;
	Scene mErrorScene;
	const Entity mErrorEntity;
	static E_GSMSTATE mGSMState;
	GameState* mCurrentGameState; 

	/*!*****************************************************************************
	Change the current game state to another indicated game state. Use defined
	constant EXIT or RESTART to exit or restart the game.

	\param std::string const&
	- Name of the next game state
	*******************************************************************************/
	void ChangeGameState(std::string const& _name);

	/*!*****************************************************************************
	Change the current game state to another indicated game state. Use defined
	constant EXIT or RESTART to exit or restart the game.
	This is being called constantly every loop to check if there is changes

	\param std::string const&
	- Name of the next game state
	*******************************************************************************/
	void UpdateNextGSMState();

	/*!*****************************************************************************
	Change the current game state to another indicated game state. Use defined
	constant EXIT or RESTART to exit or restart the game.

	\param std::string const&
	- Name of the next game state
	*******************************************************************************/
	Scene& SelectScene(std::string const& _name);

	/*!*****************************************************************************
	Exit the game, basically calling ChangeGameState(EXIT);
	*******************************************************************************/
	void GameStateExit();

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
	/*!*****************************************************************************
	Add new game state to game state manager's control

	\param std::string const&
	- Path of new game state
	*******************************************************************************/
	void AddGameState(std::filesystem::path const& _path = std::filesystem::path());

	/*!*****************************************************************************
	Remove the game state specified

	\param GameState* 
	- Pointer to the game state to remove from GameStateManager
	*******************************************************************************/
	void RemoveGameState(GameState* = nullptr);

	/*!*****************************************************************************
	Set the mCurrentGameState to specified game state

	\param std::string const&
	- Name of game state to set to
	*******************************************************************************/
	bool SetGameState(std::string const& _name);
	
	/*!*****************************************************************************
	Rename a game state

	\param GameState*
	- Pointer to the game state
	
	\param std::string const&
	- Game state's new name
	*******************************************************************************/
	void RenameGameState(GameState* _gs, std::string const& _name);

	/*!*****************************************************************************
	Set the next game state path

	\param std::string const&
	- Next game state's path
	*******************************************************************************/
	void SetNextGSPath(std::string const& _path) { mNextGSPath = _path; }

	/*!*****************************************************************************
	Find the gamestate name stated. Returns iterator position of gamestate if found,
	returns end position if not found

	\param std::string const&
	- Name of game state

	\return std::vector<GameState>::iterator
	- Iterator position of game state if found, mGameState.end() otherwise.
	*******************************************************************************/
	inline std::vector<GameState>::iterator FindGameState(std::string const& _name) {
		return std::find_if(mGameStates.begin(), mGameStates.end(), [_name] (GameState& gs) -> bool { return gs.mName == _name; });
	}

	std::vector<GameState> mGameStates{}; // Only 1 in game, multi gamestates allowed during editor mode
	std::map<std::string,std::vector<bool>> mGameStatesScenesPause; // One std::vector<bool> for each game state to keep record of their pause
	std::string mNextGSPath;

private:
};

