/*!*****************************************************************************
\file ECS_tools.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains easier implementation of using the Coordinate in ECS.
The functions here calls the exact function in ECS using singleton Coordinate.
*******************************************************************************/
#include "ECS_tools.h"

Entity ECS::CreateEntity() { 
	return Entity{ Coordinator::GetInstance()->CreateEntity() };
}

void ECS::DestroyEntity(EntityID _entity) { Coordinator::GetInstance()->DestroyEntity(_entity); }

void ECS::UnlinkPrefab(EntityID _e) { Coordinator::GetInstance()->UnlinkPrefab(_e); }

void ECS::DestroyAllEntities() { Coordinator::GetInstance()->DestroyAllEntities(); }

void ECS::DestroySomeEntites(const std::set<Entity>& _e) { Coordinator::GetInstance()->DestroySomeEntites(_e); }

bool ECS::EntityExists(EntityID _e) { return Coordinator::GetInstance()->EntityExists(_e); }
