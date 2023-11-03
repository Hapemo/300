/*!*****************************************************************************
\file GameState.cpp
\author Jazz Teoh Yu Jue, Charissa Yip
\par DP email: j.teoh\@digipen.edu, charissa.yip@digipen.edu
\date 28-9-2023
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
#include "AI/AIManager.h"
#include "Physics/PhysicsSystem.h"

void GameState::Init() {
	//LOG_INFO("Init() called for gamestate: " + mName + +" ==================");
	for (auto& scene : mScenes) {
		if (!scene.mIsPause) scene.Init();
	}
	systemManager->mAISystem->InitAIs();
}

void GameState::Exit() {
	for (auto& scene : mScenes) {
		scene.Exit();
		for (auto e : scene.mEntities)
			if (e.HasComponent<Scripts>())
				e.GetComponent<Scripts>().RunFunctionForAllScripts("Dead");
	}

	systemManager->mAISystem->ClearAIs();
}


void GameState::AddScene(std::string const& _name) { // filesystem
	mScenes.emplace_back(Scene());
	Scene& latestScene{ mScenes.back() };
	
	if (_name.size() == 0) {
		static int newSceneCount = 1;
		latestScene.mName = "New Scene " + std::to_string(newSceneCount++);  //cannot have same GS name
		//LOG_CUSTOM("GAMESTATE", "Adding NEW scene to gamestate: " + mName);
	} 
	else {
		std::string newName{};
		int counter = 0;
		if (std::find_if(mScenes.begin(), mScenes.end(), [_name](Scene& scene) -> bool { return scene.mName == _name; }) != mScenes.end()) {
			// if the scene exists
			++counter; // increment the counter
			// Increment file number to the biggest one
			for ([[maybe_unused]]Scene& scene : systemManager->mGameStateSystem->mCurrentGameState.mScenes) {
				// if the scene name exists AND 
				std::string tempName = _name + std::to_string(counter);
				if (std::find_if(mScenes.begin(), mScenes.end(), [tempName](Scene& scene) -> bool { return scene.mName == tempName; }) != mScenes.end())
					++counter;
			}
			
			std::string tempName = _name + std::to_string(counter);
			if (std::find_if(mScenes.begin(), mScenes.end(), [tempName](Scene& scene) -> bool { return scene.mName == tempName; }) == mScenes.end())
				newName = tempName;
		}
		latestScene.Load(_name);
		if (!newName.empty())
			latestScene.mName = newName;

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
	systemManager->GetPhysicsPointer()->Init();
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





































