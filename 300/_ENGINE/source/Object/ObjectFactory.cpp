#include "Object/ObjectFactory.h"
#include "document.h"
#include "GameState/Scene.h"
#include "GameState/GameState.h"
#include "ConfigManager.h"
#include "ResourceManagerTy.h"
#include "Debug/Logger.h"
// unused (technically)
void ObjectFactory::DeserializeScene(const std::string& filename)
{
	// loop thru container and store entities

	EntityListJSON entities;
	entities.DeserializeFile(filename);

	std::unordered_map<entt::entity, entt::entity> idMap;

	for (auto& obj : entities.EntitiesList())
	{
		Entity e = systemManager->ecs->NewEntity();

		// (oldid, newid)
		idMap.insert({ obj.GetIDJSON(), e.id });

		e.GetComponent<General>() = obj.GetGeneralJSON();
		e.GetComponent<Transform>() = obj.GetTransformJSON();

		if (obj.mrb_t)
		{
			e.AddComponent<RigidBody>();
			e.GetComponent<RigidBody>() = obj.GetRBJSON();
		}

		if (obj.mmr_t)
		{
			e.AddComponent<MeshRenderer>();
			e.GetComponent<MeshRenderer>() = obj.GetMRJSON();
			MeshRenderer mr = e.GetComponent<MeshRenderer>();
			uid uids(mr.mMeshPath);
			mr.mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(uids.id));
		}

		if (obj.mbc_t)
		{
			e.AddComponent<BoxCollider>();
			e.GetComponent<BoxCollider>() = obj.GetBCJSON();
		}

		if (obj.msc_t)
		{
			e.AddComponent<SphereCollider>();
			e.GetComponent<SphereCollider>() = obj.GetSCJSON();
		}

		if (obj.mpc_t)
		{
			e.AddComponent<PlaneCollider>();
			e.GetComponent<PlaneCollider>() = obj.GetPCJSON();
		}

		if (obj.ms_t)
		{
			e.AddComponent<Scripts>();
			e.GetComponent<Scripts>() = obj.GetSJSON();
		}

		if (obj.mp_t)
		{
			e.AddComponent<Parent>();
			e.GetComponent<Parent>() = obj.GetPJSON();
		}

		if (obj.mc_t)
		{
			e.AddComponent<Children>();
			e.GetComponent<Children>() = obj.GetCJSON();
		}

		if (obj.ma_t)
		{
			e.AddComponent<Audio>();
			e.GetComponent<Audio>() = obj.GetAJSON();
		}
	}

	auto parent_cont = systemManager->ecs->GetEntitiesWith<Parent>();
	auto child_cont = systemManager->ecs->GetEntitiesWith<Children>();

	for (Entity pe : parent_cont)
	{
		Parent& parent = pe.GetComponent<Parent>();
		parent.mNextSibling = (uint32_t)idMap[(entt::entity)parent.mNextSibling];
		parent.mParent = (uint32_t)idMap[(entt::entity)parent.mParent];
		parent.mPrevSibling = (uint32_t)idMap[(entt::entity)parent.mPrevSibling];
	}

	for (Entity ce : child_cont)
	{
		Children& child = ce.GetComponent<Children>();
		child.mFirstChild = (uint32_t)idMap[(entt::entity)child.mFirstChild];
	}
}

// unused (technically)
void ObjectFactory::SerializeScene(const std::string& filename)
{
	// loop thru container and store entity data from editor
	// into .json file

	std::ofstream ofs;
	ofs.open(filename, std::fstream::out | std::fstream::trunc);
	ofs.close();

	EntityJSON ent;
	EntityListJSON entities;
	entities.EntitiesList().clear();

	auto container = systemManager->ecs->GetEntitiesWith<General, Transform>();

	for (Entity e : container)
	{
		// basic stuff that must be set
		std::string name = e.GetComponent<General>().name;
		ent.SetIDJSON(e.id);
		ent.SetGeneralJSON(e.GetComponent<General>());
		ent.SetTransformJSON(e.GetComponent<Transform>());

		if (e.HasComponent<RigidBody>())
			ent.SetRBJSON(e.GetComponent<RigidBody>());
		else ent.mrb_t = false;

		if (e.HasComponent<MeshRenderer>())
			ent.SetMRJSON(e.GetComponent<MeshRenderer>());
		else ent.mmr_t = false;

		if (e.HasComponent<BoxCollider>())
			ent.SetBCJSON(e.GetComponent<BoxCollider>());
		else ent.mbc_t = false;

		if (e.HasComponent<SphereCollider>())
			ent.SetSCJSON(e.GetComponent<SphereCollider>());
		else ent.msc_t = false;

		if (e.HasComponent<PlaneCollider>())
			ent.SetPCJSON(e.GetComponent<PlaneCollider>());
		else ent.mpc_t = false;

		if (e.HasComponent<Scripts>())
			ent.SetSJSON(e.GetComponent<Scripts>());
		else ent.ms_t = false;

		if (e.HasComponent<Parent>())
			ent.SetPJSON(e.GetComponent<Parent>());
		else ent.mp_t = false;

		if (e.HasComponent<Children>())
			ent.SetCJSON(e.GetComponent<Children>());
		else ent.mc_t = false;

		if (e.HasComponent<Audio>())
			ent.SetAJSON(e.GetComponent<Audio>());
		else ent.ma_t = false;

		// push back after done
		entities.EntitiesList().push_back(ent);
	}

	entities.SerializeFile(filename);
}

void ObjectFactory::SerializePrefab(Entity e, const std::string& filename)
{
	std::ofstream ofs;
	ofs.open(filename, std::fstream::out | std::fstream::trunc);
	ofs.close();

	EntityJSON ent;
	EntityListJSON entities;
	entities.EntitiesList().clear();

	// basic stuff that must be set
	ent.SetIDJSON(e.id);
	ent.SetGeneralJSON(e.GetComponent<General>());
	ent.SetTransformJSON(e.GetComponent<Transform>());

	// basic stuff that must be set
	std::string name = e.GetComponent<General>().name;
	ent.SetIDJSON(e.id);
	ent.SetGeneralJSON(e.GetComponent<General>());
	ent.SetTransformJSON(e.GetComponent<Transform>());

	if (e.HasComponent<RigidBody>())
		ent.SetRBJSON(e.GetComponent<RigidBody>());
	else ent.mrb_t = false;

	if (e.HasComponent<MeshRenderer>())
		ent.SetMRJSON(e.GetComponent<MeshRenderer>());
	else ent.mmr_t = false;

	if (e.HasComponent<BoxCollider>())
		ent.SetBCJSON(e.GetComponent<BoxCollider>());
	else ent.mbc_t = false;

	if (e.HasComponent<SphereCollider>())
		ent.SetSCJSON(e.GetComponent<SphereCollider>());
	else ent.msc_t = false;

	if (e.HasComponent<PlaneCollider>())
		ent.SetPCJSON(e.GetComponent<PlaneCollider>());
	else ent.mpc_t = false;

	if (e.HasComponent<Scripts>())
		ent.SetSJSON(e.GetComponent<Scripts>());
	else ent.ms_t = false;

	if (e.HasComponent<Parent>())
		ent.SetPJSON(e.GetComponent<Parent>());
	else ent.mp_t = false;

	ent.mc_t = false;
	ent.ma_t = false;

	// push back after done
	entities.EntitiesList().push_back(ent);

	entities.SerializeFile(filename);
}

Entity ObjectFactory::DeserializePrefab(const std::string& filename, int id)
{
	EntityListJSON entities;
	entities.DeserializeFile(filename);

	if (entities.EntitiesList().size() > 1)
		PWARNING("more than 1 entity in prefab file, taking first entity...");

	EntityJSON& eJ = entities.EntitiesList().front();

	Entity e = systemManager->ecs->NewEntity().id;

	General temp = eJ.GetGeneralJSON();

	General& curr = e.GetComponent<General>();

	curr.name = temp.name + " Prefab " + std::to_string(id);
	curr.isActive = true;
	curr.isPaused = true;
	curr.tag = temp.tag;
	curr.subtag = temp.subtag;

	Transform tempX = eJ.GetTransformJSON();

	Transform& xform = e.GetComponent<Transform>();
	xform.mScale = tempX.mScale;
	xform.mRotate = tempX.mRotate;


	if (eJ.mrb_t)
	{
		e.AddComponent<RigidBody>();
		e.GetComponent<RigidBody>() = eJ.GetRBJSON();
	}

	if (eJ.mmr_t)
	{
		e.AddComponent<MeshRenderer>();
		e.GetComponent<MeshRenderer>() = eJ.GetMRJSON();
		MeshRenderer& mr = e.GetComponent<MeshRenderer>();
		uid uids(mr.mMeshPath);
		mr.mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(uids.id));
		for (int i{ 0 }; i < 4; i++) {

			if (mr.mTextureCont[i] == true) {
				uid uids(mr.mMaterialInstancePath[i]);
				mr.mTextureRef[i] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(uids.id));
			}
		}
		GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mr.mMeshRef);
		if (meshinst->mHasAnimation)
		{
			e.AddComponent<Animator>();
			e.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst->mAnimation[0]);
		}
	}

	if (eJ.mbc_t)
	{
		e.AddComponent<BoxCollider>();
		e.GetComponent<BoxCollider>() = eJ.GetBCJSON();
	}

	if (eJ.msc_t)
	{
		e.AddComponent<SphereCollider>();
		e.GetComponent<SphereCollider>() = eJ.GetSCJSON();
	}

	if (eJ.mpc_t)
	{
		e.AddComponent<PlaneCollider>();
		e.GetComponent<PlaneCollider>() = eJ.GetPCJSON();
	}

	if (eJ.ms_t)
	{
		e.AddComponent<Scripts>();
		e.GetComponent<Scripts>() = eJ.GetSJSON();
	}

	if (eJ.ma_t)
	{
		e.AddComponent<Audio>();
		e.GetComponent<Audio>() = eJ.GetAJSON();
	}

	return e;
}

// deserialize scenes from the Scenes folder
void ObjectFactory::LoadScene(Scene* scene, const std::string& filename)
{
	// loop thru container and store entities

	EntityListJSON entities;
	//entities.DeserializeFile(ConfigManager::GetValue("ScenePath") + filename + ".scn");
	entities.DeserializeFile("../assets/Scenes/" + filename + ".scn");

	std::unordered_map<entt::entity, entt::entity> idMap;

	for (auto& obj : entities.EntitiesList())
	{
		Entity e = systemManager->ecs->NewEntity();

		idMap.insert({ obj.GetIDJSON(), e.id });

		e.GetComponent<General>() = obj.GetGeneralJSON();
		e.GetComponent<Transform>() = obj.GetTransformJSON();

		if (obj.mrb_t)
		{
			e.AddComponent<RigidBody>();
			e.GetComponent<RigidBody>() = obj.GetRBJSON();
		}

		if (obj.mmr_t)
		{
			e.AddComponent<MeshRenderer>();
			e.GetComponent<MeshRenderer>() = obj.GetMRJSON();
			MeshRenderer& mr = e.GetComponent<MeshRenderer>();
			uid uids(mr.mMeshPath);
			mr.mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(uids.id));
			for (int i{ 0 }; i < 4; i++) {

				if (mr.mTextureCont[i] == true) {
					uid uids(mr.mMaterialInstancePath[i]);
					mr.mTextureRef[i] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(uids.id));
				}
			}
			GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mr.mMeshRef);
			if (meshinst->mHasAnimation)
			{
				e.AddComponent<Animator>();
				e.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst->mAnimation[0]);
			}
		}

		if (obj.mbc_t)
		{
			e.AddComponent<BoxCollider>();
			e.GetComponent<BoxCollider>() = obj.GetBCJSON();
		}

		if (obj.msc_t)
		{
			e.AddComponent<SphereCollider>();
			e.GetComponent<SphereCollider>() = obj.GetSCJSON();
		}

		if (obj.mpc_t)
		{
			e.AddComponent<PlaneCollider>();
			e.GetComponent<PlaneCollider>() = obj.GetPCJSON();
		}

		if (obj.ms_t)
		{
			e.AddComponent<Scripts>();
			e.GetComponent<Scripts>() = obj.GetSJSON();
		}

		if (obj.mp_t)
		{
			e.AddComponent<Parent>();
			e.GetComponent<Parent>() = obj.GetPJSON();

			Parent& parent = e.GetComponent<Parent>();
			parent.mNextSibling = (uint32_t)idMap[(entt::entity)parent.mNextSibling];
			parent.mParent = (uint32_t)idMap[(entt::entity)parent.mParent];
			parent.mPrevSibling = (uint32_t)idMap[(entt::entity)parent.mPrevSibling];
		}

		if (obj.mc_t)
		{
			e.AddComponent<Children>();
			e.GetComponent<Children>() = obj.GetCJSON();

			Children& child = e.GetComponent<Children>();
			child.mFirstChild = (uint32_t)idMap[(entt::entity)child.mFirstChild];
		}

		if (obj.ma_t)
		{
			e.AddComponent<Audio>();
			e.GetComponent<Audio>() = obj.GetAJSON();
		}

		scene->mEntities.insert(e);
	}

	// split the file name and save it into the scene
	// eg "../resources/Scenes/test.json"
	size_t lastSep = filename.find_last_of("/\\");
	size_t lastStop = filename.find_last_of(".");

	if (lastSep != std::string::npos && lastStop != std::string::npos && lastStop > lastSep)
		scene->mName = filename.substr(lastSep + 1, lastStop - lastSep - 1);

}

// save into the Scenes folder
void ObjectFactory::SaveScene(Scene* scene)
{
	// form the filename
	std::string filename = "../assets/Scenes/" + scene->mName + ".scn";

	std::ofstream ofs;
	ofs.open(filename, std::fstream::out | std::fstream::trunc);
	ofs.close();

	EntityJSON ent;
	EntityListJSON entities;
	entities.EntitiesList().clear();

	for (Entity e : scene->mEntities)
	{
		// basic stuff that must be set
		std::string name = e.GetComponent<General>().name;
		ent.SetIDJSON(e.id);
		ent.SetGeneralJSON(e.GetComponent<General>());
		ent.SetTransformJSON(e.GetComponent<Transform>());

		if (e.HasComponent<RigidBody>())
			ent.SetRBJSON(e.GetComponent<RigidBody>());
		else ent.mrb_t = false;

		if (e.HasComponent<MeshRenderer>())
			ent.SetMRJSON(e.GetComponent<MeshRenderer>());
		else ent.mmr_t = false;

		if (e.HasComponent<BoxCollider>())
			ent.SetBCJSON(e.GetComponent<BoxCollider>());
		else ent.mbc_t = false;

		if (e.HasComponent<SphereCollider>())
			ent.SetSCJSON(e.GetComponent<SphereCollider>());
		else ent.msc_t = false;

		if (e.HasComponent<PlaneCollider>())
			ent.SetPCJSON(e.GetComponent<PlaneCollider>());
		else ent.mpc_t = false;

		if (e.HasComponent<Scripts>())
			ent.SetSJSON(e.GetComponent<Scripts>());
		else ent.ms_t = false;

		if (e.HasComponent<Parent>())
			ent.SetPJSON(e.GetComponent<Parent>());
		else ent.mp_t = false;

		if (e.HasComponent<Children>())
			ent.SetCJSON(e.GetComponent<Children>());
		else ent.mc_t = false;

		if (e.HasComponent<Audio>())
			ent.SetAJSON(e.GetComponent<Audio>());
		else ent.ma_t = false;

		// push back after done
		entities.EntitiesList().push_back(ent);
	}

	entities.SerializeFile(filename);
}

void ObjectFactory::LoadGameState(GameState* gs, const std::string& _name)
{
	gs->mName = _name;

	SceneListJSON scenes;
	scenes.DeserializeFile("../assets/GameStates/" + _name + ".gs");

	Scene sce;
	for (auto& s : scenes.SceneList())
	{
		sce = s.GetSceneJSON();
		gs->mScenes.push_back(sce);
	}

	// split the file name and save it into the scene
	// eg "../resources/GameStates/test.json"
	size_t lastSep = _name.find_last_of("/\\");
	size_t lastStop = _name.find_last_of(".");

	if (lastSep != std::string::npos && lastStop != std::string::npos && lastStop > lastSep)
		gs->mName = _name.substr(lastSep + 1, lastStop - lastSep - 1);
}

void ObjectFactory::SaveGameState(GameState* gs)
{
	// form the filename
	std::string filename = "../assets/GameStates/" + gs->mName + ".gs";

	std::ofstream ofs;
	ofs.open(filename, std::fstream::out | std::fstream::trunc);

	SceneJSON sce;
	SceneListJSON scenes;
	scenes.SceneList().clear();

	for (Scene s : gs->mScenes)
	{
		sce.SetSceneJSON(s);

		// push back after done
		scenes.SceneList().push_back(sce);
	}

	scenes.SerializeFile(filename);
	ofs.close();
}