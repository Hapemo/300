/*!*****************************************************************************
\file Singleton.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Singleton base class, a design pattern that allows creation of only one instance
of a class.
*******************************************************************************/

#pragma once

#include "ISingleton.h"

template <typename T>
class Singleton : public ISingleton{
private:
	static T* instance;

	/*!*****************************************************************************
	\brief
	Frees the singleton memory if it exists
	*******************************************************************************/
	void Destroy() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

protected:
	Singleton() = default;

public:
	// Default ctor and dtor
	~Singleton() = default;

	// To disable any copying mechanism for Singleton
	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator= (Singleton&&) = delete;

	/*!*****************************************************************************
	\brief
	Returns the Singleton instance, make one if there is none

	\return T*
	- Current singleton instance
	*******************************************************************************/
	static T* GetInstance() {
		if (!instance) instance = new T;
		return instance;
	}

};

template<typename T>
T* Singleton<T>::instance = nullptr;


