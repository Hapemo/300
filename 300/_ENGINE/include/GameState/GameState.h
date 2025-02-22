/*!*****************************************************************************
\file GameState.h
\author Jazz Teoh Yu Jue, Charissa Yip
\par DP email: j.teoh\@digipen.edu, charissa.yip@digipen.edu
\date 29-09-2023
GameState is a container that manages a range of scenes. It acts as manager for
the scenes. Only 1 gmae state should be loaded and running in the game at a 
time.
*******************************************************************************/
#pragma once
#include "pch.h"
#include "Scene.h"

class GameState {
public:
	/*!*****************************************************************************
	Initialise the scenes of the game state, only initially active scenes will init.
	*******************************************************************************/
	void Init();


	/*!*****************************************************************************
	Exit all the scenes in the game state, all scenes' exit will be called.
	*******************************************************************************/
	void Exit();

	
	// *EDITOR ONLY FUNCTIONALITIES*
	/*!*****************************************************************************
	Add new scenes to a game state via json file or load an existing scene into the
	gamestate. (Should not be used when gamestate is playing in editor mode)

	\param std::filesystem::path const&
	- File path of scene file. Leave this empty to create a new scene without a json
	file
	*******************************************************************************/
	void AddScene(std::string const& _name = std::string());

	/*!*****************************************************************************
	Remove a scene from the game state.

	\param std::string const&
	- Name of scene to remove
	*******************************************************************************/
	void RemoveScene(std::string const& _name);

	/*!*****************************************************************************
	Get a scene in current game state by comparing name

	\param _name
	- Name of scene to geet

	\return Scene*
	- Pointer to scene, returns error scene if scene not found
	*******************************************************************************/
	Scene* GetScene(std::string const& _name);

	/*!*****************************************************************************
	Load in a game state file, running al the load of it's scenes too.

	\param std::filesystem::path const&
	- File path of the gamestate file 
	*******************************************************************************/
	void Load(std::string const& _name);

	/*!*****************************************************************************
	Save the game state data.
	*******************************************************************************/
	void Save();

	/*!*****************************************************************************
	Unload a gamestate and all its scenes
	*******************************************************************************/
	void Unload();
	
	std::string mName="";
	std::vector<Scene> mScenes{};
private:

};