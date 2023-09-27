#pragma once
#include "entt.hpp";
#include "ECS_Systems.h"
#include "Debug/AssertException.h"

struct Children;
struct Parent;

struct Entity
{
	entt::entity id;

	Entity() = delete;
	Entity(entt::entity id);
	Entity(std::uint32_t id);

	Entity(const Entity& entity);
	void operator=(const Entity& entity);

	bool operator<(Entity e) { return id < e.id; }
	bool operator<(const Entity e) const { return id < e.id; }
	bool operator==(Entity e) { return id == e.id; }
	bool operator==(const Entity e) const { return id == e.id; }

	template <typename Component>
	Component& AddComponent();

	template <typename Component>
	Component& AddComponent(const Component& component);

	template <typename Component>
	Component& GetComponent();

	template <typename Component>
	const Component& GetComponent() const;

	template <typename Component, typename OtherComponent, typename ...Components>
	auto GetComponent();

	template <typename Component>
	bool HasComponent() const;

	template <typename ... Components>
	bool HasAnyOfComponents() const;

	template <typename ... Components>
	bool HasAllOfComponents() const;

	template <typename Component>
	void RemoveComponent();

	bool ShouldRun();

	void Activate();

	void Deactivate();

	void AddChild(Entity e);

	std::vector<Entity> GetAllChildren();

	Entity GetParent();

	bool HasChildren();

	bool HasParent();

	void RemoveChild(Entity e);

	template<typename Component>
	Component& LuaGetComponent(Entity entity);

	//template <typename Component, typename OtherComponent, typename ...Components>
	/*auto LuaGetComponents(Entity entity);*/

	//template <typename Component, typename OtherComponent, typename ...Components>
	//auto LuaGetComponents(Entity entity, sol::as_args args);
};

class ECS
{
public:
	ECS();

	std::unordered_map<std::string, std::vector<Entity>> mPrefabs;

	Entity mClipboard;

	entt::registry registry;

	Entity NewEntity();

	void DeleteEntity(Entity e);

	template <typename Component, typename OtherComponent, typename ...Components>
	auto GetEntitiesWith();

	template <typename Component>
	auto GetEntitiesWith();

	void DeleteAllEntities();

	void NewPrefab(Entity e);

	Entity NewEntityFromPrefab(std::string prefabName);

	void UpdatePrefabEntities(std::string prefabName);

	void UnlinkPrefab(Entity e);

	void CopyEntity(Entity e);

	Entity StartEditPrefab(std::string prefabName);

	void EndEditPrefab(Entity e); //saves
	
	void EndEditPrefabNoSave(Entity e); //no save


	Entity PasteEntity();

	const Entity NullEntity;
};



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
	PASSERT(static_cast<std::uint32_t>(this->id) != 0, "Tried to add component from null entity! e.id = 0");
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
	PASSERT(static_cast<std::uint32_t>(this->id) != 0, "Tried to add component from null entity! e.id = 0");
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
	PASSERT(static_cast<std::uint32_t>(this->id) != 0, "Tried to get component from null entity! e.id = 0");
	PASSERT(this->HasComponent<Component>(), "no such component in entity");
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
	PASSERT(static_cast<std::uint32_t>(this->id) != 0, "Tried to get component from null entity! e.id = 0");
	return systemManager->ecs->registry.get_or_emplace<Component>(id, Component());
}

template <typename Component, typename OtherComponent, typename ...Components>
auto Entity::GetComponent()
{
//#ifdef _DEBUG
//	assert(static_cast<std::uint32_t>(this->id) != 0);
//#else
//	if (static_cast<std::uint32_t>(this->id) == 0)
//		return auto();
//#endif
	PASSERT(static_cast<std::uint32_t>(this->id) != 0, "Tried to get component from null entity! e.id = 0");
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

template<typename Component>
Component& Entity::LuaGetComponent(Entity entity)
{
	return entity.GetComponent<Component>();
}

//template <typename Component, typename OtherComponent, typename ...Components>
//auto Entity::LuaGetComponents(Entity entity)
//{
//	return entity.GetComponent<Component, OtherComponent, Components...>;
//}
