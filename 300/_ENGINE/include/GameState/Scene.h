/*!*****************************************************************************
\file Scene.h
\author Jazz Teoh Yu Jue, Charissa Yip
\par DP email: j.teoh\@digipen.edu, charissa.yip@digipen.edu
\date 28-9-2023
\brief
Scene base class. Each game state can have multiple scenes which encapsulates
a group of entities and operates on them.

For Milestone 2:
Added load and unload of scene using resourceManager
*******************************************************************************/
#pragma once
//#include "ECS_items.h"
#include "pch.h"
//#include "ECSManager.h"
//#include "ResourceManager.h"

struct Entity;

class Scene {
public:
	Scene();
	//Scene(ResourceManager::GUID const&); TODO
	Scene(std::string const&);

	// Check if scene has the same name
	bool operator==(Scene const& _scene) { return mName == _scene.mName; }

	~Scene();
	/*!*****************************************************************************
	Set the scene's pause status

	\param bool
	- True to pause the scene's update loop, otherwise false
	*******************************************************************************/
	void Pause(bool);

	///*!*****************************************************************************
	//Main update loop of the game that dismisses all other updates if game is paused
	//*******************************************************************************/
	//void PrimaryUpdate();

	/*!*****************************************************************************
	Initialise variables and entities for the scene
	*******************************************************************************/
	void Init();

	///*!*****************************************************************************
	//Update the scene and entity operations every game loop.
	//*******************************************************************************/
	//void Update();

	/*!*****************************************************************************
	Resets the scene variables. This should be called in the init function. 
	*******************************************************************************/
	void Exit();
	
	/*!*****************************************************************************
	Load scene from a file path
	*******************************************************************************/
	void Load(std::string const& _name = std::string());

	// *EDITOR ONLY FUNCTION*
	void Save();

	/*!*****************************************************************************
	Unload all entities from the scene
	*******************************************************************************/
	void Unload();

	/*!*****************************************************************************
	Add Entity to scene. NOTE: Should not be called on game run-time
	*EDITOR ONLY FUNCTION*
	
	*******************************************************************************/
	Entity AddEntity();

	/*!*****************************************************************************
	Remove entity from scene. NOTE: Should not be called on game run-time
	*EDITOR ONLY FUNCTION*
	
	\param Entity const&
	- Entity to remove
	*******************************************************************************/
	void RemoveEntity(Entity);

	bool IsError();

	std::set<Entity> mEntities{};			// Entities in the scene 
	bool mIsPause = true;											// Paused state of the scene
	bool mForceRender;
	std::string mName="";
	bool selected;
	//Transform mCamera{};

private:
	bool mInitBefore = false;
};




