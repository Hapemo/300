#include "ECS.h"
#include "Physics/PhysicsSystem.h"

entt::registry ECS::registry;

Entity ECS::NewEntity()
{
	return { ECS::registry.create() };
}

void ECS::DeleteEntity(Entity e)
{
	ECS::registry.destroy(e.id);
}

void ECS::DeleteAllEntities()
{
	ECS::registry.clear();
}

Entity::Entity(entt::entity id) : id(id) {}