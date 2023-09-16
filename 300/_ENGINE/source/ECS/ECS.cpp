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
//#ifdef _DEBUG
//	assert(static_cast<std::uint32_t>(e.id) != 0);
//#endif
	if (static_cast<std::uint32_t>(e.id) == 0)
		throw ("tried to delete entitiy with id 0");
	registry.destroy(e.id);
}

void ECS::DeleteAllEntities()
{
	registry.clear();
}

Entity::Entity(entt::entity id) : id(id) 
{ 
//#ifdef _DEBUG
//	assert(static_cast<std::uint32_t>(this->id) != 0); 
//#endif
	if (static_cast<std::uint32_t>(this->id) == 0)
		throw ("tired to construct entitiy with id 0");
}
Entity::Entity(std::uint32_t id) : id(entt::entity(id)) 
{ 
//#ifdef _DEBUG
//	assert(static_cast<std::uint32_t>(this->id) != 0);
//#endif
	if (static_cast<std::uint32_t>(this->id) == 0)
		throw ("tired to construct entitiy with id 0");
}

Entity::Entity(const Entity& entity)
{
//#ifdef _DEBUG
//	assert(static_cast<std::uint32_t>(entity.id) != 0);
//#endif
	if (static_cast<std::uint32_t>(entity.id) == 0)
		throw ("tried to copy null entity to entity");

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
//#ifdef _DEBUG
//	assert(static_cast<std::uint32_t>(entity.id) != 0);
//#endif
	if (static_cast<std::uint32_t>(entity.id) == 0)
		throw ("tried to assign null entity to entity");

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

Children& Entity::AddChild(Entity e)
{
	if (e.HasComponent<Parent>())
		throw ("entity is the child of another!");
	
	std::uint32_t eID = static_cast<std::uint32_t>(e.id);
	Children& children = this->GetComponent<Children>();
	Parent& parent = e.GetComponent<Parent>();
	
	++children.mNumChildren;
	parent.mParent = static_cast<std::uint32_t>(this->id);
	if (children.mNumChildren == 1) // this entity is a new parent
	{
		parent.mNextSibling = parent.mPrevSibling = children.mFirstChild = eID;
		return;
	}
	// entity is already a parent of another
	Entity firstBorn(children.mFirstChild);
	Entity lastBorn(firstBorn.GetComponent<Parent>().mPrevSibling);
	firstBorn.GetComponent<Parent>().mPrevSibling = eID;
	lastBorn.GetComponent<Parent>().mNextSibling = eID;
	parent.mNextSibling = static_cast<uint32_t>(firstBorn.id);
	parent.mPrevSibling = static_cast<uint32_t>(lastBorn.id);
}
