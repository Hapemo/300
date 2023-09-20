#pragma once
#include "pch.h"
#include "../Serialization/Serialization.h"

class ObjectFactory
{
public:
	static void DeserializeScene(const std::string& filename);
	static void SerializeScene(const std::string& filename);

	//serializationManager->LoadScene(*this, _path); TODO
	static void LoadScene(/*gamestate,*/const std::string& filename);

	//serializationManager->SaveScene(*this); TODO
	static void SaveScene(/*gamestate*/);

	//serializationManager->LoadGameState(*this, _path); TODO
	static void LoadGameState();

	//serializationManager->SaveGameState(*this); TODO
	static void SaveGameState(/*gamestate*/);

	static void DeserializePrefab(const std::string& filename);
	static void SerializePrefab(Entity e, const std::string& filename);
};