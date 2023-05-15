/*!*****************************************************************************
\file WASDScript.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-10-2022
\brief
This file contains the function definitions of the class WASDScript.
The WASDScript class is an example to test the scripting component in the
engine.
*******************************************************************************/

#include "WASDScript.h"

REGISTER_SCRIPT(ScriptComponent, WASDScript);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void WASDScript::Alive(Entity const& _e) {
	(void)_e;
	LOG_INFO("WASDScript alive works!!!");
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void WASDScript::Init(Entity const& _e) {
	(void)_e;
	LOG_INFO("WASDScript init works!!!");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void WASDScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void WASDScript::Update(Entity const& _e) {
	//LOG_INFO("RotateScript updating works!!!");
	if (VI::iInput::CheckKey(E_STATE::HOLD, W)) {
		_e.GetComponent<Transform>().translation.y += 1.f;
	}
	if (VI::iInput::CheckKey(E_STATE::HOLD, A)) {
		_e.GetComponent<Transform>().translation.x -= 1.f;
	}
	if (VI::iInput::CheckKey(E_STATE::HOLD, S)) {
		_e.GetComponent<Transform>().translation.y -= 1.f;
	}
	if (VI::iInput::CheckKey(E_STATE::HOLD, D)) {
		_e.GetComponent<Transform>().translation.x += 1.f;
	}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void WASDScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void WASDScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void WASDScript::Exit(Entity const& _e) {
	(void)_e;
	LOG_INFO("WASDScript exit works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void WASDScript::Dead(Entity const& _e) {
	(void)_e;
	LOG_INFO("WASDScript dead works!!!");
}
