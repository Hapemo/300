#include "ECS.h"

Entity ECS::NewEntity()
{
	return { registry.create() };
}

void ECS::DeleteEntity(Entity e)
{
	registry.destroy(e.id);
}

Entity::Entity(entt::entity id) : id(id) {}