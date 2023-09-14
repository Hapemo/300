#pragma once
#include "entt.hpp";
#include "Singleton.h"
#include "ECS_Systems.h"

struct Entity {
	entt::entity id;

	Entity() = delete;
	Entity(entt::entity id);

	bool operator<(Entity e) { return id < e.id; }
	bool operator<(const Entity e) const { return id < e.id; }

	template <typename Component>
	Component& AddComponent();

	template <typename Component>
	Component& AddComponent(const Component& component);

	template <typename Component>
	Component& GetComponent();

	template <typename Component, typename OtherComponent, typename ...Components>
	auto GetComponent();

	template <typename Component>
	bool HasComponent();

	template <typename ... Components>
	bool HasAnyOfComponents();

	template <typename ... Components>
	bool HasAllOfComponents();

	template <typename Component>
	void RemoveComponent();
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

template <typename Component, typename OtherComponent, typename ...Components>
auto Entity::GetComponent()
{
	return systemManager->ecs->registry.get<Component, OtherComponent, Components...>(id);
}

template <typename Component>
bool Entity::HasComponent()
{
	return systemManager->ecs->registry.all_of<Component>(id);
}

template <typename ... Components>
bool Entity::HasAnyOfComponents()
{
	return systemManager->ecs->registry.any_of<Components...>(id);
}

template <typename ... Components>
bool Entity::HasAllOfComponents()
{
	return systemManager->ecs->registry.all_of<Components...>(id);
}

template <typename Component>
void Entity::RemoveComponent()
{
	systemManager->ecs->registry.remove<Component>(id);
}
