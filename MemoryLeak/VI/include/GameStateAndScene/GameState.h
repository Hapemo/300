/*!*****************************************************************************
\file GameState.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
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
	Update the scene every loop, only active scenes will update.
	*******************************************************************************/
	void Update();

	/*!*****************************************************************************
	Exit all the scenes in the game state, all scenes' exit will be called.
	*******************************************************************************/
	void Exit();

	
	// *EDITOR ONLY FUNCTIONALITIES*
	/*!*****************************************************************************
	Add new scenes to a game state via json file or load an existing scene into the
	gamestate.

	\param std::filesystem::path const&
	- File path of scene file. Leave this empty to create a new scene without a json
	file
	*******************************************************************************/
	void AddScene(std::filesystem::path const& _path = std::filesystem::path());

	/*!*****************************************************************************
	Remove a scene from the game state.

	\param std::string const&
	- Name of scene to remove
	*******************************************************************************/
	void RemoveScene(std::string const& _name); 

	/*!*****************************************************************************
	Load in a game state file, running al the load of it's scenes too.

	\param std::filesystem::path const&
	- File path of the gamestate file 
	*******************************************************************************/
	void Load(std::filesystem::path const& _path);

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
	Transform mCamera{ {1600.f,900.f} , 1.f , {0.f,0.f}};
private:

};






































