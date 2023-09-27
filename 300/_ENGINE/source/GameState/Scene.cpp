/*!*****************************************************************************
\file Scene.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Scene base class. Each game state can have multiple scenes which encapsulates
a group of entities and operates on them.

For Milestone 2:
Added load and unload of scene using resourceManager
Added adding and removing of entity
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

//Scene::Scene(ResourceManager::GUID const& _guid) : mEntities(), mIsPause(false), mName(),/* mCamera(),*/ mLayer(), mOrder() {
//	(void)_guid;
//	//LOG_CUSTOM("SCENE", "New Scene created with no name");
//}

Scene::Scene(std::string const& _name) : mEntities(), mIsPause(false), mName(_name) {
	(void)_name;
	//LOG_CUSTOM("SCENE", "Scene created with name: " + mName);
}

Scene::~Scene() {
	//LOG_CUSTOM("SCENE", "Scene with name " + mName + " destroyed");
}

void Scene::Init() {
	return; // Temporary remove - Han

	//LOG_INFO("Attempting to call Init() called for scene: " + mName + +" ==================");
	//std::set<Entity> shouldRun{};
	mInitBefore = true;
	for (auto e : mEntities) {
		//LOG_INFO("Attempting to call Init() called for ENTITY: " + std::to_string(e.id) + " with name: " + e.GetComponent<General>().name + " ==================");
		if (e.GetComponent<General>().isActive) {
			//LOG_INFO("Call Init() called for ENTITY: " + std::to_string(e.id) + " with name: " + e.GetComponent<General>().name + " ==================");
			e.Activate(); 
		}
	}
	
	
	//logicSystem->Init(shouldRun);
};

//void Scene::Update() {
//	
//};

void Scene::Exit() {
	return; // Temporary remove - Han
	for (auto e : mEntities) if (e.GetComponent<General>().isActive) e.Deactivate();
};

//void Scene::PrimaryUpdate() {
//	if (mIsPause) return;
//	Update();
//}

void Scene::Pause(bool _pause) { 
	//LOG_CUSTOM("SCENE", "Changed Scene \"" + mName + "\" pause status to " + (_pause ? "true" : "false"));
	for (auto& e : mEntities)
		//e.GetComponent<General>().isPaused = _pause;yj
	mIsPause = _pause; 
	
	if (!_pause) if (!mInitBefore) Init();
}

void Scene::Load(std::string const& _name) {
	//LOG_CUSTOM("SCENE", "Loading Scene: " + mName);
	if (mName.empty()) mName = _name;


	
	ObjectFactory::LoadScene(this, mName);
	for (auto e : mEntities) {
		if (e.HasComponent<Scripts>()) systemManager->GetScriptingPointer()->ScriptAlive(e);
		e.GetComponent<General>().isPaused = mIsPause;
	}
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

void Scene::RemoveEntity(Entity _e) {
	// if (_e.GetComponent<General>().isActive) _e.Deactivate(); // Temporary remove - Han
	if (_e.HasChildren()) {
		std::vector<Entity> children = _e.GetAllChildren();
		for (Entity child : children) {
			RemoveEntity(child);
		}
	}

	mEntities.erase(_e);
	systemManager->ecs->DeleteEntity(_e);
}

bool Scene::IsError() {
	return operator==(*systemManager->mGameStateSystem->GetErrorScene());
}
