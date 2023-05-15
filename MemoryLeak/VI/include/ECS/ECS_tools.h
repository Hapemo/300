/*!*****************************************************************************
\file ECS_tools.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains easier implementation of using the Coordinate in ECS. 
The functions here calls the exact function in ECS using singleton Coordinate.
*******************************************************************************/
#pragma once

#include "ECS_systems.h"
#include "ECS_items.h"
#include "ECS_managers.h"

namespace ECS {
	// From EntityID Manager
	/*!*****************************************************************************
	Create an entity

	\param EntityID
	- ID of newly created entity
	*******************************************************************************/
	Entity CreateEntity();

	/*!*****************************************************************************
	Destroy entity from all ECS managers

	\param EntityID
	- ID of entity
	*******************************************************************************/
	void DestroyEntity(EntityID _entity);

	// From Component Manager
	/*!*****************************************************************************
	Register new component to ECS
	*******************************************************************************/
	template<typename T>
	void RegisterComponent() { Coordinator::GetInstance()->RegisterComponent<T>(); }

	/*!*****************************************************************************
	Add a component to entity

	\param EntityID
	- ID of an Entity

	\param T
	- Component data to add into entity
	*******************************************************************************/
	template<typename T>
	void AddComponent(EntityID _entity, T _component) { Coordinator::GetInstance()->AddComponent<T>(_entity, _component); }

	/*!*****************************************************************************
	Remove a component from an entity

	\param EntityID
	- ID of an Entity
	*******************************************************************************/
	template<typename T>
	void RemoveComponent(EntityID _entity) { Coordinator::GetInstance()->RemoveComponent(_entity); }

	/*!*****************************************************************************
	Get the component data of an entity

	\param EntityID
	- ID of an Entity

	\return T&
	- Component data of an entity
	*******************************************************************************/
	template<typename T>
	T& GetComponent(EntityID _entity) { return Coordinator::GetInstance()->GetComponent<T>(_entity); }

	/*!*****************************************************************************
	Get the component type of a component

	\return ComponentType
	- ComponentType of a component
	*******************************************************************************/
	template<typename T>
	ComponentType GetComponentType() { return Coordinator::GetInstance()->GetComponentType<T>(); }

	/*!*****************************************************************************
	Checks if an entity has a component

	\return EntityID
	- ID of an entity
	*******************************************************************************/
	template<typename T>
	bool HasComponent(EntityID _entity) { return Coordinator::GetInstance()->HasComponent<T>(_entity); }

	// From System Manager
	/*!*****************************************************************************
	Register a system

	\return std::shared_ptr<T>
	- Pointer to newly registered system
	*******************************************************************************/
	template<typename T>
	std::shared_ptr<T> RegisterSystem() { return Coordinator::GetInstance()->RegisterSystem<T>(); }

	/*!*****************************************************************************
	Set the signature of a system

	\param const Signature&
	- Signature to assign to system
	*******************************************************************************/
	template<typename T>
	void SetSystemSignature(const Signature& _signature) { return Coordinator::GetInstance()->SetSystemSignature<T>(_signature); }

	// Extra
	/*!*****************************************************************************
	Unlink prefabs from entity
	*******************************************************************************/
	void UnlinkPrefab(EntityID _e);


	/*!*****************************************************************************
	Destroy all entities in ECS
	*******************************************************************************/
	void DestroyAllEntities();

	/*!*****************************************************************************
	Destroy all entities except specified ones

	/param const std::vector<EntityID>&
	- A bunch of entities to be blacklisted from being deleted
	*******************************************************************************/
	void DestroySomeEntites(const std::set<Entity>&);

	bool EntityExists(EntityID _e);
}
