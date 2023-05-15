/*!*****************************************************************************
\file ScaleScript.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-10-2022
\brief
This file contains the function definitions of the class ScaleScript.
The ScaleScript class is an example to test the scripting component in the
engine.
*******************************************************************************/

#include "ScaleScript.h"

REGISTER_SCRIPT(ScriptComponent, ScaleScript);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void ScaleScript::Alive(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void ScaleScript::Init(Entity const& _e) {
	(void)_e;
	LOG_INFO("ScaleScript starts works!!!");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void ScaleScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void ScaleScript::Update(Entity const& _e) {
	//LOG_INFO("ScaleScript updating works!!!");
	if (Input::CheckKey(E_STATE::HOLD, Y)) {
		_e.GetComponent<Transform>().scale += 2;
	}
	else if (Input::CheckKey(E_STATE::HOLD, U)) {
		_e.GetComponent<Transform>().scale -= 2;
	}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void ScaleScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void ScaleScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void ScaleScript::Exit(Entity const& _e) {
	(void)_e;
	LOG_INFO("ScaleScript end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void ScaleScript::Dead(Entity const& _e) {
	(void)_e;
}
