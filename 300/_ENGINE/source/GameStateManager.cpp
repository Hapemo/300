/*!*************************************************************************
****
\file GameStateManager.cpp
\author Lee Kwok Yew Glenn
\par DP email: kwokyewglenn.lee\@digipen.edu
\date 22-9-2022
\brief
This file contains functions that manages the different states of the game.

The functions include:
- GameStateManager
This function creates a default constructor and stores each level's states
into their appropriate array of function pointers.

- Init
This function creates a camera object and initializes the first level's init 
state.

- Update
This function calls the current level's update state. When pause, the 
update state will not run. When going into the next level, it will call the 
free function and initializes the next level.

- Free
This function calls the current level's free state. 

- FreeCamera
This function deletes the camera object.

- Save
This function calls the current level's save state.

- Exit
This function exits the application.
****************************************************************************
***/

//#include "../PCH/pch.h"
#include "GameStateManager.h"
//#include "../Engine/Engine.h"
//#include "../ECS/EntityManager.h"
//#include "../Levels/Level1.h"
//#include "../Input/InputSystem.h"
//#include "../GraphicsComponents/Camera.h"
//#include "../GraphicsComponents/Transform.h"
//#include "../Object/ObjectFactory.h"
//#include "../../../GUI/src/Editor/Editor.h"
//#include "../../../GUI/src/Editor/EditorWindow.h"
//#include "../Audio/AudioSystem.h"
#include "Object/ObjectFactory.h"
#include "Window.hpp"

GameStateManager::GameStateManager()
{

}

void GameStateManager::Init()
{
	// Creates a camera object
	//int entity = g_engine->entityMgr->AddNewEntity();
	//g_engine->entityMgr->entityName = "Camera";
	//g_engine->entityMgr->EditName();
	//g_engine->entityMgr->AddComponent<Transform>(entity);
	//g_engine->entityMgr->AddComponent<Camera>(entity);
	/*PASSERTMSG(g_engine->entityMgr->HasComponent<Transform>(entity), "PLEASE ADD TRANFORM COMPONENT!");
	PASSERTMSG(g_engine->entityMgr->HasComponent<Camera>(entity), "PLEASE ADD CAMERA COMPONENT!");*/

	//initState[currentState]();
	ObjectFactory::DeserializeScene("../Resources/scenes/Digipen.json");
}

void GameStateManager::Update()
{
	static bool once{};
	static bool twice{ true };

	if (isPlaying)
	{
		//Editor::Entity_Selected = false;

		if (!once)
		{
			// Serialize a temporary JSON to store the current Scene
			ObjectFactory::SerializeScene("../Resources/scenes/temp.json");
			once = true;
			twice = false;
		}
	}
	else
	{
		if (!twice)
		{
			//g_engine->audioSys->StopAllAudio();
			//g_engine->entityMgr->ClearAllEntities();
			ObjectFactory::DeserializeScene("../Resources/scenes/temp.json");
			once = false;
			twice = true;
		}
	}
}

void GameStateManager::Free()
{
	//ObjectFactory::FreeAllEntities();
}

void GameStateManager::FreeCamera()
{
	//g_engine->entityMgr->DestroyEntity(0);
}

void GameStateManager::Save()
{
	ObjectFactory::SerializeScene(sceneFileName);
}

void GameStateManager::Exit() 
{
	//something->CloseWindow(); 
}
