/*!*****************************************************************************
\file Scene.cpp
\author Jazz Teoh Yu Jue, Charissa Yip
\par DP email: j.teoh\@digipen.edu, charissa.yip@digipen.edu
\date 28-9-2023
\brief
Scene base class. Each game state can have multiple scenes which encapsulates
a group of entities and operates on them.
*******************************************************************************/
#include "GameState/Scene.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "ScriptingSystem.h"
#include "Object/ObjectFactory.h"
//#include "LogicSystem.h"
#include "GameState/GameStateManager.h"


Scene::Scene() : mEntities(), mIsPause(false), mName() {
	//LOG_CUSTOM("SCENE", "New Scene created with no name");
}

Scene::Scene(std::string const& _name) : mEntities(), mIsPause(false), mName(_name) {
	(void)_name;
	//LOG_CUSTOM("SCENE", "Scene created with name: " + mName);
}

Scene::~Scene() {
	//LOG_CUSTOM("SCENE", "Scene with name " + mName + " destroyed");
}

void Scene::Init() {

	return; // Temporary remove - Han
	////LOG_INFO("Attempting to call Init() called for scene: " + mName + +" ==================");
	////std::set<Entity> shouldRun{};
	//mInitBefore = true;
	//for (auto e : mEntities) {
	//	//LOG_INFO("Attempting to call Init() called for ENTITY: " + std::to_string(e.id) + " with name: " + e.GetComponent<General>().name + " ==================");
	//	if (e.GetComponent<General>().isActive) {
	//		//LOG_INFO("Call Init() called for ENTITY: " + std::to_string(e.id) + " with name: " + e.GetComponent<General>().name + " ==================");
	//		e.Activate(); 
	//	}
	//}
	
	
	//logicSystem->Init(shouldRun);
};

void Scene::Exit() {
	return; // Temporary remove - Han
	//for (auto e : mEntities) if (e.GetComponent<General>().isActive) e.Deactivate();
};

void Scene::Pause(bool _pause) { 
	//LOG_CUSTOM("SCENE", "Changed Scene \"" + mName + "\" pause status to " + (_pause ? "true" : "false"));
	//for (auto& e : mEntities)
		//e.GetComponent<General>().isPaused = _pause;yj
	mIsPause = _pause; 
	
	if (!_pause) if (!mInitBefore) Init();
}

static bool once = false;

void Scene::Load(std::string const& _name) {
	//LOG_CUSTOM("SCENE", "Loading Scene: " + mName);
	if (mName.empty()) mName = _name;


	
	ObjectFactory::LoadScene(this, mName);
	for (auto e : mEntities) {
		if (e.HasComponent<Scripts>()) systemManager->GetScriptingPointer()->ScriptAlive(e);
		if (e.HasComponent<MeshRenderer>()) {
			auto& meshdata = e.GetComponent<MeshRenderer>();
			
			
		}
		e.GetComponent<General>().isPaused = mIsPause;
	}
	// [10/14] Testing [Audio] Script stuff
	Entity testaudio = systemManager->ecs->NewEntity();
	Entity testaudio2 = systemManager->ecs->NewEntity();
	testaudio.AddComponent<General>().name = "Farm";
	testaudio.AddComponent<Audio>();

	testaudio2.AddComponent<General>().name = "Radio";
	testaudio2.AddComponent<Audio>();

	if (!once)
	{
		Audio& audio1 = testaudio.GetComponent<Audio>();
		audio1.mFullPath = "../assets\\Audio\\farm_ambience.wav";
		audio1.mFilePath = "../assets\\Audio";
		audio1.mFileName = "farm_ambience.wav";
		audio1.mIsEmpty = false;
		audio1.mAudioType = AUDIO_SFX;
		//testaudio.GetComponent<Audio>().mFileName = "farm_ambience";
		
		Audio& audio2 = testaudio2.GetComponent<Audio>();
		audio2.mFullPath = "../assets\\Audio\\tuning-radio-7150.wav";
		audio2.mFilePath = "../assets\\Audio";
		audio2.mFileName = "tuning-radio-7150.wav";
		audio2.mIsEmpty = false;
		audio2.mAudioType = AUDIO_SFX;

		once = true;
	}

	mEntities.insert(testaudio);
	mEntities.insert(testaudio2);
}

void Scene::Save() {
	//LOG_CUSTOM("SCENE", "Saving Scene: " + mName);
	ObjectFactory::SaveScene(this);
}

void Scene::Unload() {
	//LOG_CUSTOM("SCENE", "Unloading Scene: " + mName);

	decltype(mEntities) tempEntities = mEntities;
	for (auto e : tempEntities) {
		RemoveEntity(e);
	}
	assert(mEntities.empty() && std::string("Scene \"" + mName + "\"still contains " + std::to_string(mEntities.size()) + " after unloading").c_str());

	// Clearing all scene data
	mEntities.clear();
	mIsPause = false;
	mName = decltype(mName)();
}

Entity Scene::AddEntity() {
	


	Entity e{ systemManager->ecs->NewEntity() };
	mEntities.insert(e);

	return e;
}

void Scene::RemoveChildFromScene(Entity _e) {
	if (!_e.HasChildren()) return;

	std::vector<Entity> children = _e.GetAllChildren();
	for (Entity child : children) {
		RemoveChildFromScene(child);
		mEntities.erase(child);
	}
}

void Scene::RemoveEntity(Entity _e) {
	// if (_e.GetComponent<General>().isActive) _e.Deactivate(); // Temporary remove - Han
	RemoveChildFromScene(_e);

	mEntities.erase(_e);
	systemManager->ecs->DeleteEntity(_e);
}

bool Scene::IsError() {
	return operator==(*systemManager->mGameStateSystem->GetErrorScene());
}
