#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"

Entity ECS::NewEntity()
{
	Entity e = registry.create();
	e.AddComponent<General>();
	e.AddComponent<Transform>();
	return e;
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