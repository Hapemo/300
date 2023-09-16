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
Entity::Entity(std::uint32_t id) : id(entt::entity(id)) {}

Entity::Entity(const Entity& entity)
{
	id = systemManager->ecs->NewEntity().id;

	// temp, change after rttr is done
	if (entity.HasComponent<General>())
		this->AddComponent<General>(entity.GetComponent<General>());
	if (entity.HasComponent<Transform>())
		this->AddComponent<Transform>(entity.GetComponent<Transform>());
	if (entity.HasComponent<RigidBody>())
		this->AddComponent<RigidBody>(entity.GetComponent<RigidBody>());
	if (entity.HasComponent<BoxCollider>())
		this->AddComponent<BoxCollider>(entity.GetComponent<BoxCollider>());
	if (entity.HasComponent<SphereCollider>())
		this->AddComponent<SphereCollider>(entity.GetComponent<SphereCollider>());
	if (entity.HasComponent<PlaneCollider>())
		this->AddComponent<PlaneCollider>(entity.GetComponent<PlaneCollider>());
	if (entity.HasComponent<Scripts>())
		this->AddComponent<Scripts>(entity.GetComponent<Scripts>());
}

void Entity::operator=(const Entity& entity)
{
	systemManager->ecs->registry.destroy(id);
	id = systemManager->ecs->NewEntity().id;

	if (entity.HasComponent<General>())
		this->AddComponent<General>(entity.GetComponent<General>());
	if (entity.HasComponent<Transform>())
		this->AddComponent<Transform>(entity.GetComponent<Transform>());
	if (entity.HasComponent<RigidBody>())
		this->AddComponent<RigidBody>(entity.GetComponent<RigidBody>());
	if (entity.HasComponent<BoxCollider>())
		this->AddComponent<BoxCollider>(entity.GetComponent<BoxCollider>());
	if (entity.HasComponent<SphereCollider>())
		this->AddComponent<SphereCollider>(entity.GetComponent<SphereCollider>());
	if (entity.HasComponent<PlaneCollider>())
		this->AddComponent<PlaneCollider>(entity.GetComponent<PlaneCollider>());
	if (entity.HasComponent<Scripts>())
		this->AddComponent<Scripts>(entity.GetComponent<Scripts>());
}
