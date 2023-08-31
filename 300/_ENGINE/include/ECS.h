#pragma once
#include "Singleton.h"
#include "entt.hpp";

struct Entity;

class ECS : public Singleton<ECS>
{
public:
	ECS() = default;
	entt::registry registry;

	Entity NewEntity();

	void DeleteEntity(Entity e);

	template <typename Component, typename OtherComponent, typename ...Components>
	auto GetEntitiesWith();

	template <typename Component>
	auto GetEntitiesWith();

	void DeleteAllEntities();
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
	return ECS::GetInstance()->registry.emplace_or_replace<Component>(id, Component());
}

template <typename Component>
Component& Entity::AddComponent(const Component& component)
{
	return ECS::GetInstance()->registry.emplace_or_replace<Component>(id, component);
}

template <typename Component>
Component& Entity::GetComponent()
{
	return ECS::GetInstance()->registry.get_or_emplace<Component>(id, Component());
}

template <typename Component, typename OtherComponent, typename ...Components>
auto Entity::GetComponent()
{
	return ECS::GetInstance()->registry.get<Component, OtherComponent, Components...>(id);
}

template <typename Component>
bool Entity::HasComponent()
{
	return ECS::GetInstance()->registry.all_of<Component>(id);
}

template <typename ... Components>
bool Entity::HasAnyOfComponents()
{
	return ECS::GetInstance()->registry.any_of<Components...>(id);
}

template <typename ... Components>
bool Entity::HasAllOfComponents()
{
	return ECS::GetInstance()->registry.all_of<Components...>(id);
}

template <typename Component>
void Entity::RemoveComponent()
{
	ECS::GetInstance()->registry.remove<Component>(id);
}
