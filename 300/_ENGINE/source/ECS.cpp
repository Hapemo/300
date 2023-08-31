#include "ECS.h"

Entity ECS::NewEntity()
{
	return { registry.create() };
}

void ECS::DeleteEntity(Entity e)
{
	registry.destroy(e.id);
}

void ECS::DeleteAllEntities()
{
	registry.clear();
}

Entity::Entity(entt::entity id) : id(id) {}