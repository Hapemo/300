/*!*****************************************************************************
\file ISingleton.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
ISingleton is only used to provide polymorphism control for SingletonManager. This
is necessary since Singleton class is templatized
*******************************************************************************/

#pragma once

#include "SingletonManager.h"

class ISingleton {
public:
	/*!*****************************************************************************
	\brief
	Adds a singleton to the SingletonManager
	*******************************************************************************/
	ISingleton() { SingletonManager::addSingleton(this); }
	
	// Default dtor
	~ISingleton() {}

	// To disable any copying mechanism for Singleton
	ISingleton(const ISingleton&) = delete;
	ISingleton& operator= (const ISingleton&) = delete;
	ISingleton(ISingleton&&) = delete;
	ISingleton& operator= (ISingleton&&) = delete;

	// Deallocate heap memory
	virtual void Destroy() = 0;
};