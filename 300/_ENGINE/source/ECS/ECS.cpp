#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "ScriptingSystem.h"
#include "pch.h"
#include "Object/ObjectFactory.h"
#include "GameState/GameStateManager.h"
#include "Debug/AssertException.h"
#include "ResourceManagerTy.h"


bool Entity::ShouldRun() {
	assert(HasComponent<General>() && std::string("There is no general component when attempting to change Entity's isActive").c_str());

	General const& genComponent = GetComponent<General>();
	return !genComponent.isPaused && genComponent.isActive;
}

void Entity::Activate() {
	if (!HasComponent<General>()) {
		//LOG_ERROR("There is no general component when attempting to activate, entity ID: " + std::to_string(id));
		assert(false && std::string("There is no general component when attempting to activate, entity ID: " + std::to_string(static_cast<uint32_t>(id))).c_str());
		return;
	}
	General& genComp{ GetComponent<General>() };

	//------------------------------------------------------------------
	// Codes that should run when activating entity halfway through game

	// Scripting
#ifdef _EDITOR
	if (editorManager->IsScenePaused()) return;
#endif
	//if (!editorManager->IsScenePaused())
	if (HasComponent<Script>())
		systemManager->GetScriptingPointer()->ScriptStart(*this);


	// General
	genComp.isActive = true;

	// Parent Child
	/*if (HasComponent<)
	Entity firstChild = 

	for (Entity e : genComp.children) e.Activate();*/



	//------------------------------------------------------------------
}

void Entity::Deactivate() {
	if (!HasComponent<General>()) {
		//LOG_ERROR("There is no general component when attempting to activate, entity ID: " + std::to_string(id));
		assert(false && std::string("There is no general component when attempting to deactivate, entity ID: " + std::to_string(static_cast<uint32_t>(id))).c_str());
		return;
	}
	General& genComp{ GetComponent<General>() };

	//------------------------------------------------------------------
	// Codes that should run when activating entity halfway through game

	// Scripting
#ifdef _EDITOR
	if (editorManager->IsScenePaused()) return;
#endif
	//if (!editorManager->IsScenePaused())
	if (HasComponent<Script>())
		systemManager->GetScriptingPointer()->ScriptExit(*this);


	// General
	genComp.isActive = true;

	// Parent Child
	/*if (HasComponent<)
	Entity firstChild =

	for (Entity e : genComp.children) e.Deactivate();*/



	//------------------------------------------------------------------
}

ECS::ECS() : registry(), NullEntity(registry.create()), mClipboard(0) {} 

Entity ECS::NewEntity()
{
	Entity e = entt::to_integral(registry.create());
	e.AddComponent<General>().name = "Entity" + std::to_string(static_cast<uint32_t>(e.id));
	e.AddComponent<Transform>();
	return e;
}

void ECS::DeleteEntity(Entity e)
{
//#ifdef _DEBUG
//	assert(static_cast<std::uint32_t>(e.id) != 0);
//#endif
	if (static_cast<std::uint32_t>(e.id) == 0)
	{
		PWARNING("tried to delete entitiy with id 0");
		return;
	}
	std::string name = e.GetComponent<General>().name;
	if (e.HasParent())
		Entity(e.GetParent()).RemoveChild(e);
	if (e.HasChildren())
		for (Entity child : e.GetAllChildren())
			e.RemoveChild(child);
	if (e.HasComponent<Prefab>())
		UnlinkPrefab(e);
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
	if (static_cast<std::uint32_t>(e.id) == 0)
	{
		 PWARNING("trying to make prefab from null entity - prefab not created");
		 return;
	}
	std::string name = e.GetComponent<General>().name;
	if (mPrefabs.count(name))
	{
		PWARNING("prefab of the same name exists! - prefab not created");
		return;
	}
	ObjectFactory::SerializePrefab(e, "../assets/Prefabs/" + name + ".prefab");
	e.AddComponent<Prefab>().mPrefab = name;
	mPrefabs[name].push_back(e);
}

Entity ECS::NewEntityFromPrefab(std::string prefabName)
{
	// void ObjectFactory::DeserializeScene(const std::string& filename)
	// creation of new entity done inside deserializescene function
	Entity e(ObjectFactory::DeserializePrefab("../assets/Prefabs/" + prefabName + ".prefab", static_cast<int>(mPrefabs[prefabName].size())));
	e.AddComponent<Prefab>().mPrefab = prefabName;
	systemManager->mGameStateSystem->mCurrentGameState.mScenes[0].mEntities.insert(e);
	//copy all prefab components (except transform) to new entity
	//General temp1 = e.GetComponent<General>();
	//MeshRenderer temp = e.GetComponent<MeshRenderer>();
	mPrefabs[prefabName].push_back(e);
	PASSERT(static_cast<uint32_t>(e.id) != 0);
	return e;
}

void ECS::UpdatePrefabEntities(std::string prefabName)
{
	Entity temp(ObjectFactory::DeserializePrefab("../assets/Prefabs/" + prefabName + ".prefab", 0));
	
	for (Entity e : mPrefabs[prefabName])
	{
		if (temp.HasComponent<MeshRenderer>())
			e.AddComponent<MeshRenderer>() = temp.GetComponent<MeshRenderer>();
		if (temp.HasComponent<RigidBody>())
			e.AddComponent<RigidBody>() = temp.GetComponent<RigidBody>();
		if (temp.HasComponent<BoxCollider>())
			e.AddComponent<BoxCollider>() = temp.GetComponent<BoxCollider>();
		if (temp.HasComponent<SphereCollider>())
			e.AddComponent<SphereCollider>() = temp.GetComponent<SphereCollider>();
		if (temp.HasComponent<PlaneCollider>())
			e.AddComponent<PlaneCollider>() = temp.GetComponent<PlaneCollider>();
		if (temp.HasComponent<Scripts>())
			e.AddComponent<Scripts>() = temp.GetComponent<Scripts>();
		if (temp.HasComponent<Audio>())
			e.AddComponent<Audio>() = temp.GetComponent<Audio>();
	}

	systemManager->ecs->DeleteEntity(temp);
}

void ECS::CopyEntity(Entity e)
{
	if (static_cast<std::uint32_t>(e.id) == 0)
		PASSERT ("trying to copy null entity");
	mClipboard = e;
}

Entity ECS::StartEditPrefab(std::string prefabName)
{
	// void ObjectFactory::DeserializeScene(const std::string& filename)
	// creation of new entity done inside deserializescene function
	Entity e(ObjectFactory::DeserializePrefab("../assets/Prefabs/" + prefabName + ".prefab", static_cast<int>(mPrefabs[prefabName].size())));
	e.AddComponent<Prefab>().mPrefab = prefabName;
	//copy all prefab components (except transform) to new entity
	//General temp1 = e.GetComponent<General>();
	//MeshRenderer temp = e.GetComponent<MeshRenderer>();
	mPrefabs[prefabName].push_back(e);
	e.GetComponent<General>().name = prefabName;
	PASSERT(static_cast<uint32_t>(e.id) != 0);
	return e;
}


void ECS::EndEditPrefab(Entity e)
{
	ObjectFactory::SerializePrefab(e, "../assets/Prefabs/" + e.GetComponent<General>().name + ".prefab");
	UpdatePrefabEntities(e.GetComponent<General>().name);
	DeleteEntity(e);
}

void ECS::EndEditPrefabNoSave(Entity e)
{
	DeleteEntity(e);
}

Entity ECS::PasteEntity(int scene)
{
	if (static_cast<uint32_t>(mClipboard.id) == 0)
		return Entity(0);



	//Entity e = NewEntity();
	Entity e =systemManager->mGameStateSystem->mCurrentGameState.mScenes[scene].AddEntity();

	e.GetComponent<Transform>().mScale = Entity(mClipboard).GetComponent<Transform>().mScale;
	e.GetComponent<Transform>().mRotate = Entity(mClipboard).GetComponent<Transform>().mRotate;


	if (mClipboard.HasComponent<MeshRenderer>())
	{
		e.AddComponent<MeshRenderer>();
		e.GetComponent<MeshRenderer>() = mClipboard.GetComponent<MeshRenderer>();
		MeshRenderer& mr = e.GetComponent<MeshRenderer>();
		uid uids(mr.mMeshPath);
		mr.mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(uids.id));
		for (int i{ 0 }; i < 4; i++) {

			if (mr.mTextureCont[i] == true) {
				uid localuids(mr.mMaterialInstancePath[i]);
				mr.mTextureRef[i] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(localuids.id));
			}
		}
		GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mr.mMeshRef);
		if (meshinst->mHasAnimation)
		{
			e.AddComponent<Animator>();
			e.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst->mAnimation[0]);
		}
	}
	if (mClipboard.HasComponent<RigidBody>())
	{
		e.AddComponent<RigidBody>();
		e.GetComponent<RigidBody>() = mClipboard.GetComponent<RigidBody>();
	}
	if (mClipboard.HasComponent<BoxCollider>())
	{
		e.AddComponent<BoxCollider>();
		e.GetComponent<BoxCollider>() = mClipboard.GetComponent<BoxCollider>();
	}
	if (mClipboard.HasComponent<SphereCollider>())
	{
		e.AddComponent<SphereCollider>();
		e.GetComponent<SphereCollider>() = mClipboard.GetComponent<SphereCollider>();
	}
	if (mClipboard.HasComponent<PlaneCollider>())
	{
		e.AddComponent<PlaneCollider>();
		e.GetComponent<PlaneCollider>() = mClipboard.GetComponent<PlaneCollider>();
	}
	if (mClipboard.HasComponent<Scripts>())
	{
		e.AddComponent<Scripts>();
		e.GetComponent<Scripts>() = mClipboard.GetComponent<Scripts>();
	}
	if (mClipboard.HasComponent<Audio>())
	{
		e.AddComponent<Audio>();
		e.GetComponent<Audio>() = mClipboard.GetComponent<Audio>();
	}

	return e;
}



void ECS::UnlinkPrefab(Entity e)
{
	if (!e.HasComponent<Prefab>())
		return;
	std::string name = e.GetComponent<Prefab>().mPrefab;
	auto temp = std::find(mPrefabs[name].begin(), mPrefabs[name].end(), e);
	if (temp == mPrefabs[name].end())
		return;
	mPrefabs[name].erase(temp);
	e.RemoveComponent<Prefab>();
}

Entity::Entity(entt::entity id) : id(id) {}
Entity::Entity(std::uint32_t id) : id(entt::entity(id)) {}
Entity::Entity(const Entity& entity) : id(entity.id) {}

void Entity::operator=(const Entity& entity)
{
	this->id = entity.id;
}

void Entity::AddChild(Entity e)
{
	if (this->id == e.id)
	{
		PWARNING("tried to add entity as child to itself");
		return;
	}
	if (static_cast<std::uint32_t>(e.id) == 0)
	{
		PWARNING("tried to add null entity as child");
		return;
	}
	if (static_cast<std::uint32_t>(this->id) == 0)
	{
		PWARNING("tried to add child to null entity");
		return;
	}
	if (e.HasComponent<Parent>())
	{
		PWARNING("tried to add child with parent to another parent");
		return;
	}
	if (e.HasComponent<Prefab>())
		systemManager->ecs->UnlinkPrefab(e);
	if (this->HasComponent<Prefab>())
		systemManager->ecs->UnlinkPrefab(*this);
	
	std::uint32_t eID = static_cast<std::uint32_t>(e.id);
	Children& children = this->HasComponent<Children>() ? this->GetComponent<Children>() : this->AddComponent<Children>();
	Parent& parent = e.HasComponent<Parent>() ? e.GetComponent<Parent>() : e.AddComponent<Parent>();
	
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

void Entity::RemoveChild(Entity e)
{
	if (!this->HasComponent<Children>())
		return;
	if (!e.HasComponent<Parent>())
		return;
	if (e.GetComponent<Parent>().mParent != static_cast<uint32_t>(this->id))
		return;
	
	Children& thisChildren = this->GetComponent<Children>();
	if (thisChildren.mNumChildren == 1)
	{
		e.RemoveComponent<Parent>();
		this->RemoveComponent<Children>();
		return;
	}
	--thisChildren.mNumChildren;
	Parent& eParent = e.GetComponent<Parent>();
	Entity prev = eParent.mPrevSibling;
	Entity next = eParent.mNextSibling;
	prev.GetComponent<Parent>().mNextSibling = static_cast<uint32_t>(next.id);
	next.GetComponent<Parent>().mPrevSibling = static_cast<uint32_t>(prev.id);
	e.RemoveComponent<Parent>();
}
