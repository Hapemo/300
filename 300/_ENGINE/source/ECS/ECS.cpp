#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "pch.h"

bool Entity::ShouldRun() {
	assert(HasComponent<General>() && std::string("There is no general component when attempting to change Entity's isActive").c_str());

	General const& genComponent = GetComponent<General>();
	return !genComponent.isPaused && genComponent.isActive;
}

ECS::ECS() : registry(), NullEntity(registry.create()) {} 

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

//for (string str : component list)
//	if (e.hascom<rttr(stR)>())
//		e.getcom<rttr(str)>()