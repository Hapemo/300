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
#define DESERIALIZE_SELF(T, S) if(reader.HasMember(S)) e.AddComponent<T>().DeserializeSelf(reader[S])

void ObjectFactory::LoadEntity(Entity e, rapidjson::Value& reader)
{
	DESERIALIZE_SELF(General, "general");
	DESERIALIZE_SELF(Transform, "transform");
	DESERIALIZE_SELF(RigidBody, "rigidbody");
	DESERIALIZE_SELF(MeshRenderer, "meshrenderer");
	DESERIALIZE_SELF(BoxCollider, "boxcollider");
	DESERIALIZE_SELF(SphereCollider, "spherecollider");
	DESERIALIZE_SELF(PlaneCollider, "planecollider");
	DESERIALIZE_SELF(Scripts, "scripts");
	DESERIALIZE_SELF(Parent, "parent");
	DESERIALIZE_SELF(Children, "children");
	DESERIALIZE_SELF(Audio, "audio");
	DESERIALIZE_SELF(Camera, "camera");
	DESERIALIZE_SELF(Prefab, "prefab");
	DESERIALIZE_SELF(PointLight, "pointlight");
}

// deserialize scenes from the Scenes folder
void ObjectFactory::LoadScene(Scene* scene, const std::string& filename)
{
	// the doc is my rapidjson::Value& reader
	rapidjson::Document doc;
	ReadFromFile(ConfigManager::GetValue("ScenePath") + filename + ".scn", doc);

	std::unordered_map<entt::entity, entt::entity> idMap;
	entt::entity tmp_id{};

	// because an array of objects is contained inside of doc
	for (rapidjson::Value::ValueIterator ci = doc.Begin(); ci != doc.End(); ++ci)
	{
		Entity e = systemManager->ecs->NewEntity();
		Deserialize(*ci, "entityid", tmp_id);
		idMap.insert({ tmp_id, e.id });
		LoadEntity(e, *ci);
		std::cout << "tmp_id: " << (int)tmp_id << ", entity_id: " << (int)e.id << ", entity_name: " << e.GetComponent<General>().name << std::endl;
		scene->mEntities.insert(e);

		std::cout << (int)e.id << std::endl;
	}
}

void ObjectFactory::LoadGameState(GameState* gs, const std::string& _name)
{
	gs->mName = _name;

	// the doc is my rapidjson::Value& reader
	rapidjson::Document doc;
	ReadFromFile(ConfigManager::GetValue("GameStatePath") + _name + ".gs", doc);

	Scene scn;
	// because an array of objects is contained inside of doc
	for (rapidjson::Value::ValueIterator ci = doc.Begin(); ci != doc.End(); ++ci)
	{
		Deserialize(*ci, "scene_name", scn.mName);
		Deserialize(*ci, "pause", scn.mIsPause);
		Deserialize(*ci, "force_render", scn.mForceRender);

		gs->mScenes.push_back(scn);
	}
}

Entity ObjectFactory::DeserializePrefab(const std::string& filename)
{
	//return Entity(0);
	rapidjson::Document doc;
	ReadFromFile(filename, doc);

	Entity e = systemManager->ecs->NewEntity();
	LoadEntity(e, doc);
	return e;
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
	for (Scene scn : gs->mScenes)
	{
		writer.StartObject();
		Serialize(writer, "scene_name", scn.mName);
		Serialize(writer, "pause", scn.mIsPause);
		Serialize(writer, "force_render", scn.mForceRender);
		writer.EndObject();
	}
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

// OLD LOADSCENE CONTENTS
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