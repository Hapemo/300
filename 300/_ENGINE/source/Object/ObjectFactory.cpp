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
#include "Graphics/GraphicsSystem.h"
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
	if (e.HasComponent<MeshRenderer>())
	{
		if (e.GetComponent<MeshRenderer>().mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) != nullptr)
		{
			GFX::Mesh& meshinst = *reinterpret_cast<GFX::Mesh*>(e.GetComponent<MeshRenderer>().mMeshRef.data);
			if (meshinst.mHasAnimation) {
				e.AddComponent<Animator>();
			}
		}
	}
	DESERIALIZE_SELF(BoxCollider, "boxcollider");
	DESERIALIZE_SELF(SphereCollider, "spherecollider");
	DESERIALIZE_SELF(CapsuleCollider, "capsulecollider");
	DESERIALIZE_SELF(Scripts, "scripts");
	DESERIALIZE_SELF(Parent, "parent");
	DESERIALIZE_SELF(Children, "children");
	DESERIALIZE_SELF(Audio, "audio");
	DESERIALIZE_SELF(Camera, "camera");
	DESERIALIZE_SELF(Prefab, "prefab");
	if (e.HasComponent<Prefab>())
		systemManager->ecs->mPrefabs[e.GetComponent<Prefab>().mPrefab].push_back(e);
	DESERIALIZE_SELF(PointLight, "pointlight");
	DESERIALIZE_SELF(AISetting, "aisetting");
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

		auto parent_cont = systemManager->ecs->GetEntitiesWith<Parent>();
		auto child_cont = systemManager->ecs->GetEntitiesWith<Children>();
		for (Entity pe : parent_cont)
		{
			//need to check if entity came from this scene!!!!
			Parent& parent = pe.GetComponent<Parent>();
			if (idMap.count((entt::entity)parent.mNextSibling) == 0) //quick fix
				continue;
			parent.mNextSibling = entt::to_integral(idMap[(entt::entity)parent.mNextSibling]);
			parent.mParent = entt::to_integral(idMap[(entt::entity)parent.mParent]);
			parent.mPrevSibling = entt::to_integral(idMap[(entt::entity)parent.mPrevSibling]);
		}

		for (Entity ce : child_cont)
		{
			//need to check if entity came from this scene!!!!
			Children& child = ce.GetComponent<Children>();
			if (idMap.count((entt::entity)child.mFirstChild) == 0) //quick fix
				continue;
			child.mFirstChild = entt::to_integral(idMap[(entt::entity)child.mFirstChild]);
		}

		std::cout << "tmp_id: " << (int)tmp_id << ", entity_id: " << (int)e.id << ", entity_name: " << e.GetComponent<General>().name << std::endl;
		scene->mEntities.insert(e);
	}
}

void ObjectFactory::LoadGameState(GameState* gs, const std::string& _name)
{
	gs->mName = _name;

	// the doc is my rapidjson::Value& reader
	rapidjson::Document doc;
	ReadFromFile(ConfigManager::GetValue("GameStatePath") + _name + ".gs", doc);

	Scene scn;
	auto& sys = systemManager->mGraphicsSystem;

	// because an array of objects is contained inside of doc
	for (rapidjson::Value::ValueIterator ci = doc.Begin(); ci != doc.End(); ++ci)
	{
		Deserialize(*ci, "scene_name", scn.mName);
		Deserialize(*ci, "pause", scn.mIsPause);
		Deserialize(*ci, "force_render", scn.mForceRender);
		Deserialize(*ci, "bloom_threshold", sys->mAmbientBloomThreshold);
		Deserialize(*ci, "bloom_exposure", sys->mAmbientBloomExposure);
		Deserialize(*ci, "bloom_offset", sys->mTexelOffset);
		Deserialize(*ci, "bloom_sampleweight", sys->mSamplingWeight);
		Deserialize(*ci, "chroma_strength", sys->mChromaticStrength);
		Deserialize(*ci, "bloom_enable", sys->m_EnableBloom);
		Deserialize(*ci, "chroma_enable", sys->m_EnableChromaticAbberation);

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
	SERIALIZE_SELF(CapsuleCollider);
	SERIALIZE_SELF(Scripts);
	SERIALIZE_SELF(Parent);
	SERIALIZE_SELF(Children);
	SERIALIZE_SELF(Audio);
	SERIALIZE_SELF(Camera);
	SERIALIZE_SELF(Prefab);
	SERIALIZE_SELF(PointLight);
	SERIALIZE_SELF(AISetting);
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

	auto& sys = systemManager->mGraphicsSystem;
	for (Scene scn : gs->mScenes)
	{
		writer.StartObject();
		Serialize(writer, "scene_name", scn.mName);
		Serialize(writer, "pause", scn.mIsPause);
		Serialize(writer, "force_render", scn.mForceRender);
		Serialize(writer, "bloom_threshold", sys->mAmbientBloomThreshold);
		Serialize(writer, "bloom_exposure", sys->mAmbientBloomExposure);
		Serialize(writer, "bloom_offset", sys->mTexelOffset);
		Serialize(writer, "bloom_sampleweight", sys->mSamplingWeight);
		Serialize(writer, "chroma_strength", sys->mChromaticStrength);
		Serialize(writer, "bloom_enable", sys->m_EnableBloom);
		Serialize(writer, "chroma_enable", sys->m_EnableChromaticAbberation);


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
