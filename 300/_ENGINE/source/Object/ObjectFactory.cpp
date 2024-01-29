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
#include "Physics/PhysicsSystem.h"
#include "AI/AIManager.h"
#include "ScriptingSystem.h"

#define SERIALIZE_SELF(T) if (e.HasComponent<T>()) e.GetComponent<T>().SerializeSelf(writer)
#define DESERIALIZE_SELF(T, S) if(reader.HasMember(S)) e.AddComponent<T>().DeserializeSelf(reader[S])

void ObjectFactory::LoadEntity(Entity e, rapidjson::Value& reader)
{
	DESERIALIZE_SELF(General, "general");
	DESERIALIZE_SELF(Transform, "transform");
	DESERIALIZE_SELF(RigidBody, "rigidbody");
	DESERIALIZE_SELF(MeshRenderer, "meshrenderer");
	DESERIALIZE_SELF(Animator, "animator");

	if (e.HasComponent<MeshRenderer>())
	{
		if(e.GetComponent<MeshRenderer>().mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) != nullptr)
		{
			GFX::Mesh& meshinst = *reinterpret_cast<GFX::Mesh*>(e.GetComponent<MeshRenderer>().mMeshRef.data);
			if (meshinst.mHasAnimation) {
				e.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst.mAnimation[0]);
			}
		}
	}
	DESERIALIZE_SELF(UIrenderer, "uirenderer");
	DESERIALIZE_SELF(BoxCollider, "boxcollider");
	DESERIALIZE_SELF(SphereCollider, "spherecollider");
	DESERIALIZE_SELF(CapsuleCollider, "capsulecollider");
	DESERIALIZE_SELF(MeshCollider, "meshcollider");
	DESERIALIZE_SELF(Scripts, "scripts");
	DESERIALIZE_SELF(Parent, "parent");
	DESERIALIZE_SELF(Children, "children");
	DESERIALIZE_SELF(Audio, "audio");
	DESERIALIZE_SELF(AudioListener, "audiolistener");
	DESERIALIZE_SELF(Camera, "camera");
	DESERIALIZE_SELF(Prefab, "prefab");
	if (e.HasComponent<Prefab>())
		systemManager->ecs->mPrefabs[e.GetComponent<Prefab>().mPrefab].push_back(e);
	DESERIALIZE_SELF(PointLight, "pointlight");
	DESERIALIZE_SELF(AISetting, "aisetting");
	if (e.HasComponent<AISetting>())
		systemManager->mAISystem->InitialiseAI(e);
	DESERIALIZE_SELF(Crosshair, "crosshair");
	DESERIALIZE_SELF(Healthbar, "healthbar");
	DESERIALIZE_SELF(Button, "button");
	DESERIALIZE_SELF(Spotlight, "spotlight");
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

		//std::cout << "tmp_id: " << (int)tmp_id << ", entity_id: " << (int)e.id << ", entity_name: " << e.GetComponent<General>().name << std::endl;
		scene->mEntities.insert(e);
	}
	auto parent_cont = systemManager->ecs->GetEntitiesWith<Parent>();
	auto child_cont = systemManager->ecs->GetEntitiesWith<Children>();
	for (Entity pe : parent_cont)
	{
		//need to check if entity came from this scene!!!!
		if (!scene->HasEntity(pe)) continue;
		Parent& parent = pe.GetComponent<Parent>();
		
		parent.mNextSibling = entt::to_integral(idMap[(entt::entity)parent.mNextSibling]);
		parent.mParent = entt::to_integral(idMap[(entt::entity)parent.mParent]);
		parent.mPrevSibling = entt::to_integral(idMap[(entt::entity)parent.mPrevSibling]);
		parent.mPrevSibling = parent.mPrevSibling;
	}

	for (Entity ce : child_cont)
	{
		//need to check if entity came from this scene!!!!
		if (!scene->HasEntity(ce)) continue;

		Children& child = ce.GetComponent<Children>();
		if (idMap.count((entt::entity)child.mFirstChild) == 0) //quick fix
			continue;
		child.mFirstChild = entt::to_integral(idMap[(entt::entity)child.mFirstChild]);
		child.mFirstChild = child.mFirstChild;
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
		Deserialize(*ci, "chroma_strength", sys->mChromaticOffset);
		Deserialize(*ci, "bloom_enable", sys->m_EnableBloom);
		Deserialize(*ci, "crt_enable", sys->m_EnableCRT);
		Deserialize(*ci, "crt_distortion_value", PostProcessing::getInstance().mCRT_DistortionValue);
		Deserialize(*ci, "crt_height_offset", PostProcessing::getInstance().mCRT_HeightOffset);
		Deserialize(*ci, "chroma_enable", sys->m_EnableChromaticAbberation);
		Deserialize(*ci, "global_tint", sys->m_GlobalTint);
		Deserialize(*ci, "debug", sys->m_DebugDrawing);
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
	SERIALIZE_SELF(Animator);
	SERIALIZE_SELF(UIrenderer);
	SERIALIZE_SELF(BoxCollider);
	SERIALIZE_SELF(SphereCollider);
	SERIALIZE_SELF(CapsuleCollider);
	SERIALIZE_SELF(MeshCollider);
	SERIALIZE_SELF(Scripts);
	SERIALIZE_SELF(Parent);
	SERIALIZE_SELF(Children);
	SERIALIZE_SELF(Audio);
	SERIALIZE_SELF(AudioListener);
	SERIALIZE_SELF(Camera);
	SERIALIZE_SELF(Prefab);
	SERIALIZE_SELF(PointLight);
	SERIALIZE_SELF(AISetting);
	SERIALIZE_SELF(Crosshair);
	SERIALIZE_SELF(Healthbar);
	SERIALIZE_SELF(Button);
	SERIALIZE_SELF(Spotlight);
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
		Serialize(writer, "chroma_strength", sys->mChromaticOffset);
		Serialize(writer, "bloom_enable", sys->m_EnableBloom);
		Serialize(writer, "crt_enable", sys->m_EnableCRT);
		Serialize(writer, "crt_distortion_value", PostProcessing::getInstance().mCRT_DistortionValue);
		Serialize(writer, "crt_height_offset", PostProcessing::getInstance().mCRT_HeightOffset);
		Serialize(writer, "chroma_enable", sys->m_EnableChromaticAbberation);
		Serialize(writer, "global_tint", sys->m_GlobalTint);
		Serialize(writer, "debug", sys->m_DebugDrawing);
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
