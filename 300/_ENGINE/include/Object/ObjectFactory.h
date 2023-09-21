#pragma once
#include "pch.h"
#include "../Serialization/Serialization.h"
//#include "GameState/GameState.h"

class GameState;

class ObjectFactory
{
public:
	static void DeserializeScene(const std::string& filename);
	static void SerializeScene(const std::string& filename);

	// *this refers to Scene pointer
	//serializationManager->LoadScene(*this, _path); TODO
	static void LoadScene(Scene* scene, const std::string& filename);
	// deserialize path
	// load stuff and push into *this->mEntities

	//serializationManager->SaveScene(*this); TODO
	static void SaveScene(Scene* scene);
	// pass in Scene pointer
	// loop thru the mEntities container and serialize
	// *this->mName + ".json"
	// serializefile(blabla.json)

	// *this refers to GameState pointer
	//serializationManager->LoadGameState(*this, _path); TODO
	static void LoadGameState(GameState* gs, const std::string& filename);

	//serializationManager->SaveGameState(*this); TODO
	static void SaveGameState(GameState* gs);

	//static void DeserializePrefab(const std::string& filename); // just use the normal deserializescene function, works exactly the same
	static void SerializePrefab(Entity e, const std::string& filename);
};