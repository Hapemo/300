/*!*****************************************************************************
\file GameStateManager.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Manages the gamestate flow, control which game state is running or will be
running.

In editor mode, gamestate manager init will not load in any gamestate
Editor has to call ChangeGameState("gamestate name") to get a gamestate out.
If editor wants to close a gamestate and have blank state, call
ChangeGameState(E_GSMSTATE::NONE)

M_GSMSTATE::EXIT is only used for closing the game in GAMEMODE only.
*******************************************************************************/
#include "GameState/GameStateManager.h"
#include "GameState/GameState.h"
#include "Helper.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
//#include "PerformanceVisualiser.h"
//#include "ResourceManager.h"
//#include "Graphics/RenderManager.h"
//#include "Graphics/SpriteManager.h"
#include "Input.h"

//Scene GameStateManager::mErrorScene("Error");
//const Entity GameStateManager::mErrorEntity(0);
GameStateManager::GameStateManager() : mErrorScene("Error"), mGSMState(E_GSMSTATE::NONE), mCurrentGameState(), mNextGSPath("") {};

// Load the first game state.
void GameStateManager::Init() {
	//LOG_CREATE("SCENE");
	//LOG_CREATE("GAMESTATE");
	//LOG_CREATE("GAMESTATEMANAGER");

	//mGameStates.emplace_back(GameState());
	//mCurrentGameState = &mGameStates.front();

#ifdef _EDITOR // If editor, don't load in any initial game state
	return;
#endif

	mCurrentGameState.Load(mNextGSPath);
	mCurrentGameState.Init();

	mGSMState = E_GSMSTATE::RUNNING;
}

void GameStateManager::UpdateNextGSMState() {
	if (mGSMState == E_GSMSTATE::RUNNING) return; // No change in GS, ignore.

	mCurrentGameState.Exit();

	switch (mGSMState) {
	case E_GSMSTATE::NONE: // For editor only
		if (mCurrentGameState.mName != "") {
			mCurrentGameState.Unload();
			mCurrentGameState.mName.clear();
		}
		return;
	case E_GSMSTATE::EXIT:
		mCurrentGameState.Unload();
		return;
	case E_GSMSTATE::RESTART:
		mCurrentGameState.Init();
		break;
	case E_GSMSTATE::CHANGING:
		mCurrentGameState.Unload();
		//SetGameState(mNextGSPath);
		mCurrentGameState.Load(mNextGSPath);
		mCurrentGameState.Init();
		break;
	}

	mGSMState = E_GSMSTATE::RUNNING;
}

Scene& GameStateManager::SelectScene(std::string const& _name) {
	for (auto& scene : mCurrentGameState.mScenes)
		if (scene.mName == _name) return scene;

	assert(false && std::string("Unable to select scene: " + _name).c_str());

	return mErrorScene;
}

void GameStateManager::ChangeGameState(std::string const& _name) {
	//LOG_CUSTOM("GAMESTATEMANAGER", "Set gamestate to change to: " + _name);
	//std::string path{ ResourceManager::GetInstance()->FileTypePath(ResourceManager::E_RESOURCETYPE::gamestateEntities).string() + _name + ".json" }; TODO
	//audioManager->StopAllSound(); TODO minglun
	
	//if (!ResourceManager::FileExist(path)) { TODO
	//	LOG_ERROR("Unable to change gamestate to: " + _name);
	//	return;
	//}
	//mNextGSPath = path; TODO
	mGSMState = E_GSMSTATE::CHANGING;
}

void GameStateManager::ChangeGameState(E_GSMSTATE const& _state) {
	//LOG_CUSTOM("GAMESTATEMANAGER", "Set gamestate to change to: " + _state);
	mGSMState = _state;
}

void GameStateManager::Exit() {
	ChangeGameState(E_GSMSTATE::EXIT);
}

void GameStateManager::NewGameState(std::string const& _name) {
	if (!mCurrentGameState.mName.empty())
		mCurrentGameState.Unload();
	mCurrentGameState.mName = _name;
}

//void GameStateManager::AddGameState(std::filesystem::path const& _path) {
//	std::string currName{ mCurrentGameState->mName };
//
//	mGameStates.push_back(GameState());
//	
//	for (auto& gs : mGameStates) 
//		if (gs.mName == currName) 
//			mCurrentGameState = &gs; // This line is required because push_back changes arrangement of gamestates, messing up where mCurrentGameState is pointing at.
//	
//	if (_path.string().size() == 0) {
//		static int newGSCount = 1;
//		mGameStates.back().mName = "New GameState " + std::to_string(newGSCount++);  //cannot have same GS name
//		//LOG_CUSTOM("GAMESTATEMANAGER", "Add NEW gamestate");
//	} else {
//		mGameStates.back().Load(_path);
//		//LOG_CUSTOM("GAMESTATEMANAGER", "Add gamestate: " + _path.string());
//	}
//
//	// Init the gamestate after loading it.
//	// But must temporarily set it to current game state because some of the functions uses that.
//	GameState* tempStoreGS = mCurrentGameState;
//	mCurrentGameState = &mGameStates.back();
//	mGameStates.back().Init();
//	mCurrentGameState = tempStoreGS;
//
//	// FOR EDITOR
//	std::vector<bool> pausedList{};
//	for (Scene& scene : mGameStates.back().mScenes)
//		pausedList.push_back(scene.mIsPause);
//
//	mGameStatesScenesPause[mGameStates.back().mName] = pausedList; // FOR EDITOR
//	SetGameState(_path.stem().string());  //Call set game state to chnage is paused of entites
//	//SetGameState(_path.stem().string());  //Call set game state to chnage is paused of entites
//	//mCurrentGameState = &mGameStates.back();
//}
//
//void GameStateManager::RemoveGameState(GameState* _gameState) {
//	if (!_gameState) {
//		_gameState = mCurrentGameState;
//
//		assert(_gameState && "There is no existing gamestate to be removed");
//		//if (!_gameState) {
//		//	LOG_WARN();
//		//	return;
//		//}
//	}
//
//	for (auto it = mGameStates.begin(); it != mGameStates.end(); ++it) {
//		// Find the game state
//		if (it->mName == _gameState->mName) {
//			// Do necessary unloading 
//			it->Exit();
//			it->Unload();
//			mGameStatesScenesPause.erase(mGameStatesScenesPause.find(it->mName)); // FOR EDITOR
//			std::string currName{ mCurrentGameState->mName };
//			// Erase selected gs from pool
//			if (mCurrentGameState == &*it) {
//				if (mGameStates.empty())
//					mCurrentGameState = nullptr;
//				else mCurrentGameState = &mGameStates.front();
//			}
//			mGameStates.erase(it);
//			//if (!mGameStates.size()) {
//			//	LOG_CUSTOM("GAMESTATEMANAGER", "No more gamestate exists after removing this gamestate: " + _gameState->mName);
//			//	mCurrentGameState = nullptr;
//			//}
//			//for (auto& gs : mGameStates) if (gs.mName == currName) mCurrentGameState = &gs; // This line is required because removing gamestates from vector changes arrangement of gamestates, 
//																																											// messing up where mCurrentGameState is pointing at.
//			//LOG_CUSTOM("GAMESTATEMANAGER", "Removed gamestate: " + _gameState->mName);
//
//			return;
//		}
//	}
//
//	assert(false && std::string("Unable to find gamestate to remove: " + _gameState->mName).c_str());
//}

//bool GameStateManager::SetGameState(std::string const& _name) {
//	for (auto& gs : mGameStates) {
//		if (gs.mName == _name) {
//			// save curr gamestate scene pause, and pause all the scenes
//			std::vector<bool>& currPauseList = mGameStatesScenesPause[mCurrentGameState->mName];
//			currPauseList.clear();
//			currPauseList.resize(mCurrentGameState->mScenes.size());
//			for (size_t i{}; i < mCurrentGameState->mScenes.size(); ++i) {
//				currPauseList[i] = mCurrentGameState->mScenes[i].mIsPause;
//				mCurrentGameState->mScenes[i].Pause(true);
//			}
//
//			// load next gamestate scene pause, and unload all the saved pause status to the scenes
//			mCurrentGameState = &gs;
//			std::vector<bool>& nextPauseList = mGameStatesScenesPause[mCurrentGameState->mName];
//			//if (nextPauseList.size() == 0) {
//			//	LOG_WARN("GameState Scene Pause is 0, gamestate: " + mCurrentGameState->mName);
//			//} else
//			for (size_t i{}; i < mCurrentGameState->mScenes.size(); ++i) {
//				mCurrentGameState->mScenes[i].Pause(nextPauseList[i]);
//			}
//			nextPauseList.clear();
//
//			//LOG_CUSTOM("GAMESTATEMANAGER", "Set gamestate to: " + _name);
//			return 1;
//		}
//	}
//	
//	assert(false && std::string("Unable to find gamestate to set to: " + _name).c_str());
//
//	return 0;
//}

//void GameStateManager::RenameGameState(GameState* _gs, std::string const& _name) {
//	std::string ogName = _gs->mName;
//	mGameStatesScenesPause[_name] = mGameStatesScenesPause[ogName];
//	mGameStatesScenesPause.erase(ogName);
//	_gs->mName = _name;
//}

Entity GameStateManager::GetEntity(std::string const& _entityName, std::string const& _sceneName) {
	for (Scene& scene : mCurrentGameState.mScenes) {
		// If scene is specified, skip those scenes that are not same name.
		if (_sceneName.size() > 0 && _sceneName != scene.mName) continue;

		// Find entity of same name and return
		for (Entity e : scene.mEntities)
			if (e.GetComponent<General>().name == _entityName) {
				//LOG_CUSTOM("GAMESTATEMANAGER", "Get entity: \"" + std::to_string(e.id) + "\" from scene: " + scene.mName);
				return e;
			}
	}
	assert(false && std::string("Unable to get entity: " + _entityName).c_str());

	return systemManager->ecs->NullEntity;
}

void GameStateManager::Unload() {
	//LOG_CUSTOM("GAMESTATEMANAGER", "Unload GameStateManager");
	mCurrentGameState.Exit();
	mCurrentGameState.Unload();
	mNextGSPath = "";
}