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
#include "GameState.h"
//#include "Serialization.h"

void GameState::Init() {
	//LOG_INFO("Init() called for gamestate: " + mName + +" ==================");
	for (auto& scene : mScenes) {
		//LOG_INFO("Attempting to call Alive() called for scene: " + scene.mName + +" ==================");
#ifdef _EDITOR
		if (!editorManager->IsScenePaused())
#endif
			for (auto e : scene.mEntities) {
				if (e.HasComponent<Script>()) logicSystem->Alive(e);
			}
#ifdef _EDITOR
		if (!editorManager->IsScenePaused())
#endif
			if (!scene.mIsPause) scene.Init();

	}
}

void GameState::Update() {
	for (auto& scene : mScenes)
		if (!scene.mIsPause) scene.PrimaryUpdate();
}

void GameState::Exit() {
	for (auto& scene : mScenes) {
		scene.Exit();
		for (auto e : scene.mEntities) {
			if (e.HasComponent<Script>()) logicSystem->Dead(e);
		}
	}
	particleManager->Reset();
}

// Load new gamestate with file path


void GameState::AddScene(std::filesystem::path const& _path) { // filesystem
	mScenes.emplace_back(Scene());
	Scene& latestScene{ mScenes.back() };
	
	if (_path.string().size() == 0) {
		static int newSceneCount = 1;
		latestScene.mName = "New Scene " + std::to_string(newSceneCount++);  //cannot have same GS name
		LOG_CUSTOM("GAMESTATE", "Adding NEW scene to gamestate: " + mName);
	} else {
		latestScene.Load(_path);
		LOG_CUSTOM("GAMESTATE", "Adding scene \"" + _path.stem().string() + "\" to gamestate: " + mName);
	}

#ifndef _EDITOR
	//if(!editorManager->IsScenePaused())
		for (auto e : latestScene.mEntities)
			if (e.HasComponent<Script>()) logicSystem->Alive(e);
#endif

	if (!latestScene.mIsPause) latestScene.Init();
}
void GameState::RemoveScene(std::string const& _name){
	LOG_CUSTOM("GAMESTATE", "Attempting to remove scene \"" + _name + "\" from gamestate: " + mName);
	
	for (auto it = mScenes.begin(); it != mScenes.end(); ++it) {
		if (it->mName == _name) {
			it->Unload();
			mScenes.erase(it);
			break;
		}
	}
}

void GameState::Load(std::filesystem::path const& _path){
	LOG_CUSTOM("GAMESTATE", "Load GameState: " + _path.string());
	// Load relevant resources here, Game Mode only
#ifndef _EDITOR
	//ResourceManager::GetInstance()->LoadGameStateResources(_path);
#endif
	serializationManager->LoadGameState(*this, _path);
	for (auto& scene : mScenes) {
		std::filesystem::path path{ ResourceManager::GetInstance()->FileTypePath(ResourceManager::E_RESOURCETYPE::scene).string() + scene.mName + ".json"};
		scene.Load(path);
	}
}

void GameState::Save() {
	LOG_CUSTOM("GAMESTATE", "Save GameState: " + mName);
	serializationManager->SaveGameState(*this);
}

void GameState::Unload() {
	LOG_CUSTOM("GAMESTATE", "Attempt to unload GameState: " + mName);
	for (auto& scene : mScenes)
		scene.Unload();
	mScenes.clear();
#ifndef _EDITOR
	// ResourceManager::GetInstance()->SelectiveUnloadAllResources();
#endif
}






































