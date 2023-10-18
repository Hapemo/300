/*!*************************************************************************
****
\file ObjectFactory.h
\author Charissa Yip, Jazz Teoh Yu Jue, Kew Yu Jun
\par DP email: charissa.yip@digipen.edu
\date 28-9-2023
\brief

This file contains declarations of functions to facilitate the loading
and saving of prefabs, scenes and gamestates using serialization.

****************************************************************************
***/
#pragma once
#include "pch.h"
#include "Serialization/Serialization.h"

class GameState;
class Scene;
struct Entity;

class ObjectFactory
{
public:
	//static void DeserializeScene(const std::string& filename);
	//static void SerializeScene(const std::string& filename);

/***************************************************************************/
/*

\brief
Function that loads a scene by deserializing data from a file and store
them into a scene.

\param scene
The scene variable to store the data into

\param filename
The file to deserialize

*/
/**************************************************************************/
	static void LoadScene(Scene* scene, const std::string& filename);

/***************************************************************************/
/*

\brief
Function that loads a gamestate by deserializing data from a file and store
them into a gamestate.

\param gs
The gamestate variable to store the data into

\param filename
The file to deserialize

*/
/**************************************************************************/
	static void LoadGameState(GameState* gs, const std::string& filename);

/***************************************************************************/
/*

\brief
Function that loads a prefab by deserializing data from a file and store
them into a gamestate.

\param filename
The file to deserialize

\param id
The id of the entity that the prefab was created from.

\return
Returns the prefab entity

*/
/**************************************************************************/
	static Entity DeserializePrefab(const std::string& filename, int id);

/***************************************************************************/
/*

\brief
Function that saves a scene by serializing data from the scene variable
into a .scn object then serializing it into a file. The filename is created
by parsing the scene name to a directory.

\param scene
The scene variable to serialize into a file

*/
/**************************************************************************/
	static void SaveScene(Scene* scene);

/***************************************************************************/
/*

\brief
Function that saves a gamestate by serializing data from the gamestate variable
into a .gs object then serializing it into a file. The filename is created
by parsing the gamestate name to a directory.

\param gs
The gamestate variable to serialize into a file

*/
/**************************************************************************/
	static void SaveGameState(GameState* gs);

/***************************************************************************/
/*

\brief
Function that saves creates a prefab of the selected entity.

\param e
The entity to create a prefab from

\param filename
The filename to save the prefab into

*/
/**************************************************************************/
	static void SerializePrefab(Entity e, const std::string& filename);

	static void SaveEntity(Entity e, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	static void LoadEntity(Entity e, rapidjson::Value& reader);
};