/*!*****************************************************************************
\file SingletonManager.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Singleton Manager keeps a list of all Singletons and free their memory together
*******************************************************************************/

#pragma once

#include "pch.h"

class ISingleton;

class SingletonManager {
private:
	static std::vector<ISingleton*> singletons;

	// Default ctor and dtor
	SingletonManager() {}
	~SingletonManager() {}


public:
	// To disable any copying mechanism for Singleton
	SingletonManager(const SingletonManager&) = delete;
	SingletonManager& operator= (const SingletonManager&) = delete;
	SingletonManager(SingletonManager&&) = delete;
	SingletonManager& operator= (SingletonManager&&) = delete;

	/*!*****************************************************************************
	\brief
	Adds a singleton to the manager, mainly used by Singleton ctor
	
	\param ISingleton*
	- ISingleton pointer to the singleton added
	*******************************************************************************/
	static void addSingleton(ISingleton*);

	/*!*****************************************************************************
	\brief
	Deallocate all singleton's heap memory
	*******************************************************************************/
	static void destroyAllSingletons();
};