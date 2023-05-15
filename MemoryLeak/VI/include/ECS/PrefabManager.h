/*!*****************************************************************************
\file PrefabManager.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022

This file contain Prefab Manager, which manages the creation, loading and saving
of prefabs

Prefabs are resources. They're json files containing information of components.
Load in prefabs into a vector container.
Prefab manager can create prefabs and store it as a new prefab json file.
Prefab is an "editor-only" functionality. Start of editor application should
load in all prefabs usable by designers.
Prefab's memory is dynamic, thus unable to use resource manager to manage
*******************************************************************************/
#pragma once
#include "pch.h"
#include "ResourceManager.h"
#include "Singleton.h"

class Prefab;

//------------------------------------------------------------------------------------
// Prefab manager contains all the prefabs, drawing these data from resource manager.
// It allows editor to present all prefabs. 
// It also allows creation, and saving of new/current prefabs.
//------------------------------------------------------------------------------------
class PrefabManager : public Singleton<PrefabManager> {
public:
	using PrefabPtr = std::shared_ptr<Prefab>;
	// Load all prefabs in from resource manager.
	// Store them into the container here for ease of usage. (1)
	// Maybe store pointers to those data instead. (2)
	
	// This function should be called by resource manager when it is loading all resources

	/*!*****************************************************************************
	Load prefabs in from json file

	\param std::filesystem::path const&
	- File path of the prefab file
	*******************************************************************************/
	void LoadPrefab(std::filesystem::path const&);

	/*!*****************************************************************************
	Create a new empty prefab and append it to PrefabManager's container

	\return PrefabPtr
	- Pointer to newly created prefab
	*******************************************************************************/
	PrefabPtr CreatePrefab();

	/*!*****************************************************************************
	Deserialise prefab into json file

	\return PrefabPtr
	- Pointer to newly created prefab
	*******************************************************************************/
	void SavePrefab(std::string const&);

	/*!*****************************************************************************
	Get prefab data

	\param std::string const& 
	- Name of prefab

	\return PrefabPtr
	- Pointer to newly created prefab
	*******************************************************************************/
	PrefabPtr GetPrefab(std::string const&);
	std::vector<PrefabPtr>& GetPrefab() { return mPrefabs; }
private:
	std::vector<PrefabPtr> mPrefabs;
};
