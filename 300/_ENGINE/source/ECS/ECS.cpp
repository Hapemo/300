#include "pch.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "ECS/ECS_Systems.h"
#include "Physics/PhysicsSystem.h"

#include "ScriptingSystem.h"
#include "Object/ObjectFactory.h"
#include "GameState/GameStateManager.h"
#include "Debug/AssertException.h"
#include "ResourceManagerTy.h"
#include "AI/AIManager.h"

std::vector<std::string> ECS::mEntityTags({ "PLAYER", "ENEMY", "BULLET", "FLOOR", "WALL", "TELEPORTER", "UI"});

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

	// General
	genComp.isActive = false;

	// Parent Child
	/*if (HasComponent<)
	Entity firstChild =

	for (Entity e : genComp.children) e.Deactivate();*/



	//------------------------------------------------------------------
}

ECS::ECS() : registry(), mClipboard(0) { PASSERT(static_cast<int>(registry.create()) == 0); }

void ECS::AddTag(const std::string& tag)
{
	std::string temp = tag;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
	mEntityTags.push_back(temp);
}

std::string ECS::GetTag(unsigned char id)
{
	if (id < 0 || id >= mEntityTags.size())
	{
		PWARNING("Tag id is out of vector bounds!")
		return "";
	}
	return mEntityTags[id];
}

unsigned char ECS::GetTag(const std::string& tag)
{
	std::string temp = tag;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
	for (unsigned char i = 0; i < (unsigned char)mEntityTags.size(); ++i)
		if (mEntityTags[i] == temp)
			return i;
	PWARNING("Tag not found!")
	return 0;
}

Entity ECS::NewEntity()
{
	Entity e = entt::to_integral(registry.create());
	e.AddComponent<General>().name = "Entity" + std::to_string(static_cast<uint32_t>(e.id));
	e.AddComponent<Transform>();
	return e;
}

Entity ECS::NewEntityByScene()
{
	auto& allScene = systemManager->mGameStateSystem->mCurrentGameState.mScenes;

	if (allScene.size() <= 0) {
		systemManager->mGameStateSystem->mCurrentGameState.AddScene("NewScene");
		Entity newEntity = allScene[0].AddEntity();
		return newEntity;
	}
	else {
		Entity newEntity = allScene[SelectedScene].AddEntity();
		return newEntity;
	}
}

void ECS::DeleteEntity(Entity e)
{
//#ifdef _DEBUG
//	assert(static_cast<std::uint32_t>(e.id) != 0);
//#endif
	if (static_cast<std::uint32_t>(e.id) == 0)
	{
		PWARNING("tried to delete entity with id 0");
		return;
	}
	if (e.HasParent())
		Entity(e.GetParent()).RemoveChild(e);
	if (e.HasChildren())
		for (Entity child : e.GetAllChildren())
			e.RemoveChild(child);
	if (e.HasComponent<Prefab>())
		UnlinkPrefab(e);
	systemManager->mAISystem->RemoveAIFromEntity(e);
	systemManager->mPhysicsSystem->RemoveActor(e);
	registry.destroy(e.id);
}

void ECS::DeleteAllEntities()
{
	registry.clear();
}

void ECS::SetDeleteEntity(Entity e)
{
	e.GetComponent<General>().isDelete = true;
	PINFO("Entity to be deleted: %d", (int)e.id)
	//std::cout << "Enstity to be deleted" << (int)e.id << std::endl;
}

void ECS::DeleteEntityUpdate()
{
	auto generalEntities = systemManager->ecs->GetEntitiesWith<General>();
	for (Entity entity : generalEntities)
	{
		if (entity.GetComponent<General>().isDelete)
		{
			//std::cout << "Deleting entity:" << (int)entity.id << std::endl;
			systemManager->GetGameStateSystem()->DeleteEntity(entity);
			PINFO("Delete entity done");
		}
	}
}

void ECS::CopyEntity(Entity e)
{
	if (static_cast<std::uint32_t>(e.id) == 0)
		PASSERT("trying to copy null entity");
	mClipboard = e;
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
	bool isExistingPrefab = e.HasComponent<Prefab>();
	e.AddComponent<Prefab>().mPrefab = name;
	ObjectFactory::SerializePrefab(e, "../assets/Prefabs/" + name + ".prefab");
	if (mPrefabs.count(name))
		UpdatePrefabEntities(name);
	if (isExistingPrefab) return;
	mPrefabs[name].push_back(e);
}

Entity ECS::NewEntityFromPrefab(std::string prefabName, const glm::vec3& pos)
{
	// void ObjectFactory::DeserializeScene(const std::string& filename)
	// creation of new entity done inside deserializescene function
	Entity e(ObjectFactory::DeserializePrefab("../assets/Prefabs/" + prefabName + ".prefab"));
	systemManager->mGameStateSystem->mCurrentGameState.mScenes[0].mEntities.insert(e);
	//copy all prefab components (except transform) to new entity
	//General temp1 = e.GetComponent<General>();
	//MeshRenderer temp = e.GetComponent<MeshRenderer>();
	e.GetComponent<Transform>().mTranslate = pos;
	PASSERT(static_cast<uint32_t>(e.id) != 0);
	e.GetComponent<Scripts>().LoadForAllScripts((int)e.id);
	e.GetComponent<Scripts>().RunFunctionForAllScripts("Alive");

	if (e.HasComponent<RigidBody>())
		systemManager->mPhysicsSystem->AddEntity(e);
	return e;
}

void ECS::UpdatePrefabEntities(std::string prefabName)
{
	Entity temp(ObjectFactory::DeserializePrefab("../assets/Prefabs/" + prefabName + ".prefab"));
	
	for (Entity e : mPrefabs[prefabName])
	{
		e.GetComponent<Transform>().mScale = temp.GetComponent<Transform>().mScale;

		AddComponentHelper<ALL_COMPONENTS>(e, temp);
	}

	systemManager->ecs->DeleteEntity(temp);
}

Entity ECS::StartEditPrefab(std::string prefabName)
{
	// void ObjectFactory::DeserializeScene(const std::string& filename)
	// creation of new entity done inside deserializescene function
	Entity e(ObjectFactory::DeserializePrefab("../assets/Prefabs/" + prefabName + ".prefab"));
	//copy all prefab components (except transform) to new entity
	//General temp1 = e.GetComponent<General>();
	//MeshRenderer temp = e.GetComponent<MeshRenderer>();
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
		mr.mMeshRef.data = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(uids.id));
		for (int i{ 0 }; i < 4; i++) {

			if (mr.mTextureRef[i].data != nullptr) {
				uid localuids(mr.mMaterialInstancePath[i]);
				mr.mTextureRef[i].data = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(localuids.id));
			}
		}
		GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mr.mMeshRef.data);
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

ECS::~ECS()
{
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
	if (thisChildren.mFirstChild == static_cast<uint32_t>(e.id))
		thisChildren.mFirstChild = static_cast<uint32_t>(prev.id);
	prev.GetComponent<Parent>().mNextSibling = static_cast<uint32_t>(next.id);
	next.GetComponent<Parent>().mPrevSibling = static_cast<uint32_t>(prev.id);
	e.RemoveComponent<Parent>();
}
