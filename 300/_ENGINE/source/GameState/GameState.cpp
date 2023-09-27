/*!*****************************************************************************
\file GameState.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
GameState is a container that manages a range of scenes. It acts as manager for
the scenes. Only 1 gmae state should be loaded and running in the game at a
time.
*******************************************************************************/
#include "GameState/GameState.h"
#include "ECS/ECS_Components.h"
#include "ScriptingSystem.h"
//#include "Serialization.h"
#include "Object/ObjectFactory.h"
#include "ECS/ECS.h"
#include "GameState/GameStateManager.h"

void GameState::Init() {
	//LOG_INFO("Init() called for gamestate: " + mName + +" ==================");
	for (auto& scene : mScenes) {
		if (!scene.mIsPause) scene.Init();
	}
}

void GameState::Exit() {
	for (auto& scene : mScenes) {
		scene.Exit();
		for (auto e : scene.mEntities)
			if (e.HasComponent<Scripts>()) systemManager->GetScriptingPointer()->ScriptDead(e);
	}
}


void GameState::AddScene(std::string const& _name) { // filesystem
	mScenes.emplace_back(Scene());
	Scene& latestScene{ mScenes.back() };
	
	if (_name.size() == 0) {
		static int newSceneCount = 1;
		latestScene.mName = "New Scene " + std::to_string(newSceneCount++);  //cannot have same GS name
		//LOG_CUSTOM("GAMESTATE", "Adding NEW scene to gamestate: " + mName);
	} else {
		//latestScene.Load(_name);
		//LOG_CUSTOM("GAMESTATE", "Adding scene \"" + _path.stem().string() + "\" to gamestate: " + mName);
	}
	
	if (!latestScene.mIsPause) latestScene.Init();
}

void GameState::RemoveScene(std::string const& _name){
	//LOG_CUSTOM("GAMESTATE", "Attempting to remove scene \"" + _name + "\" from gamestate: " + mName);
	
	for (auto it = mScenes.begin(); it != mScenes.end(); ++it) {
		if (it->mName == _name) {
			it->Unload();
			mScenes.erase(it);
			break;
		}
	}
}

Scene* GameState::GetScene(std::string const& _name) {
	for (Scene& scene : mScenes)
		if (_name == scene.mName) return &scene;
	return systemManager->mGameStateSystem->GetErrorScene();
}


void GameState::Load(std::string const& _name){
	//LOG_CUSTOM("GAMESTATE", "Load GameState: " + _path.string());
	// Load relevant resources here, Game Mode only
#ifndef _EDITOR
	//ResourceManager::GetInstance()->LoadGameStateResources(_path);
#endif
	ObjectFactory::LoadGameState(this, _name);
	for (auto& scene : mScenes) {
		scene.Load();
	}
}

void GameState::Save() {
	//LOG_CUSTOM("GAMESTATE", "Save GameState: " + mName);
	ObjectFactory::SaveGameState(this);
}

void GameState::Unload() {
	//LOG_CUSTOM("GAMESTATE", "Attempt to unload GameState: " + mName);
	for (auto& scene : mScenes)
		scene.Unload();
	mScenes.clear();
#ifndef _EDITOR
	// ResourceManager::GetInstance()->SelectiveUnloadAllResources();
#endif
}





































