/*!*************************************************************************
****
\file ObjectFactory.cpp
\author Charissa Yip, Jazz Teoh Yu Jue, Kew Yu Jun
\par DP email: charissa.yip@digipen.edu
\date 28-9-2023
\brief

This file contains definitions of functions to facilitate the loading
and saving of prefabs, scenes and gamestates using serialization.

****************************************************************************
***/

#include "Object/ObjectFactory.h"
#include "document.h"
#include "GameState/Scene.h"
#include "GameState/GameState.h"
#include "ConfigManager.h"
#include "ResourceManagerTy.h"
#include "Debug/Logger.h"
#include "ECS/ECS_Components.h"

#define SERIALIZE_SELF(T) if (e.HasComponent<T>()) e.GetComponent<T>().SerializeSelf(writer)

// deserialize scenes from the Scenes folder
void ObjectFactory::LoadScene(Scene* scene, const std::string& filename)
{
	//// loop thru container and store entities

	//EntityListJSON entities;
	////entities.DeserializeFile( + filename + ".scn");
	//entities.DeserializeFile(ConfigManager::GetValue("ScenePath") + filename + ".scn");

	//std::unordered_map<entt::entity, entt::entity> idMap;

	//for (auto& obj : entities.EntitiesList())
	//{
	//	Entity e = systemManager->ecs->NewEntity();

	//	idMap.insert({ obj.GetIDJSON(), e.id });

	//	e.GetComponent<General>() = obj.GetGeneralJSON();
	//	e.GetComponent<Transform>() = obj.GetTransformJSON();

	//	if (obj.mrb_t)
	//	{
	//		e.AddComponent<RigidBody>();
	//		e.GetComponent<RigidBody>() = obj.GetRBJSON();
	//	}

	//	if (obj.mmr_t)
	//	{
	//		e.AddComponent<MeshRenderer>();
	//		e.GetComponent<MeshRenderer>() = obj.GetMRJSON();
	//		MeshRenderer& mr = e.GetComponent<MeshRenderer>();
	//		uid uids(mr.mMeshPath);
	//		mr.mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(uids.id));
	//		for (int i{ 0 }; i < 4; i++) {

	//			if (mr.mTextureCont[i] == true) {
	//				uid uidss(mr.mMaterialInstancePath[i]);
	//				mr.mTextureRef[i] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(uidss.id));
	//			}
	//		}
	//		GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mr.mMeshRef);
	//		if (meshinst->mHasAnimation)
	//		{
	//			e.AddComponent<Animator>();
	//			e.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst->mAnimation[0]);
	//		}
	//	}

	//	if (obj.mbc_t)
	//	{
	//		e.AddComponent<BoxCollider>();
	//		e.GetComponent<BoxCollider>() = obj.GetBCJSON();
	//	}

	//	if (obj.msc_t)
	//	{
	//		e.AddComponent<SphereCollider>();
	//		e.GetComponent<SphereCollider>() = obj.GetSCJSON();
	//	}

	//	if (obj.mpc_t)
	//	{
	//		e.AddComponent<PlaneCollider>();
	//		e.GetComponent<PlaneCollider>() = obj.GetPCJSON();
	//	}

	//	if (obj.ms_t)
	//	{
	//		e.AddComponent<Scripts>();
	//		e.GetComponent<Scripts>() = obj.GetSJSON();
	//	}

	//	if (obj.mp_t)
	//	{
	//		e.AddComponent<Parent>();
	//		e.GetComponent<Parent>() = obj.GetPJSON();
	//	}

	//	if (obj.mc_t)
	//	{
	//		e.AddComponent<Children>();
	//		e.GetComponent<Children>() = obj.GetCJSON();
	//	}

	//	if (obj.ma_t)
	//	{
	//		e.AddComponent<Audio>();
	//		e.GetComponent<Audio>() = obj.GetAJSON();
	//	}

	//	if (obj.mcam_t)
	//	{
	//		e.AddComponent<Camera>();
	//		e.GetComponent<Camera>() = obj.GetCAMJSON();
	//	}

	//	if (obj.mpl_t)
	//	{
	//		e.AddComponent<PointLight>();
	//		e.GetComponent<PointLight>() = obj.GetPLJSON();
	//	}

	//	scene->mEntities.insert(e);
	//}
	//auto parent_cont = systemManager->ecs->GetEntitiesWith<Parent>();
	//auto child_cont = systemManager->ecs->GetEntitiesWith<Children>();
	//for (Entity pe : parent_cont)
	//{
	//	//need to check if entity came from this scene!!!!
	//	Parent& parent = pe.GetComponent<Parent>();
	//	if (idMap.count((entt::entity)parent.mNextSibling) == 0) //quick fix
	//		continue;
	//	parent.mNextSibling = entt::to_integral(idMap[(entt::entity)parent.mNextSibling]);
	//	parent.mParent = entt::to_integral(idMap[(entt::entity)parent.mParent]);
	//	parent.mPrevSibling = entt::to_integral(idMap[(entt::entity)parent.mPrevSibling]);
	//}

	//for (Entity ce : child_cont)
	//{
	//	//need to check if entity came from this scene!!!!
	//	Children& child = ce.GetComponent<Children>();
	//	if (idMap.count((entt::entity)child.mFirstChild) == 0) //quick fix
	//		continue;
	//	child.mFirstChild = entt::to_integral(idMap[(entt::entity)child.mFirstChild]);
	//}
	//// split the file name and save it into the scene
	//// eg "../resources/Scenes/test.json"
	//size_t lastSep = filename.find_last_of("/\\");
	//size_t lastStop = filename.find_last_of(".");

	//if (lastSep != std::string::npos && lastStop != std::string::npos && lastStop > lastSep)
	//	scene->mName = filename.substr(lastSep + 1, lastStop - lastSep - 1);

}

void ObjectFactory::LoadGameState(GameState* gs, const std::string& _name)
{
	//gs->mName = _name;

	//SceneListJSON scenes;
	//scenes.DeserializeFile(ConfigManager::GetValue("GameStatePath") + _name + ".gs");

	//Scene sce;
	//for (auto& s : scenes.SceneList())
	//{
	//	sce = s.GetSceneJSON();
	//	gs->mScenes.push_back(sce);
	//}

	//// split the file name and save it into the scene
	//// eg "../resources/GameStates/test.json"
	//size_t lastSep = _name.find_last_of("/\\");
	//size_t lastStop = _name.find_last_of(".");

	//if (lastSep != std::string::npos && lastStop != std::string::npos && lastStop > lastSep)
	//	gs->mName = _name.substr(lastSep + 1, lastStop - lastSep - 1);
}

Entity ObjectFactory::DeserializePrefab(const std::string& filename, int id)
{
	return Entity(0);
	//EntityListJSON entities;
	//entities.DeserializeFile(filename);

	//if (entities.EntitiesList().size() > 1)
	//{
	//	PWARNING("more than 1 entity in prefab file, taking first entity...");
	//}

	//EntityJSON& eJ = entities.EntitiesList().front();

	//Entity e = systemManager->ecs->NewEntity().id;

	//General temp = eJ.GetGeneralJSON();

	//General& curr = e.GetComponent<General>();

	//curr.name = temp.name + " Prefab " + std::to_string(id);
	//curr.isActive = true;
	//curr.isPaused = true;
	////curr.tag = temp.tag;
	//curr.subtag = temp.subtag;

	//Transform tempX = eJ.GetTransformJSON();

	//Transform& xform = e.GetComponent<Transform>();
	//xform.mScale = tempX.mScale;
	//xform.mRotate = tempX.mRotate;


	//if (eJ.mrb_t)
	//{
	//	e.AddComponent<RigidBody>();
	//	e.GetComponent<RigidBody>() = eJ.GetRBJSON();
	//}

	//if (eJ.mmr_t)
	//{
	//	e.AddComponent<MeshRenderer>();
	//	e.GetComponent<MeshRenderer>() = eJ.GetMRJSON();
	//	MeshRenderer& mr = e.GetComponent<MeshRenderer>();
	//	uid uids(mr.mMeshPath);
	//	mr.mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(uids.id));
	//	for (int i{ 0 }; i < 4; i++) {

	//		if (mr.mTextureCont[i] == true) {
	//			uid uidss(mr.mMaterialInstancePath[i]);
	//			mr.mTextureRef[i] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(uidss.id));
	//		}
	//	}
	//	GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mr.mMeshRef);
	//	if (meshinst->mHasAnimation)
	//	{
	//		e.AddComponent<Animator>();
	//		e.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst->mAnimation[0]);
	//	}
	//}

	//if (eJ.mbc_t)
	//{
	//	e.AddComponent<BoxCollider>();
	//	e.GetComponent<BoxCollider>() = eJ.GetBCJSON();
	//}

	//if (eJ.msc_t)
	//{
	//	e.AddComponent<SphereCollider>();
	//	e.GetComponent<SphereCollider>() = eJ.GetSCJSON();
	//}

	//if (eJ.mpc_t)
	//{
	//	e.AddComponent<PlaneCollider>();
	//	e.GetComponent<PlaneCollider>() = eJ.GetPCJSON();
	//}

	//if (eJ.ms_t)
	//{
	//	e.AddComponent<Scripts>();
	//	e.GetComponent<Scripts>() = eJ.GetSJSON();
	//}

	//if (eJ.ma_t)
	//{
	//	e.AddComponent<Audio>();
	//	e.GetComponent<Audio>() = eJ.GetAJSON();
	//}

	//if (eJ.mcam_t)
	//{
	//	e.AddComponent<Camera>();
	//	e.GetComponent<Camera>() = eJ.GetCAMJSON();
	//}

	//if (eJ.mpl_t)
	//{
	//	e.AddComponent<PointLight>();
	//	e.GetComponent<PointLight>() = eJ.GetPLJSON();
	//}

	//return e;
}
// save into the Scenes folder
void ObjectFactory::SaveScene(Scene* scene)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{ buffer };

	writer.StartArray();
	for (Entity e : scene->mEntities)
		SaveEntity(e, writer);
	writer.EndArray();

	WriteToFile(ConfigManager::GetValue("ScenePath") + scene->mName + ".scn", buffer);
}

void ObjectFactory::SaveGameState(GameState* gs)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{ buffer };
	writer.StartArray();
	Serialize(writer, "scenes", gs->mScenes);
	writer.EndArray();
	WriteToFile(ConfigManager::GetValue("GameStatePath") + gs->mName + ".gs", buffer);
}

void ObjectFactory::SerializePrefab(Entity e, const std::string& filename)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{ buffer };
	SaveEntity(e, writer);
	WriteToFile(filename, buffer);
}

void ObjectFactory::SaveEntity(Entity e, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	Serialize(writer, "entityid", static_cast<uint32_t>(e.id));
	SERIALIZE_SELF(General);
	SERIALIZE_SELF(Transform);
	SERIALIZE_SELF(RigidBody);
	SERIALIZE_SELF(MeshRenderer);
	SERIALIZE_SELF(BoxCollider);
	SERIALIZE_SELF(SphereCollider);
	SERIALIZE_SELF(PlaneCollider);
	SERIALIZE_SELF(Scripts);
	SERIALIZE_SELF(Parent);
	SERIALIZE_SELF(Children);
	SERIALIZE_SELF(Audio);
	SERIALIZE_SELF(Camera);
	SERIALIZE_SELF(Prefab);
	SERIALIZE_SELF(PointLight);
	writer.EndObject();
}

// unused (technically)
//void ObjectFactory::DeserializeScene(const std::string& filename)
//{
//	// loop thru container and store entities
//
//	EntityListJSON entities;
//	entities.DeserializeFile(filename);
//
//	std::unordered_map<entt::entity, entt::entity> idMap;
//
//	for (auto& obj : entities.EntitiesList())
//	{
//		Entity e = systemManager->ecs->NewEntity();
//
//		// (oldid, newid)
//		idMap.insert({ obj.GetIDJSON(), e.id });
//
//		e.GetComponent<General>() = obj.GetGeneralJSON();
//		e.GetComponent<Transform>() = obj.GetTransformJSON();
//
//		if (obj.mrb_t)
//		{
//			e.AddComponent<RigidBody>();
//			e.GetComponent<RigidBody>() = obj.GetRBJSON();
//		}
//
//		if (obj.mmr_t)
//		{
//			e.AddComponent<MeshRenderer>();
//			e.GetComponent<MeshRenderer>() = obj.GetMRJSON();
//			MeshRenderer mr = e.GetComponent<MeshRenderer>();
//			uid uids(mr.mMeshPath);
//			mr.mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(uids.id));
//		}
//
//		if (obj.mbc_t)
//		{
//			e.AddComponent<BoxCollider>();
//			e.GetComponent<BoxCollider>() = obj.GetBCJSON();
//		}
//
//		if (obj.msc_t)
//		{
//			e.AddComponent<SphereCollider>();
//			e.GetComponent<SphereCollider>() = obj.GetSCJSON();
//		}
//
//		if (obj.mpc_t)
//		{
//			e.AddComponent<PlaneCollider>();
//			e.GetComponent<PlaneCollider>() = obj.GetPCJSON();
//		}
//
//		if (obj.ms_t)
//		{
//			e.AddComponent<Scripts>();
//			e.GetComponent<Scripts>() = obj.GetSJSON();
//		}
//
//		if (obj.mp_t)
//		{
//			e.AddComponent<Parent>();
//			e.GetComponent<Parent>() = obj.GetPJSON();
//		}
//
//		if (obj.mc_t)
//		{
//			e.AddComponent<Children>();
//			e.GetComponent<Children>() = obj.GetCJSON();
//		}
//
//		if (obj.ma_t)
//		{
//			e.AddComponent<Audio>();
//			e.GetComponent<Audio>() = obj.GetAJSON();
//		}
//	}
//
//	auto parent_cont = systemManager->ecs->GetEntitiesWith<Parent>();
//	auto child_cont = systemManager->ecs->GetEntitiesWith<Children>();
//
//	for (Entity pe : parent_cont)
//	{
//		Parent& parent = pe.GetComponent<Parent>();
//		parent.mNextSibling = (uint32_t)idMap[(entt::entity)parent.mNextSibling];
//		parent.mParent = (uint32_t)idMap[(entt::entity)parent.mParent];
//		parent.mPrevSibling = (uint32_t)idMap[(entt::entity)parent.mPrevSibling];
//	}
//
//	for (Entity ce : child_cont)
//	{
//		Children& child = ce.GetComponent<Children>();
//		child.mFirstChild = (uint32_t)idMap[(entt::entity)child.mFirstChild];
//	}
//}
//
//// unused (technically)
//void ObjectFactory::SerializeScene(const std::string& filename)
//{
//	// loop thru container and store entity data from editor
//	// into .json file
//
//	std::ofstream ofs;
//	ofs.open(filename, std::fstream::out | std::fstream::trunc);
//	ofs.close();
//
//	EntityJSON ent;
//	EntityListJSON entities;
//	entities.EntitiesList().clear();
//
//	auto container = systemManager->ecs->GetEntitiesWith<General, Transform>();
//
//	for (Entity e : container)
//	{
//		// basic stuff that must be set
//		std::string name = e.GetComponent<General>().name;
//		ent.SetIDJSON(e.id);
//		ent.SetGeneralJSON(e.GetComponent<General>());
//		ent.SetTransformJSON(e.GetComponent<Transform>());
//
//		if (e.HasComponent<RigidBody>())
//			ent.SetRBJSON(e.GetComponent<RigidBody>());
//		else ent.mrb_t = false;
//
//		if (e.HasComponent<MeshRenderer>())
//			ent.SetMRJSON(e.GetComponent<MeshRenderer>());
//		else ent.mmr_t = false;
//
//		if (e.HasComponent<BoxCollider>())
//			ent.SetBCJSON(e.GetComponent<BoxCollider>());
//		else ent.mbc_t = false;
//
//		if (e.HasComponent<SphereCollider>())
//			ent.SetSCJSON(e.GetComponent<SphereCollider>());
//		else ent.msc_t = false;
//
//		if (e.HasComponent<PlaneCollider>())
//			ent.SetPCJSON(e.GetComponent<PlaneCollider>());
//		else ent.mpc_t = false;
//
//		if (e.HasComponent<Scripts>())
//			ent.SetSJSON(e.GetComponent<Scripts>());
//		else ent.ms_t = false;
//
//		if (e.HasComponent<Parent>())
//			ent.SetPJSON(e.GetComponent<Parent>());
//		else ent.mp_t = false;
//
//		if (e.HasComponent<Children>())
//			ent.SetCJSON(e.GetComponent<Children>());
//		else ent.mc_t = false;
//
//		if (e.HasComponent<Audio>())
//			ent.SetAJSON(e.GetComponent<Audio>());
//		else ent.ma_t = false;
//
//		// push back after done
//		entities.EntitiesList().push_back(ent);
//	}
//
//	entities.SerializeFile(filename);
//}