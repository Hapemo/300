/*!*****************************************************************************
\file ECS.h
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Pepe Production
\date 15/08/2023
\brief
ECS functions for creating entities, parent child relationships
and prefabs.
*******************************************************************************/
#pragma once
#include "entt.hpp"
#include "ECS_Systems.h"
#include "Debug/AssertException.h"
#include <vector>
#include <string>

struct Children;
struct Parent;
struct Transform;

#define EXPAND(...) __VA_ARGS__ 

#define ALL_COMPONENTS EXPAND(MeshRenderer, RigidBody, BoxCollider, SphereCollider, CapsuleCollider, Scripts, Audio) 

struct Entity
{
	entt::entity id;

	Entity() : id(entt::null) {};

	/*!*****************************************************************************
	Copy and conversion constructors.
	*******************************************************************************/
	Entity(entt::entity id);
	Entity(std::uint32_t id);
	Entity(const Entity& entity);

	/*!*****************************************************************************
	Operator overloads.
	*******************************************************************************/
	void operator=(const Entity& entity);
	bool operator<(Entity e) { return id < e.id; }
	bool operator<(const Entity e) const { return id < e.id; }
	bool operator==(Entity e) { return id == e.id; }
	bool operator==(const Entity e) const { return id == e.id; }

	/*!*****************************************************************************
	Adds or replaces a component in an entity and returns it.
	*******************************************************************************/
	template <typename Component>
	Component& AddComponent();
	template <typename Component>
	Component& AddComponent(const Component& component);

	/*!*****************************************************************************
	Retrieves a component from an entity.
	*******************************************************************************/
	template <typename Component>
	Component& GetComponent();
	template <typename Component>
	const Component& GetComponent() const;

	/*!*****************************************************************************
	Retrieves multiple components from an entity.
	*******************************************************************************/
	template <typename Component, typename OtherComponent, typename ...Components>
	auto GetComponents();

	/*!*****************************************************************************
	Checks if an entity contains any component.
	*******************************************************************************/
	template <typename Component>
	bool HasComponent() const;
	template <typename ... Components>
	bool HasAnyOfComponents() const;
	template <typename ... Components>
	bool HasAllOfComponents() const;

	/*!*****************************************************************************
	Removes a component from an entity.
	*******************************************************************************/
	template <typename Component>
	void RemoveComponent();

	/*!*****************************************************************************
	Checks if the entity should be updated based on the scene status.
	*******************************************************************************/
	bool ShouldRun();

	/*!*****************************************************************************
	Activates an entity so it runs.
	*******************************************************************************/
	void Activate();

	/*!*****************************************************************************
	Deactivates an entity so it does not run.
	*******************************************************************************/
	void Deactivate();

	/*!*****************************************************************************
	Adds a child to an entity.
	*******************************************************************************/
	void AddChild(Entity e);

	/*!*****************************************************************************
	Returns all children of an entity.
	*******************************************************************************/
	std::vector<Entity> GetAllChildren();

	/*!*****************************************************************************
	Retrieves the parent of an entity if any.
	*******************************************************************************/
	Entity GetParent();

	/*!*****************************************************************************
	Checks if an entity has any children.
	*******************************************************************************/
	bool HasChildren();

	/*!*****************************************************************************
	Checks if an entity has a parent.
	*******************************************************************************/
	bool HasParent();

	/*!*****************************************************************************
	Removes a child from a parent entity.
	*******************************************************************************/
	void RemoveChild(Entity e);
};

class ECS
{
public:
	/*!*****************************************************************************
	Constructor.
	*******************************************************************************/
	ECS();

	std::unordered_map<std::string, std::vector<Entity>> mPrefabs;

	Entity mClipboard;

	entt::registry registry;

	static void AddTag(const std::string& tag);

	static std::string GetTag(unsigned char id);

	static unsigned char GetTag(const std::string& tag);

	/*!*****************************************************************************
	Creates a new entity.
	*******************************************************************************/
	Entity NewEntity();

	/*!*****************************************************************************
	Creates a new entity and assign it to the correct scene.
	*******************************************************************************/
	Entity NewEntityByScene();

	/*!*****************************************************************************
	Deletes an entity and all its components.
	*******************************************************************************/
	void DeleteEntity(Entity e);

	/*!*****************************************************************************
	Returns entites that contain specific components.
	*******************************************************************************/
	template <typename Component, typename OtherComponent, typename ...Components>
	auto GetEntitiesWith();
	template <typename Component>
	auto GetEntitiesWith();

	/*!*****************************************************************************
	Cleanup.
	*******************************************************************************/
	void DeleteAllEntities();

	/*!*****************************************************************************
	Scripts will only call this delete entity function, which is to set
	isDelete of entity in general component to true, indicating that it should
	be delete later.
	*******************************************************************************/
	void SetDeleteEntity(Entity e);

	/*!*****************************************************************************
	This function is the follow up after SetDeleteEntity is called in scripts.
	This function will be called after all systems had updated under systemManager
	update to make sure that the intended deleted entity is deleted at the end 
	of the update loop.
	*******************************************************************************/
	void DeleteEntityUpdate();

	/*!*****************************************************************************
	Creates a prefab from an entity.
	*******************************************************************************/
	void NewPrefab(Entity e);

	/*!*****************************************************************************
	Creates an entity from an existing prefab.
	*******************************************************************************/
	Entity NewEntityFromPrefab(std::string prefabName, const glm::vec3& pos);
	Entity NewEntityFromPrefabScene(std::string prefabName, int prefabscene, const glm::vec3& pos);
	Entity NewEntityFromPrefabSceneName(std::string prefabName, std::string, const glm::vec3& pos);
	Entity NewEntityPrefabForGraph(std::string prefabName, const glm::vec3& pos);
	/*!*****************************************************************************
	Updates all entities tied to a specific prefab, to be called when the prefab is updated.
	*******************************************************************************/
	void UpdatePrefabEntities(std::string prefabName);

	/*!*****************************************************************************
	Unlinks an entity from a prefab, causing updates made to the prefab not affect this entity anymore.
	*******************************************************************************/
	void UnlinkPrefab(Entity e);

	/*!*****************************************************************************
	Copies an entity to the clipboard.
	*******************************************************************************/
	void CopyEntity(Entity e);

	/*!*****************************************************************************
	Pastes an entity from the clipboard.
	*******************************************************************************/
	Entity PasteEntity(int);

	/*!*****************************************************************************
	Creates a dummy entity for editing prefabs.
	*******************************************************************************/
	Entity StartEditPrefab(std::string prefabName);

	/*!*****************************************************************************
	Destroys the dummy entity, and updates all entities that are linked to the prefab.
	*******************************************************************************/
	void EndEditPrefab(Entity e);

	/*!*****************************************************************************
	Destroys the dummy entity, and DOES NOT updates all entities that are linked to the prefab.
	*******************************************************************************/
	void EndEditPrefabNoSave(Entity e);

	/*!*****************************************************************************
	Destructor.
	*******************************************************************************/
	~ECS();

	template <typename T, typename ...Ts>
	void AddComponentHelper(Entity e, Entity temp);

	template <typename T, typename ...Ts>
	void RemoveComponentHelper(Entity e);

	int SelectedScene;

	static std::vector<std::string> mEntityTags;
};

template <typename T, typename ...Ts>
void ECS::AddComponentHelper(Entity e, Entity temp) {
	if (temp.HasComponent<T>())
		e.AddComponent<T>() = temp.GetComponent<T>();

	if constexpr (sizeof...(Ts) > 0) {
		AddComponentHelper<Ts...>(e, temp);
	}
}

template <typename T, typename ...Ts>
void ECS::RemoveComponentHelper(Entity e) {
	if (e.HasComponent<T>())
		e.RemoveComponent<T>();

	if constexpr (sizeof...(Ts) > 0) {
		RemoveComponentHelper<Ts...>(e);
	}
}


template <typename Component>
auto ECS::GetEntitiesWith()
{
	return registry.view<Component>();
}

template <typename Component, typename OtherComponent, typename ...Components>
auto ECS::GetEntitiesWith()
{
	return registry.group<Component>(entt::get<OtherComponent, Components...>);
}

template <typename Component>
Component& Entity::AddComponent()
{
	//#ifdef _DEBUG
	//	assert(static_cast<std::uint32_t>(this->id) != 0);
	//#else
	//	if (static_cast<std::uint32_t>(this->id) == 0)
	//		return Component();
	//#endif
	PASSERT((static_cast<std::uint32_t>(this->id) != 0, "Tried to add component from null entity! e.id = 0"));
	return systemManager->ecs->registry.emplace_or_replace<Component>(id, Component());
}

template <typename Component>
Component& Entity::AddComponent(const Component& component)
{
	//#ifdef _DEBUG
	//	assert(static_cast<std::uint32_t>(this->id) != 0);
	//#else
	//	if (static_cast<std::uint32_t>(this->id) == 0)
	//		return Component();
	//#endif
	PASSERT((static_cast<std::uint32_t>(this->id) != 0, "Tried to add component from null entity! e.id = 0"));
	return systemManager->ecs->registry.emplace_or_replace<Component>(id, component);
}

template <typename Component>
Component& Entity::GetComponent()
{
	//#ifdef _DEBUG
	//	assert(static_cast<std::uint32_t>(this->id) != 0);
	//#else
	//	if (static_cast<std::uint32_t>(this->id) == 0)
	//		return Component();
	//#endif
	PASSERT((static_cast<std::uint32_t>(this->id) != 0, "Tried to get component from null entity! e.id = 0"));
	PASSERT((this->HasComponent<Component>(), "no such component in entity"));
	return systemManager->ecs->registry.get_or_emplace<Component>(id, Component());
}

template<typename Component>
const Component& Entity::GetComponent() const
{
	//#ifdef _DEBUG
	//	assert(static_cast<std::uint32_t>(this->id) != 0);
	//#else
	//	if (static_cast<std::uint32_t>(this->id) == 0)
	//		return Component();
	//#endif
	PASSERT((static_cast<std::uint32_t>(this->id) != 0, "Tried to get component from null entity! e.id = 0"));
	return systemManager->ecs->registry.get_or_emplace<Component>(id, Component());
}

template <typename Component, typename OtherComponent, typename ...Components>
auto Entity::GetComponents()
{
	//#ifdef _DEBUG
	//	assert(static_cast<std::uint32_t>(this->id) != 0);
	//#else
	//	if (static_cast<std::uint32_t>(this->id) == 0)
	//		return auto();
	//#endif
	PASSERT((static_cast<std::uint32_t>(this->id) != 0, "Tried to get component from null entity! e.id = 0"));
	return systemManager->ecs->registry.get<Component, OtherComponent, Components...>(id);
}

template <typename Component>
bool Entity::HasComponent() const
{
	return systemManager->ecs->registry.all_of<Component>(id);
}

template <typename ... Components>
bool Entity::HasAnyOfComponents() const
{
	return systemManager->ecs->registry.any_of<Components...>(id);
}

template <typename ... Components>
bool Entity::HasAllOfComponents() const
{
	return systemManager->ecs->registry.all_of<Components...>(id);
}

template <typename Component>
void Entity::RemoveComponent()
{
	systemManager->ecs->registry.remove<Component>(id);
}

//template<typename Component>
//Component& Entity::LuaGetComponent(Entity entity)
//{
//	return entity.GetComponent<Component>();
//}

//template <typename Component, typename OtherComponent, typename ...Components>
//auto Entity::LuaGetComponents(Entity entity)
//{
//	return entity.GetComponent<Component, OtherComponent, Components...>;
//}
