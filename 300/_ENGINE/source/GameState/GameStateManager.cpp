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
#include "ConfigManager.h"
//#include "PerformanceVisualiser.h"
//#include "ResourceManager.h"
//#include "Graphics/RenderManager.h"
//#include "Graphics/SpriteManager.h"
#include "Input.h"

//Scene GameStateManager::mErrorScene("Error");
//const Entity GameStateManager::mErrorEntity(0);
GameStateManager::GameStateManager() : mErrorScene("Error"), mGSMState(E_GSMSTATE::NONE), mCurrentGameState(), mNextGSName(ConfigManager::GetValue("StartingGameState")) {};



// Load the first game state.
void GameStateManager::Init() {
	mCurrentGameState.Load(mNextGSName);
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
		mCurrentGameState.Load(mNextGSName);
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
	//audioManager->StopAllSound(); TODO minglun
	mNextGSName = _name;
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

	mCurrentGameState.AddScene();
}

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
	mNextGSName = "";
}

bool GameStateManager::GameStateJsonExist(std::string const& _name) {
	std::ifstream f(ConfigManager::GetValue("GameStatePath") + _name + ".json");
	return f.is_open();
}

bool GameStateManager::SceneJsonExist(std::string const& _name) {
	std::ifstream f(ConfigManager::GetValue("ScenePath") + _name + ".json");
	return f.is_open();
}

bool GameStateManager::TransferEntity(std::string const& _srcName, std::string const& _dstName, Entity _e) {
	Scene* src = mCurrentGameState.GetScene(_srcName);
	Scene* dst = mCurrentGameState.GetScene(_dstName);
	if (*src == mErrorScene) {
		std::cout << "Unable to find" << _srcName << " when attempting to TransferEntity()\n";
		return false;
	}
	if (*dst == mErrorScene) {
		std::cout << "Unable to find" << _dstName << " when attempting to TransferEntity()\n";
		return false;
	}
	
	return TransferEntity(&*src, &*dst, std::move(_e));
}

bool GameStateManager::TransferEntity(int _srcID, int _dstID, Entity _e) {
	TransferEntity(&mCurrentGameState.mScenes[_srcID], &mCurrentGameState.mScenes[_dstID], _e);
}

bool GameStateManager::TransferEntity(Scene* _src, Scene* _dst, Entity _e) {
	auto it = _src->mEntities.find(_e);
	if (it == _src->mEntities.end()) {
		std::cout << "Unable to find Entity in " << _src->mName << " when attempting to TransferEntity()\n";
		return false;
	}

	_src->mEntities.erase(it);

	if (_e.HasParent())
		Entity(_e.GetComponent<Parent>().mParent).RemoveChild(_e);

	std::vector<Entity> children = _e.GetAllChildren();
	for (Entity& child : children) {
		auto childIt = _src->mEntities.find(child);
		if (childIt == _src->mEntities.end())
			_e.RemoveChild(child);
		else
			_src->mEntities.erase(childIt);
	}

	for (Entity& child : _e.GetAllChildren())
		_dst->mEntities.insert(child);
	_dst->mEntities.insert(_e);
}

bool GameStateManager::DeleteEntityFromScene(int _sceneID, Entity _e) {
	mCurrentGameState.mScenes[_sceneID].RemoveEntity(_e);
}






