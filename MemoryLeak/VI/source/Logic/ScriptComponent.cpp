/*!*****************************************************************************
\file ScriptComponent.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-10-2022
\brief
This file contains the function definitions of the class ScriptComponent.
The ScriptComponent class is the base class for the scripting component in the
engine.
*******************************************************************************/

#include "ScriptComponent.h"

/*!*****************************************************************************
\brief
Alive script will run when the gamestate of the entity is activated.
*******************************************************************************/
void ScriptComponent::Alive(Entity const& _e) {
	(void)_e;
	LOG_INFO("Base script alive works!!!");
}

/*!*****************************************************************************
\brief
Start script to run on initialisation.
*******************************************************************************/
void ScriptComponent::Init(Entity const& _e) {
	(void)_e;
	LOG_INFO("Base script init works!!!");
}

/*!*****************************************************************************
\brief
Update script to run every update function call while the entity is active.
*******************************************************************************/
void ScriptComponent::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Update script to run every update function call while the entity is active after
running all EarlyUpdate	functions from all other active scripts.
*******************************************************************************/
void ScriptComponent::Update(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Base script updating works!!!");
}

/*!*****************************************************************************
\brief
Fixed update script to run on fixed delta time after running all EarlyUpdate
and Update functions from all other active scripts.
*******************************************************************************/
void ScriptComponent::FixedUpdate(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Base script fixed delta time works!!!");
}

/*!*****************************************************************************
\brief
Update script to run every update function call while the entity is active after
running all	EarlyUpdate, Update, and FixedUpdate functions from all other active
scripts.
*******************************************************************************/
void ScriptComponent::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
End script to run when the entity gets destroyed/exit.
*******************************************************************************/
void ScriptComponent::Exit(Entity const& _e) {
	(void)_e;
	LOG_INFO("Base script exit works!!!");
}

/*!*****************************************************************************
\brief
Dead script will run when the gamestate of the entity exits.
*******************************************************************************/
void ScriptComponent::Dead(Entity const& _e) {
	(void)_e;
	LOG_INFO("Base script dead works!!!");
}
