#pragma once
#include "entt.hpp"
#include "Singleton.h"

using Entity = entt::entity;

class ECS
{
	entt::registry registry;
public:
	ECS() = default;

	Entity NewEntity();

	template <typename T>
	T& AddComponent(Entity e, T component);

	template <typename T>
	T& GetComponent(Entity e);

	template <typename T>
	auto GetView();

	template <class T, class ... Ts>
	auto GetGroup();
};

template <typename T>
T& ECS::AddComponent(Entity e, T component)
{
	return registry.emplace_or_replace<T>(e, component);
}

template <typename T>
T& ECS::GetComponent(Entity e)
{
	return registry.get_or_emplace<T>(e, T());
}

template <typename T>
auto ECS::GetView()
{
	return registry.view<T>();
}

template <class T, class ... Ts>
auto ECS::GetGroup()
{
	return registry.group<T>(entt::get<Ts>);
}