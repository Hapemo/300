#pragma once
#include "entt.hpp";
#include "Singleton.h"
#include "ECS_Systems.h"
//#include "Prefab.h"

struct Entity {
	entt::entity id;

	Entity() = delete;
	Entity(entt::entity id);
	Entity(std::uint32_t id);

	Entity(const Entity& entity);
	void operator=(const Entity& entity);

	bool operator<(Entity e) { return id < e.id; }
	bool operator<(const Entity e) const { return id < e.id; }

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
};

class ECS
{
public:
	ECS();

	entt::registry registry;

	Entity NewEntity();

	void DeleteEntity(Entity e);

	template <typename Component, typename OtherComponent, typename ...Components>
	auto GetEntitiesWith();

	template <typename Component>
	auto GetEntitiesWith();

	void DeleteAllEntities();

	//Entity NewPrefabEntity(const Prefab& prefab);

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
	return systemManager->ecs->registry.emplace_or_replace<Component>(id, Component());
}

template <typename Component>
Component& Entity::AddComponent(const Component& component)
{
	return systemManager->ecs->registry.emplace_or_replace<Component>(id, component);
}

template <typename Component>
Component& Entity::GetComponent()
{
	return systemManager->ecs->registry.get_or_emplace<Component>(id, Component());
}

template<typename Component>
const Component& Entity::GetComponent() const
{
	return systemManager->ecs->registry.get_or_emplace<Component>(id, Component());
}

template <typename Component, typename OtherComponent, typename ...Components>
auto Entity::GetComponent()
{
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
