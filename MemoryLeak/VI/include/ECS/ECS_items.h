/*!*****************************************************************************
\file ECS_items.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022

This file contains the basic structs that make up the ECS system. It contains 
Entity, IComponentArray and ComponentArray.

For Milestone 2:
Added prefab and it's member functions

For Milestone 3:
Added additional features for entity such as activate and deactivate
*******************************************************************************/
#pragma once
#include "pch.h"
#include "SparseSet.h"
#include "Singleton.h"

//-------------------------------------------------------------------------
// Setting basic types
//-------------------------------------------------------------------------

// An ID for entities 
using EntityID = std::uint32_t;
const EntityID MAX_ENTITIES{ 5000 };

// An ID for components
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS{ 32 };

// An ID indicating which component an entity posesses
using Signature = std::bitset<MAX_COMPONENTS>;

enum SAFE : char { safe };

/*!*****************************************************************************
Entity is a part of the ECS, representing each game object
*******************************************************************************/
struct Entity {
	// Some of the member functions are defined in ECS_managers.cpp because
	// they require functions declared in ECS_managers.h
	EntityID id;

	Entity() = default;
	
	// Copy Constructor
	explicit Entity(EntityID _id) : id(_id) {}

	/*!*****************************************************************************
	Operator overload for '<' operator. This is required for std::set as set uses 
	'<' operator to arrange placement of items in std::set.

	\param 
	- the interval (in seconds) at which fps is to be calculated
	*******************************************************************************/
	bool operator<(Entity const& _entity) const { return id < _entity.id; } 

	/*!*****************************************************************************
	 Destroy an entity, returning it's ID and components.
	*******************************************************************************/
	void Destroy() const;

	/*!*****************************************************************************
	Add components to an existing Entity

	\param T
	- First component to add in

	\param ...args
	- Other components to add in if there is any
	*******************************************************************************/
	template<typename T, typename ...args>
	void AddComponent(T, args...) const;

	/*!*****************************************************************************
	Get a reference to an Entity's component 

	\param T
	- First component to add in

	\param ...args
	- Other components to add in if there is any

	\return T&
	- Reference to entity's component
	*******************************************************************************/
	template<typename T>
	T& GetComponent() const;

	template<typename T>
	T& GetComponent(SAFE) const;

	/*!*****************************************************************************
	Remove a component from entity
	*******************************************************************************/
	template<typename T>
	void RemoveComponent() const;

	/*!*****************************************************************************
	Checks if an entity has the indicated component
	
	\return bool
	- True if has indicated component, otherwise false
	*******************************************************************************/
	template<typename T>
	bool HasComponent() const;

	/*!*****************************************************************************
	Checks if the entity is active and not paused

	\return bool
	- True if it is not paused and is active, otherwise false
	*******************************************************************************/
	bool ShouldRun() const;

	/*!*****************************************************************************
	Contains operations that should be done when entity is activated.
	Set entity's isActive to true and run it's starting script
	*******************************************************************************/
	void Activate() const;
	
	/*!*****************************************************************************
	Contains operations that should be done when entity is deactivated.
	Set entity's isActive to false and run it's ending script
	*******************************************************************************/
	void Deactivate() const;

	/*!*****************************************************************************
	Get an entity's component as read-only

	\return T const&
	- Entity's component
	*******************************************************************************/
	template<typename T>
	T const& ReadComponent() const;

	/*!*****************************************************************************
	Add child to entity, for parent-child hierarchy

	\param _e
	- Entity to add as child
	*******************************************************************************/
	void AddChild(Entity _e) const;
};



//-------------------------------------------------------------------------
// IComponentArray
// Only used for polymorphism for ComponentManager
//-------------------------------------------------------------------------
class IComponentArray {
public:
	virtual ~IComponentArray() = default;

	/*!*****************************************************************************
	Remove component from an entity

	\return bool
	- True if has indicated component, otherwise false
	*******************************************************************************/
	virtual void RemoveData(EntityID) = 0;

	/*!*****************************************************************************
	Checks if a component array has data of an entity

	\return bool
	- True if has data of entity, otherwise false
	*******************************************************************************/
	virtual bool HasEntity(EntityID) = 0;
};


//-------------------------------------------------------------------------
// ComponentArray
// An array for each components and entity ID to keep track which entity is using it
//-------------------------------------------------------------------------
template<typename T>
class ComponentArray : public IComponentArray {
public:
	ComponentArray();

	/*!*****************************************************************************
	Assign a component to an entity and give it a specified component data

	\param EntityID
	- ID of entity

	\param T
	- Component data to insert into entity
	*******************************************************************************/
	void InsertData(EntityID, T);

	/*!*****************************************************************************
	Remove a component from an entity

	\param EntityID
	- ID of entity
	*******************************************************************************/
	void RemoveData(EntityID);
	
	/*!*****************************************************************************
	Access the component information of an entity

	\param EntityID
	- ID of entity

	\return T&
	- Reference of component to entity
	*******************************************************************************/
	T& GetData(EntityID);

	/*!*****************************************************************************
	Read the component information of an entity

	\param EntityID
	- ID of entity

	\return T&
	- Reference of component to entity
	*******************************************************************************/
	T const& ReadData(EntityID);

	/*!*****************************************************************************
	Checks if the Component array has data of an entity

	\param EntityID
	- ID of entity

	\return bool
	- True if has data of the entity, otherwise false
	*******************************************************************************/
	bool HasEntity(EntityID);

private:
	SparseSet<T> mData;
};

//-------------------------------------------------------------------------
// System
// Base class containing a set of entities which the system can operate on
// 
// A system is any functionality that iterates upon a list of entities with 
// a certain signature of components.
//-------------------------------------------------------------------------
class System {
public:
	std::set<Entity> mEntities;
};



//-------------------------------------------------------------------------
// Prefab
// 
// Prefabees are entities linked to the prefab
// 
// Important notes
// Prefab will automatically contain general component.
// General {"", TAG::OTHERS, SUBTAG::NOSUBTAG, true, false, this}
// The only thing initialised in this component is the prefab pointer,
// pointing to itself.
//-------------------------------------------------------------------------
class Prefab {
public:
	Prefab();
	Prefab(std::string const&);
	~Prefab();

	/*!*****************************************************************************
	Create new prefabee and add it to ECS
	
	\return Entity
	- Prefabee created
	*******************************************************************************/
	Entity CreatePrefabee();

	/*!*****************************************************************************
	Remove entity's link from prefab.
	Must be called when entity gets deleted.
	Must be called when entity gets copy assigned to another entity.
	
	\param Entity const&
	- Entity to unlink
	*******************************************************************************/
	void UnlinkPrefabee(Entity const&);

	/*!*****************************************************************************
	Add components to prefab. Must update all prefabees on newly added component.

	\param T const&
	- Add component to prefab and all it's prefabees.
	*******************************************************************************/
	template<typename T>
	void AddComponent(T const&);

	/*!*****************************************************************************
	Change component in prefab. Update all prefabees on updated component.

	\param T const&
	- Component to update in prefab and all it's prefabees.
	*******************************************************************************/
	template<typename T>
	void UpdateComponent(T const&);

	/*!*****************************************************************************
	Remove component from prefab. Remove component from all prefabee too.
	*******************************************************************************/
	template<typename T>
	void RemoveComponent();

	/*!*****************************************************************************
	Check if prefab has certain component

	\return bool
	- True if prefab has the component, otherwise false
	*******************************************************************************/
	template<typename T>
	bool HasComponent() const;

	/*!*****************************************************************************
	Get the component in the prefab.

	\return T const&
	- Component in the prefab
	*******************************************************************************/
	template<typename T>
	T const& GetComponent() const;

	/*!*****************************************************************************
	Getter for set of prefabees

	\return std::set<Entity>
	- All the prefabees tied to the prefab
	*******************************************************************************/
	std::set<Entity> const& GetPrefabees() const { return mPrefabees; }

	/*!*****************************************************************************
	Get a reference of the prefabee's name

	\return std::string&
	- Name of prefab
	*******************************************************************************/
	std::string& Name() { return mName; }

private:
	std::string mName;																// Prefab's name
	std::set<Entity> mPrefabees;											// Set of prefabees linked to the prefab
	std::array<void*, MAX_COMPONENTS> mComponents;		// Array of void pointers to prefab's component data

	/* Components (Needs to be updated)
	General*							mGeneral;
	Lifespan*							mLifespan;
	Transform*						mTransform;
	Sprite*								mSprite;
	Animation*						mAnimation;
	SheetAnimation*				mSheetAnimation;
	Physics2D*						mPhysics2D;
	RectCollider*					mRectCollider;
	CircleCollider*				mCircleCollider;
	Edge2DCollider*				mEdge2DCollider;
	Point2DCollider*			mPoint2DCollider;
	PlayerTmp*						mPlayerTmp;
	Stuff*								mStuff;
	Audio*								mAudio;
	Text*									mText;
	*/
};


//=========================================================================
// Template function declarations
//=========================================================================

//-------------------------------------------------------------------------
// ComponentArray
//-------------------------------------------------------------------------
template<typename T>
ComponentArray<T>::ComponentArray() : mData(MAX_ENTITIES) {}

template<typename T>
void ComponentArray<T>::InsertData(EntityID _entity, T _component) {
	mData.AddData(_component, static_cast<short>(_entity));
}

template<typename T>
void ComponentArray<T>::RemoveData(EntityID _entity) {
	mData.RemoveData(static_cast<short>(_entity));
}

template<typename T>
T& ComponentArray<T>::GetData(EntityID _entity) {
	return mData[static_cast<short>(_entity)];
}

template<typename T>
T const& ComponentArray<T>::ReadData(EntityID _entity) {
	return mData[static_cast<short>(_entity)];
}

template<typename T>
bool ComponentArray<T>::HasEntity(EntityID _entity) {
	return mData.CheckData(static_cast<short>(_entity));
}

//template<typename T>
//void EntityDestroyed(EntityID entity) {
//	// This function is pointless, just use RemoveData()
//}














