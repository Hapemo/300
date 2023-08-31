#pragma once
#include "entt.hpp";

struct Entity;

class ECS 
{
public:
	static entt::registry registry;

	static Entity NewEntity();

	static void DeleteEntity(Entity e);

	template <typename Component, typename OtherComponent, typename ...Components>
	static auto GetEntitiesWith();

	template <typename Component>
	static auto GetEntitiesWith();

	static void DeleteAllEntities();
};

struct Entity
{
	entt::entity id;

	Entity() = delete;
	Entity(entt::entity id);

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
	return ECS::registry.emplace_or_replace<Component>(id, Component());
}

template <typename Component>
Component& Entity::AddComponent(const Component& component)
{
	return ECS::registry.emplace_or_replace<Component>(id, component);
}

template <typename Component>
Component& Entity::GetComponent()
{
	return ECS::registry.get_or_emplace<Component>(id, Component());
}

template <typename Component, typename OtherComponent, typename ...Components>
auto Entity::GetComponent()
{
	return ECS::registry.get<Component, OtherComponent, Components...>(id);
}

template <typename Component>
bool Entity::HasComponent()
{
	return ECS::registry.all_of<Component>(id);
}

template <typename ... Components>
bool Entity::HasAnyOfComponents()
{
	return ECS::registry.any_of<Components...>(id);
}

template <typename ... Components>
bool Entity::HasAllOfComponents()
{
	return ECS::registry.all_of<Components...>(id);
}

template <typename Component>
void Entity::RemoveComponent()
{
	ECS::registry.remove<Component>(id);
}
