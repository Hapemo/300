/*!*****************************************************************************
\file ECS_managers.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains 4 different classes that makes up the whole ECS system.
Entity Manager - manages all the entities
ComponentArrayManager - Manages all the components
SystemManager - Manages all the system in the game
Coordinator - Encapsulation of all 3 systems using smart pointers. Anyone who
							wishes to use the ECS should go through Cooridnator
*******************************************************************************/
#pragma once
#include "pch.h"

#include "ECS_items.h"

//-------------------------------------------------------------------------
// Entity Manager
// Manages the creation and destruction of entities, incharge of 
// distributing entities on request.
// Manages the system each entities can use.
//-------------------------------------------------------------------------
class EntityManager {
public:
	/*!*****************************************************************************
	Default constructor of EntityManager. Initialises all entity IDs and reserve the
	0th entity to be null.
	*******************************************************************************/
	EntityManager();

	/*!*****************************************************************************
	Creates and returns an entity.
	*******************************************************************************/
	EntityID CreateEntity();

	/*!*****************************************************************************
	Frees an entity id for future usage
	
	\param EntityID
	- ID of an entity
	*******************************************************************************/
	void DestroyEntity(EntityID);

	/*!*****************************************************************************
	Assign an signature to an entity
	
	\param EntityID
	- ID of an entity

	\param Signature
	- Signature of an entity
	*******************************************************************************/
	void SetSignature(EntityID, Signature);

	/*!*****************************************************************************
	Access the signature of an entity
	
	\param EntityID
	- ID of an entity
	*******************************************************************************/
	Signature GetSignature(EntityID);

	bool EntityExists(EntityID _e) { return std::find(mAvailableEntities.begin(), mAvailableEntities.end(), _e) == mAvailableEntities.end(); }

	uint32_t GetEntityCount() { return mLivingEntityCount; }

private:
	// A container of unused entity ids
	std::deque<EntityID> mAvailableEntities;
	// A container of signatures with entities assigned to it
	std::array<Signature, MAX_ENTITIES> mSignatures;
	// Count of total living entities
	uint32_t mLivingEntityCount;

};


//-------------------------------------------------------------------------
// Component Array Manager
// Manages the creation of all component arrays and assign them an unique 
// ComponentType id.
// Controls the inflow and outflow of entities in all component arrays.
//-------------------------------------------------------------------------
class ComponentArrayManager {
public:
	ComponentArrayManager();

	/*!*****************************************************************************
	Initialise and register a new component
	*******************************************************************************/
	template<typename T>
	void RegisterComponent() {
		std::string typeName{ typeid(T).name() };

		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Attempted to register component more than once.");
		mComponentTypes.insert({ typeName, mNextComponentType++ });
		mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
	}
	
	/*!*****************************************************************************
	Access the ComponentType, which is ID, of a component array

	\return ComponentType
	- Component Type of a component
	*******************************************************************************/
	template<typename T>
	ComponentType GetComponentType() {
		std::string typeName{ typeid(T).name() };

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Attempted to access unregistered component.");
		return mComponentTypes[typeName];
	}
	
	/*!*****************************************************************************
	Add an entity to a component array

	\param EntityID
	- ID of an entity

	\param T
	- Component data to add to entity
	*******************************************************************************/
	template<typename T>
	void AddComponent(EntityID _entity, T _component) {
		GetComponentArray<T>()->InsertData(_entity, _component);
	}

	/*!*****************************************************************************
	Remove an entity from a component array

	\param EntityID
	- ID of an entity
	*******************************************************************************/
	template<typename T>
	void RemoveComponent(EntityID _entity) {
		GetComponentArray<T>()->RemoveData(_entity);
	}
	
	/*!*****************************************************************************
	Get component data of an entity

	\param EntityID
	- ID of an entity
	*******************************************************************************/
	template<typename T>
	T& GetComponent(EntityID _entity) {
		return GetComponentArray<T>()->GetData(_entity);
	}

	/*!*****************************************************************************
	Read component data of an entity

	\param EntityID
	- ID of an entity
	*******************************************************************************/
	template<typename T>
	T const& ReadComponent(EntityID _entity) {
		return GetComponentArray<T>()->ReadData(_entity);
	}
	
	/*!*****************************************************************************
	Destroy all components of an entity in each respective component array

	\param EntityID
	- ID of an entity
	*******************************************************************************/
	void EntityDestroyed(EntityID);

private:
	// Map of component name to ComponentType
	std::unordered_map<std::string, ComponentType> mComponentTypes;
	// Map of component name to component array
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> mComponentArrays;
	// Counting the amount of component, for assigning ComponentType to each component array
	ComponentType mNextComponentType;
	
	//Helper function
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		std::string typeName{ typeid(T).name()};

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Attempted to access unregistered component.");
		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
};

//-------------------------------------------------------------------------
// System Manager
// Manages the creation of all component arrays and assign them an unique 
// ComponentType id.
// Controls the inflow and outflow of entities in all component arrays.
//-------------------------------------------------------------------------
class SystemManager {
public:
	SystemManager();

	/*!*****************************************************************************
	Initialises system and register it into SystemManager

	\return std::shared_ptr<T>
	- Pointer to templated ComponentArray
	*******************************************************************************/
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		std::string typeName{ typeid(T).name() };

		assert(mSystems.find(typeName) == mSystems.end() && "Attempted to register system more than once.");

		auto system = std::make_shared<T>();
		mSystems.insert({ typeName, system });
		return system;
	}
 
	/*!*****************************************************************************
	Assign a signature to a system

	\param Signature
	- Signature of system
	*******************************************************************************/
	template<typename T>
	void SetSignature(Signature _signature) {
		std::string typeName{ typeid(T).name() };

		assert(mSystems.find(typeName) != mSystems.end() && "Attempted to set signature to non-existance System.");

		mSignatures.insert({ typeName, _signature });
	}

	/*!*****************************************************************************
	Remove a destroyed entity from all system's set

	\param EntityID
	- ID of an entity
	*******************************************************************************/
	void EntityDestroyed(EntityID);
 
	/*!*****************************************************************************
	Notify all systems that an entity's signature has changed.
	Remove or add it to the appropriate system's set

	\param EntityID
	- ID of an entity

	\param Signature
	- New signature of entity
	*******************************************************************************/
	void EntitySignatureChanged(EntityID, Signature);

private:
	// Map system's name to their unique signature
	std::unordered_map<std::string, Signature> mSignatures;
	// Map system's name to a pointer to the system itself
	std::unordered_map<std::string, std::shared_ptr<System>> mSystems;
};

//-------------------------------------------------------------------------
// Coordinator
// The main Coordinator system, combination of EntityID, Component Array and System managers
//-------------------------------------------------------------------------
class Coordinator : public Singleton<Coordinator> {
public:
	/*!*****************************************************************************
	Initialise EntityManager, ComponentArrayManager and SystemManager.
	*******************************************************************************/
	Coordinator();

	// From EntityID Manager
	/*!*****************************************************************************
	Create an entity

	\param EntityID
	- ID of newly created entity
	*******************************************************************************/
	EntityID CreateEntity();

	/*!*****************************************************************************
	Destroy entity from all ECS managers

	\param EntityID
	- ID of entity
	*******************************************************************************/
	void DestroyEntity(EntityID);

	// From Component Manager
	/*!*****************************************************************************
	Register new component to ECS
	*******************************************************************************/
	template<typename T>
	void RegisterComponent() { mComponentArrayManager->RegisterComponent<T>(); }

	/*!*****************************************************************************
	Add a component to entity
	
	\param EntityID
	- ID of an Entity

	\param T
	- Component data to add into entity
	*******************************************************************************/
	template<typename T>
	void AddComponent(EntityID _entity, T _component) {
		mComponentArrayManager->AddComponent<T>(_entity, _component);

		Signature signature = mEntityManager->GetSignature(_entity);
		signature.set(mComponentArrayManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(_entity, signature);
		mSystemManager->EntitySignatureChanged(_entity, signature);
	};

	/*!*****************************************************************************
	Remove a component from an entity
	
	\param EntityID
	- ID of an Entity
	*******************************************************************************/
	template<typename T>
	void RemoveComponent(EntityID _entity) {
		mComponentArrayManager->RemoveComponent<T>(_entity);
		Signature signature = mEntityManager->GetSignature(_entity);
		signature.set(mComponentArrayManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(_entity, signature);
		mSystemManager->EntitySignatureChanged(_entity, signature);
	}

	/*!*****************************************************************************
	Get the component data of an entity

	\param EntityID
	- ID of an Entity

	\return T&
	- Component data of an entity
	*******************************************************************************/
	template<typename T>
	T& GetComponent(EntityID _entity) { return mComponentArrayManager->GetComponent<T>(_entity); }

	/*!*****************************************************************************
	Get read-only component data of an entity

	\param EntityID
	- ID of an Entity

	\return T&
	- Component data of an entity
	*******************************************************************************/
	template<typename T>
	T const& ReadComponent(EntityID _entity) { return mComponentArrayManager->ReadComponent<T>(_entity); }

	/*!*****************************************************************************
	Get the component type of a component

	\return ComponentType
	- ComponentType of a component
	*******************************************************************************/
	template<typename T>
	ComponentType GetComponentType() { return mComponentArrayManager->GetComponentType<T>(); }

	/*!*****************************************************************************
	Checks if an entity has a component

	\return EntityID
	- ID of an entity
	*******************************************************************************/
	template<typename T>
	bool HasComponent(EntityID _entity) {
		return mEntityManager->GetSignature(_entity)[mComponentArrayManager->GetComponentType<T>()];
	}

	// From System Manager
	/*!*****************************************************************************
	Register a system

	\return std::shared_ptr<T>
	- Pointer to newly registered system
	*******************************************************************************/
	template<typename T>
	std::shared_ptr<T> RegisterSystem() { return mSystemManager->RegisterSystem<T>(); }

	/*!*****************************************************************************
	Set the signature of a system

	\param const Signature&
	- Signature to assign to system
	*******************************************************************************/
	template<typename T>
	void SetSystemSignature(const Signature& _signature) { return mSystemManager->SetSignature<T>(_signature); }

	// For prefab
	/*!*****************************************************************************
	Unlink a prefab from an entity

	\param EntityID
	- Entity to unlink itself from it's prefab
	*******************************************************************************/
	void UnlinkPrefab(EntityID _entity);

	// Extra
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

	/*!*****************************************************************************
	Get the count of entity in ECS

	\return uint32_t
	- Number of entities
	*******************************************************************************/
	uint32_t GetEntityCount();

	bool EntityExists(EntityID _e) { return mEntityManager->EntityExists(_e); }

private:
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<ComponentArrayManager> mComponentArrayManager;
	std::unique_ptr<SystemManager> mSystemManager;
};
 
//-------------------------------------------------------------------------
// EntityID
// EntityID and it's functions
// Declarations are in Entity struct
//-------------------------------------------------------------------------

template<typename T, typename ...args>
void Entity::AddComponent(T _component, args... _components) const {
	Coordinator::GetInstance()->AddComponent<T>(id, _component);

	if constexpr (sizeof...(_components) > 0)
		return AddComponent(_components...);
}

template<typename T>
T& Entity::GetComponent() const { return Coordinator::GetInstance()->GetComponent<T>(id); }

template<typename T>
T& Entity::GetComponent(SAFE safe) const {
	static T errorComponent{};
	(void)safe;
	if (Coordinator::GetInstance()->HasComponent<T>(id))
		return Coordinator::GetInstance()->GetComponent<T>(id);
	else {
		LOG_ERROR("Unable to find component \"" + std::string(typeid(T).name()) + "\" for entity: " + std::to_string(id));
		return errorComponent;
	}
}

template<typename T>
T const& Entity::ReadComponent() const { return Coordinator::GetInstance()->ReadComponent<T>(id); }

template<typename T>
void Entity::RemoveComponent() const { return Coordinator::GetInstance()->RemoveComponent<T>(id); }

template<typename T>
bool Entity::HasComponent() const { return Coordinator::GetInstance()->HasComponent<T>(id); }


//-------------------------------------------------------------------------
// Prefab
//-------------------------------------------------------------------------
template<typename T>
void Prefab::AddComponent(T const& _component) { // TODO: Possible optimisation to put _component into data instead of *(static_cast<T*>(mComponents[pos]))
	ComponentType pos{ Coordinator::GetInstance()->GetComponentType<T>() };
	mComponents[pos] = new T;
#ifdef NDEBUG
	std::cout << "sizeof " << sizeof(T) << '\n';
#endif
	*(static_cast<T*>(mComponents[pos])) = _component;

	for (Entity const& e : mPrefabees)
		e.AddComponent<T>(*(static_cast<T*>(mComponents[pos])));
}

template<typename T>
void Prefab::UpdateComponent(T const& _component) { // TODO: Possible optimisation to put _component into data instead of *(static_cast<T*>(mComponents[pos]))
	ComponentType pos{ Coordinator::GetInstance()->GetComponentType<T>() };
	*(static_cast<T*>(mComponents[pos])) = _component;

	for (Entity const& e : mPrefabees)
	{
		if (!e.HasComponent<T>()) e.AddComponent<T>(*(static_cast<T*>(mComponents[pos]))); // Add component if prefabee doesn't have the component.
		else e.GetComponent<T>() = *(static_cast<T*>(mComponents[pos]));
	}
}

template<typename T>
void Prefab::RemoveComponent() {
	for (Entity const& e : mPrefabees)
		e.RemoveComponent<T>();

	ComponentType pos{ Coordinator::GetInstance()->GetComponentType<T>() };
	delete mComponents[pos];
	mComponents[pos] = nullptr;
}
template<typename T>
bool Prefab::HasComponent() const {
	ComponentType pos{ Coordinator::GetInstance()->GetComponentType<T>() };
	if (mComponents[pos]==nullptr)
		return false;
	else
		return true;
	
}
template<typename T>
T const& Prefab::GetComponent() const { 
	ComponentType pos{ Coordinator::GetInstance()->GetComponentType<T>() };

	return *(static_cast<T*>(mComponents[pos]));
}

