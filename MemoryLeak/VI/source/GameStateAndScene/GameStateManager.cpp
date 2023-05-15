/*!*****************************************************************************
\file GameStateManager.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Manages the gamestate flow, control which game state is running or will be
running.

For Milestone 3:
Added additional features for editor and game mode to manage the game state flow
*******************************************************************************/
#include "GameStateManager.h"
#include "GameState.h"
#include "Helper.h"
#include "PerformanceVisualiser.h"
#include "ResourceManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/SpriteManager.h"
#include "Input.h"

const std::string GameStateManager::EXIT{ "Exit" };
const std::string GameStateManager::RESTART{ "Restart" };
//Scene GameStateManager::mErrorScene("Error");
//const Entity GameStateManager::mErrorEntity(0);
GameStateManager::E_GSMSTATE GameStateManager::mGSMState{ GameStateManager::E_GSMSTATE::STARTING };

GameStateManager::GameStateManager() : mGameStates(), mCurrentGameState(nullptr), mNextGSPath(""), mErrorEntity(0), mErrorScene("Error") {};

// Load the first game state.
void GameStateManager::Init() {
	LOG_CREATE("SCENE");
	LOG_CREATE("GAMESTATE");
	LOG_CREATE("GAMESTATEMANAGER");

	mGameStates.emplace_back(GameState());
	mCurrentGameState = &mGameStates.front();

	mCurrentGameState->Load(mNextGSPath);
	mCurrentGameState->Init();

	mGameStatesScenesPause.insert({ mCurrentGameState->mName, std::vector<bool>() }); // FOR EDITOR

	mGSMState = E_GSMSTATE::RUNNING;
}

void GameStateManager::Update() {
	mCurrentGameState->Update(); // Game logic
}

void GameStateManager::UpdateNextGSMState() {
	if (mGSMState == E_GSMSTATE::RUNNING) return; // No change in GS, ignore.

	mCurrentGameState->Exit();

	switch (mGSMState) {
	case E_GSMSTATE::EXIT:
		mCurrentGameState->Unload();
		return;
	case E_GSMSTATE::RESTART:
		mCurrentGameState->Init();
		break;
	case E_GSMSTATE::CHANGING:
		// Changing gamestate to some other gamestate.
#ifdef _EDITOR
		std::filesystem::path gsPath{ mNextGSPath };
		std::string gsName{ gsPath.stem().string() };

		if (FindGameState(gsName) != mGameStates.end()) { //if not already loaded
			GameStateManager::GetInstance()->AddGameState(mNextGSPath);
			//GameStateManager::GetInstance()->ChangeGameState(gsPath.stem().string());
			GameStateManager::GetInstance()->SetGameState(gsPath.stem().string());
		} else {
			GameStateManager::GetInstance()->RemoveGameState(mCurrentGameState);
			GameStateManager::GetInstance()->AddGameState(gsPath);
			GameStateManager::GetInstance()->SetGameState(gsPath.stem().string());
		}
#else //GAME MODE
		mCurrentGameState->Unload();
		SetGameState(mNextGSPath);
		mCurrentGameState->Load(mNextGSPath);
#endif // _EDITOR

		mCurrentGameState->Init();
		break;
	}

	mGSMState = E_GSMSTATE::RUNNING;
}

Scene& GameStateManager::SelectScene(std::string const& _name) {
	for (auto& scene : mCurrentGameState->mScenes)
		if (scene.mName == _name) return scene;

	LOG_ERROR("Unable to select scene: " + _name);
	return mErrorScene;
}

void GameStateManager::ChangeGameState(std::string const& _name) {
	LOG_CUSTOM("GAMESTATEMANAGER", "Set gamestate to change to: " + _name);
	std::string path{ ResourceManager::GetInstance()->FileTypePath(ResourceManager::E_RESOURCETYPE::gamestateEntities).string() + _name + ".json" };
	audioManager->StopAllSound();
	// Check if exit or restart;
	if (_name == EXIT)
		mGSMState = E_GSMSTATE::EXIT;
	else if (_name == RESTART)
		mGSMState = E_GSMSTATE::RESTART;
	else if (!ResourceManager::FileExist(path)) {
		LOG_ERROR("Unable to change gamestate to: " + _name);
		return;
	} else {
		mNextGSPath = path;
		mGSMState = E_GSMSTATE::CHANGING;
	}
}

void GameStateManager::GameStateExit() {
	ChangeGameState(EXIT);
}

void GameStateManager::AddGameState(std::filesystem::path const& _path) {
	std::string currName{ mCurrentGameState->mName };

	mGameStates.push_back(GameState());
	
	for (auto& gs : mGameStates) 
		if (gs.mName == currName) 
			mCurrentGameState = &gs; // This line is required because push_back changes arrangement of gamestates, messing up where mCurrentGameState is pointing at.
	
	if (_path.string().size() == 0) {
		static int newGSCount = 1;
		mGameStates.back().mName = "New GameState " + std::to_string(newGSCount++);  //cannot have same GS name
		LOG_CUSTOM("GAMESTATEMANAGER", "Add NEW gamestate");
	} else {
		mGameStates.back().Load(_path);
		LOG_CUSTOM("GAMESTATEMANAGER", "Add gamestate: " + _path.string());
	}

	// Init the gamestate after loading it.
	// But must temporarily set it to current game state because some of the functions uses that.
	GameState* tempStoreGS = mCurrentGameState;
	mCurrentGameState = &mGameStates.back();
	mGameStates.back().Init();
	mCurrentGameState = tempStoreGS;

	// FOR EDITOR
	std::vector<bool> pausedList{};
	for (Scene& scene : mGameStates.back().mScenes)
		pausedList.push_back(scene.mIsPause);

	mGameStatesScenesPause[mGameStates.back().mName] = pausedList; // FOR EDITOR
	SetGameState(_path.stem().string());  //Call set game state to chnage is paused of entites
	//SetGameState(_path.stem().string());  //Call set game state to chnage is paused of entites
	//mCurrentGameState = &mGameStates.back();
}

void GameStateManager::RemoveGameState(GameState* _gameState) {
	if (!_gameState) {
		_gameState = mCurrentGameState;
		if (!_gameState) {
			LOG_WARN("There is no existing gamestate to be removed");
			return;
		}
	}

	for (auto it = mGameStates.begin(); it != mGameStates.end(); ++it) {
		// Find the game state
		if (it->mName == _gameState->mName) {
			// Do necessary unloading 
			it->Exit();
			it->Unload();
			mGameStatesScenesPause.erase(mGameStatesScenesPause.find(it->mName)); // FOR EDITOR
			std::string currName{ mCurrentGameState->mName };
			// Erase selected gs from pool
			if (mCurrentGameState == &*it) {
				if (mGameStates.empty())
					mCurrentGameState = nullptr;
				else mCurrentGameState = &mGameStates.front();
			}
			mGameStates.erase(it);
			//if (!mGameStates.size()) {
			//	LOG_CUSTOM("GAMESTATEMANAGER", "No more gamestate exists after removing this gamestate: " + _gameState->mName);
			//	mCurrentGameState = nullptr;
			//}
			//for (auto& gs : mGameStates) if (gs.mName == currName) mCurrentGameState = &gs; // This line is required because removing gamestates from vector changes arrangement of gamestates, 
																																											// messing up where mCurrentGameState is pointing at.
			LOG_CUSTOM("GAMESTATEMANAGER", "Removed gamestate: " + _gameState->mName);

			return;
		}
	}
	LOG_WARN("Unable to find gamestate to remove: " + _gameState->mName);
}

bool GameStateManager::SetGameState(std::string const& _name) {
	for (auto& gs : mGameStates) {
		if (gs.mName == _name) {
			// save curr gamestate scene pause, and pause all the scenes
			std::vector<bool>& currPauseList = mGameStatesScenesPause[mCurrentGameState->mName];
			currPauseList.clear();
			currPauseList.resize(mCurrentGameState->mScenes.size());
			for (size_t i{}; i < mCurrentGameState->mScenes.size(); ++i) {
				currPauseList[i] = mCurrentGameState->mScenes[i].mIsPause;
				mCurrentGameState->mScenes[i].Pause(true);
			}

			// load next gamestate scene pause, and unload all the saved pause status to the scenes
			mCurrentGameState = &gs;
			std::vector<bool>& nextPauseList = mGameStatesScenesPause[mCurrentGameState->mName];
			//if (nextPauseList.size() == 0) {
			//	LOG_WARN("GameState Scene Pause is 0, gamestate: " + mCurrentGameState->mName);
			//} else
			for (size_t i{}; i < mCurrentGameState->mScenes.size(); ++i) {
				mCurrentGameState->mScenes[i].Pause(nextPauseList[i]);
			}
			nextPauseList.clear();

			LOG_CUSTOM("GAMESTATEMANAGER", "Set gamestate to: " + _name);
			return 1;
		}
	}
	LOG_WARN("Unable to find gamestate to set to: " + _name);
	return 0;
}

void GameStateManager::RenameGameState(GameState* _gs, std::string const& _name) {
	std::string ogName = _gs->mName;
	mGameStatesScenesPause[_name] = mGameStatesScenesPause[ogName];
	mGameStatesScenesPause.erase(ogName);
	_gs->mName = _name;
}

Entity GameStateManager::GetEntity(std::string const& _entityName, std::string const& _sceneName) {
	for (Scene& scene : mCurrentGameState->mScenes) {
		// If scene is specified, skip those scenes that are not same name.
		if (_sceneName.size() > 0 && _sceneName != scene.mName) continue;

		// Find entity of same name and return
		for (Entity e : scene.mEntities)
			if (e.GetComponent<General>().name == _entityName) {
				//LOG_CUSTOM("GAMESTATEMANAGER", "Get entity: \"" + std::to_string(e.id) + "\" from scene: " + scene.mName);
				return e;
			}
	}
	LOG_WARN("Unable to get entity: " + _entityName);
	return mErrorEntity;
}

void GameStateManager::Unload() {
	LOG_CUSTOM("GAMESTATEMANAGER", "Unload GameStateManager");
	for (auto& gameState : mGameStates) {
		gameState.Exit();
		gameState.Unload();
	}
	mCurrentGameState = nullptr;
	mNextGSPath = "";
}