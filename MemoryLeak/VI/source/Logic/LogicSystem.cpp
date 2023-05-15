/*!*****************************************************************************
\file LogicSystem.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-10-2022
\brief
This file contains the function definitions of the class LogicSystem.
The LogicSystem class handles the C# scripting for the engine.
*******************************************************************************/

#include "LogicSystem.h"
#include "MonoManager.h"
#include "ScriptManager.h"

/*!*****************************************************************************
\brief
Activate logic system in application.
*******************************************************************************/
void LogicSystem::Activate() {
	LOG_DEBUG("Activate Logic System & Mono.");
	LOG_CREATE("Mono");
	MonoManager::GetInstance()->InitMono();
}

/*!*****************************************************************************
\brief
Exit logic system in application.
*******************************************************************************/
void LogicSystem::Close() {
	LOG_DEBUG("Closing Logic System & Mono.");
	MonoManager::GetInstance()->CloseMono();
}

/*!*****************************************************************************
\brief
Run the alive function for all active entities' scripts.
*******************************************************************************/
void LogicSystem::Alive() {
	LOG_DEBUG("Waking entities' scripts.");
	for (Entity const& e : mEntities) Alive(e);
	//GameStateManager::GetReference()->mcurrentGameState->mScenes[]
}

/*!*****************************************************************************
\brief
Run the initialisation function for all active entities' scripts.
*******************************************************************************/
void LogicSystem::Init() {
	LOG_DEBUG("Initialising entities' scripts.");
	for (Entity const& e : mEntities) Init(e);
	//GameStateManager::GetReference()->mcurrentGameState->mScenes[]
}

/*!*****************************************************************************
\brief
Run the early update function for all active entities' scripts.
*******************************************************************************/
void LogicSystem::EarlyUpdate() {
	for (Entity const& e : mEntities) EarlyUpdate(e);
}

/*!*****************************************************************************
\brief
Run the update function for all active entities' scripts.
*******************************************************************************/
void LogicSystem::Update() {
	for (Entity const& e : mEntities) Update(e);
}

/*!*****************************************************************************
\brief
Run the fixed update function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::FixedUpdate() {
	for (Entity const& e : mEntities) FixedUpdate(e);
}

/*!*****************************************************************************
\brief
Run the late update function for all active entities' scripts.
*******************************************************************************/
void LogicSystem::LateUpdate() {
	for (Entity const& e : mEntities) LateUpdate(e);
}

/*!*****************************************************************************
\brief
Run the exit function for all active entities' scripts.
*******************************************************************************/
void LogicSystem::Exit() {
	LOG_DEBUG("Exiting entities' scripts.");
	for (Entity const& e : mEntities) Exit(e);
}

/*!*****************************************************************************
\brief
Run the exit function for all active entities' scripts.
*******************************************************************************/
void LogicSystem::Dead() {
	LOG_DEBUG("Killing entities' scripts.");
	for (Entity const& e : mEntities) Dead(e);
}

/*!*****************************************************************************
\brief
Run the alive function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::Alive(std::set<Entity> const& _entities) {
	for (Entity const& e : _entities) Alive(e);
}

/*!*****************************************************************************
\brief
Run the initialisation function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::Init(std::set<Entity> const& _entities) {
	for (Entity const& e : _entities) Init(e);
}

/*!*****************************************************************************
\brief
Run the early update function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::EarlyUpdate(std::set<Entity> const& _entities) {
	for (Entity const& e : _entities) EarlyUpdate(e);
}

/*!*****************************************************************************
\brief
Run the update function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::Update(std::set<Entity> const& _entities) {
	for (Entity const& e : _entities) Update(e);
}

/*!*****************************************************************************
\brief
Run the fixed update function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::FixedUpdate(std::set<Entity> const& _entities) {
	for (Entity const& e : _entities) FixedUpdate(e);
}

/*!*****************************************************************************
\brief
Run the late update function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::LateUpdate(std::set<Entity> const& _entities) {
	for (Entity const& e : _entities) LateUpdate(e);
}

/*!*****************************************************************************
\brief
Run the exit function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::Exit(std::set<Entity> const& _entities) {
	for (Entity const& e : _entities) Exit(e);
}

/*!*****************************************************************************
\brief
Run the dead function for all entities' scripts given by the parameter.
*******************************************************************************/
void LogicSystem::Dead(std::set<Entity> const& _entities) {
	for (Entity const& e : _entities) Dead(e);
}

/*!*****************************************************************************
\brief
Run the alive function for entity.
*******************************************************************************/
void LogicSystem::Alive(Entity const& _e) {
	RunScript(_e, E_SCRIPTTYPE::ALIVE);
}

/*!*****************************************************************************
\brief
Run the initialisation function for entity.
*******************************************************************************/
void LogicSystem::Init(Entity const& _e) {
	RunScript(_e, E_SCRIPTTYPE::INIT);
}

/*!*****************************************************************************
\brief
Run the early update function for entity.
*******************************************************************************/
void LogicSystem::EarlyUpdate(Entity const& _e) {
	RunScript(_e, E_SCRIPTTYPE::EARLY_UPDATE);
}

/*!*****************************************************************************
\brief
Run the update function for entity.
*******************************************************************************/
void LogicSystem::Update(Entity const& _e) {
	RunScript(_e, E_SCRIPTTYPE::UPDATE);
}

/*!*****************************************************************************
\brief
Run the fixed update function for entity.
*******************************************************************************/
void LogicSystem::FixedUpdate(Entity const& _e) {
	// Increment accumulatedDT by the application's DT
	mAccumulatedDT += FPSManager::dt;

	// Prevent spiral of death
	if (mAccumulatedDT > mAccumulatedDTCap)
		mAccumulatedDT = mAccumulatedDTCap;

	// If the accumlatedDT is larger than or equal to the defined fixedDT,
	// Execute a simulation tick of the physics using the defined fixedDT and subtract that value from accumulatedDT 
	while (mAccumulatedDT >= mFixedDT) {
		RunScript(_e, E_SCRIPTTYPE::FIXED_UPDATE);
		mAccumulatedDT -= mFixedDT;
	}
}

/*!*****************************************************************************
\brief
Run the late update function for entity.
*******************************************************************************/
void LogicSystem::LateUpdate(Entity const& _e) {
	RunScript(_e, E_SCRIPTTYPE::LATE_UPDATE);
}

/*!*****************************************************************************
\brief
Run the exit function for entity.
*******************************************************************************/
void LogicSystem::Exit(Entity const& _e) {
	RunScript(_e, E_SCRIPTTYPE::EXIT);
}

/*!*****************************************************************************
\brief
Run the dead function for entity.
*******************************************************************************/
void LogicSystem::Dead(Entity const& _e) {
	RunScript(_e, E_SCRIPTTYPE::DEAD);
}

/*!*****************************************************************************
\brief
Run specified script for entity.
*******************************************************************************/
void LogicSystem::RunScript(Entity const& _e, E_SCRIPTTYPE _type) {
	if (!_e.ShouldRun() && _type != E_SCRIPTTYPE::DEAD) return;
	if (!_e.HasComponent<Script>()) return;

	std::string scriptName = _e.GetComponent<Script>().name;
	if (scriptName == "") {
		//LOG_ERROR("Entity " + _e.GetComponent<General>().name + " component script name is empty!");
		return;
	}
	// Script is C++ script
	if (ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName) != nullptr) {
		switch (_type) {
		case E_SCRIPTTYPE::ALIVE:
			LOG_INFO("Alive Script for " + scriptName + " ran! (C++)");
			ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName)->Alive(_e);
			break;

		case E_SCRIPTTYPE::INIT:
			LOG_INFO("Init Script for " + scriptName + " ran! (C++)");
			ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName)->Init(_e);
			break;

		case E_SCRIPTTYPE::EARLY_UPDATE:
			ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName)->EarlyUpdate(_e);
			break;

		case E_SCRIPTTYPE::UPDATE:
			ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName)->Update(_e);
			break;

		case E_SCRIPTTYPE::FIXED_UPDATE:
			ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName)->FixedUpdate(_e);
			break;

		case E_SCRIPTTYPE::LATE_UPDATE:
			ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName)->LateUpdate(_e);
			break;

		case E_SCRIPTTYPE::EXIT:
			LOG_INFO("Exit Script for " + scriptName + " ran! (C++)");
			ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName)->Exit(_e);
			break;

		case E_SCRIPTTYPE::DEAD:
			LOG_INFO("Dead Script for " + scriptName + " ran! (C++)");
			ScriptManager<ScriptComponent>::GetInstance()->GetScript(scriptName)->Dead(_e);
			break;

		default:
			LOG_WARN("Invalid script type passed in for " + scriptName + "!");
			break;
		}
		return;
	}

	// Script is C# script
	MonoManager::GetInstance()->RegisterMonoScript("BonVoyage", scriptName);
	if (MonoManager::GetInstance()->GetMonoComponent(scriptName) != nullptr) {
		void* params[1]{ (void*)&_e };
		switch (_type) {
		case E_SCRIPTTYPE::ALIVE:
			LOG_INFO("Alive Script for " + scriptName + " ran! (C#)");
			MonoManager::GetInstance()->CallMethod(scriptName, "Alive", 1, params);
			break;

		case E_SCRIPTTYPE::INIT:
			LOG_INFO("Init Script for " + scriptName + " ran! (C#)");
			MonoManager::GetInstance()->CallMethod(scriptName, "Init", 1, params);
			break;

		case E_SCRIPTTYPE::EARLY_UPDATE:
			MonoManager::GetInstance()->CallMethod(scriptName, "EarlyUpdate", 1, params);
			break;

		case E_SCRIPTTYPE::UPDATE:
			MonoManager::GetInstance()->CallMethod(scriptName, "Update", 1, params);
			break;

		case E_SCRIPTTYPE::FIXED_UPDATE:
			MonoManager::GetInstance()->CallMethod(scriptName, "FixedUpdate", 1, params);
			break;

		case E_SCRIPTTYPE::LATE_UPDATE:
			MonoManager::GetInstance()->CallMethod(scriptName, "LateUpdate", 1, params);
			break;

		case E_SCRIPTTYPE::EXIT:
			LOG_INFO("Exit Script for " + scriptName + " ran! (C#)");
			MonoManager::GetInstance()->CallMethod(scriptName, "Exit", 1, params);
			break;

		case E_SCRIPTTYPE::DEAD:
			LOG_INFO("Dead Script for " + scriptName + " ran! (C#)");
			MonoManager::GetInstance()->CallMethod(scriptName, "Dead", 1, params);
			break;

		default:
			LOG_WARN("Invalid script type passed in for " + scriptName + "!");
			break;
		}
		return;
	}

	// Script doesnt exist
	LOG_ERROR(scriptName + " Script failed to attach or doesn't exist!");
}