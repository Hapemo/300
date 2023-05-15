/*!*****************************************************************************
\file ECS_systems.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains system classes that controls the entities
*******************************************************************************/
#pragma once
#include "pch.h"

#include "ECS_components.h"
#include "ECS_items.h"
#include "ECS_managers.h"

// Manages the lifespan of entities
class LifespanManager : public System {
public:
	/*!*****************************************************************************
	Update the lifespan of entity

	/param float
	- Delta time used to calculate the lifespan
	*******************************************************************************/
	void Update(float);

	/*!*****************************************************************************
	Reset the lifepan of entities

	/param std::set<Entity>
	- Entities to reset

	\param float
	- New amount of lifespan Entities should have. By default 0 so it won't reset
	*******************************************************************************/
	void Reset(std::set<Entity>, float = 0);

private:
	// Nothing for now 
};















