#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "pch.h"
#include "Object/ObjectFactory.h"

bool Entity::ShouldRun() {
	assert(HasComponent<General>() && std::string("There is no general component when attempting to change Entity's isActive").c_str());

	General const& genComponent = GetComponent<General>();
	return !genComponent.isPaused && genComponent.isActive;
}

ECS::ECS() : registry(), NullEntity(registry.create()) {} 

Entity ECS::NewEntity()
{
	Entity e = registry.create();
	e.AddComponent<General>().name = "Entity" + static_cast<uint32_t>(e.id);
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

void ECS::NewPrefab(Entity e)
{

	// void ObjectFactory::SerializePrefab(Entity e, const std::string& filename) call here
	// store to resources/prefabs/e.getcomponent<general>().name
	// general flow 
	// 1) serialize prefab based on entity
	// 2) have a list of entities tied to a prefab
	// for now, use member map
	std::string name = e.GetComponent<General>().name;
	ObjectFactory::SerializePrefab(e, "../assets/Prefabs/" + name + ".json");
	if (mPrefabs.count(name))
		throw ("prefab of the same name exists!");
	mPrefabs[name].push_back(e);
}

void ECS::NewEntityFromPrefab(std::string prefabName)
{
	// void ObjectFactory::DeserializeScene(const std::string& filename)
	// creation of new entity done inside deserializescene function
	Entity e = ObjectFactory::DeserializePrefab("../assets/Prefabs/" + prefabName + ".json");
	//copy all prefab components (except transform) to new entity
	mPrefabs[prefabName].push_back(e);
}

void ECS::UpdatePrefabEntities(std::string prefabName)
{
	for (Entity e : mPrefabs[prefabName])
	{
		//copy all prefab components into new entity
	}
}

Entity::Entity(entt::entity id) : id(id) {}
Entity::Entity(std::uint32_t id) : id(entt::entity(id)) {}
Entity::Entity(const Entity& entity) : id(entity.id) {}

void Entity::AddChild(Entity e)
{
	if (this->id == e.id)
		throw ("trying to make entity reproduce asexually");
	if (static_cast<std::uint32_t>(e.id) == 0)
		throw ("trying to add null entity as child");
	if (static_cast<std::uint32_t>(this->id) == 0)
		throw ("trying to add child to null entity");
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

std::vector<Entity> Entity::GetAllChildren()
{
	if (!this->HasComponent<Children>())
		return {};
	Children child = this->GetComponent<Children>();
	std::vector<Entity> children;
	children.push_back(child.mFirstChild);
	Parent parent = Entity(child.mFirstChild).GetComponent<Parent>();
	while (parent.mNextSibling != child.mFirstChild)
	{
		children.push_back(parent.mNextSibling);
		parent = Entity(parent.mNextSibling).GetComponent<Parent>();
	}
	return children;
}

Entity Entity::GetParent()
{
	if (!this->HasComponent<Parent>())
		return 0;
	return this->GetComponent<Parent>().mParent;
}

bool Entity::HasChildren() 
{
	return this->HasComponent<Children>();
}

bool Entity::HasParent()
{
	return this->HasComponent<Parent>();
}