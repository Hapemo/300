/*!*****************************************************************************
\file RotateScript.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-10-2022
\brief
This file contains the function definitions of the class RotateScript.
The RotateScript class is an example to test the scripting component in the
engine.
*******************************************************************************/

#include "RotateScript.h"

REGISTER_SCRIPT(ScriptComponent, RotateScript);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void RotateScript::Alive(Entity const& _e) {
	(void)_e;
	LOG_INFO("RotateScript alive works!!!");
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void RotateScript::Init(Entity const& _e) {
	(void)_e;
	LOG_INFO("RotateScript init works!!!");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void RotateScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void RotateScript::Update(Entity const& _e) {
	//LOG_INFO("RotateScript updating works!!!");
	if (VI::iInput::CheckKey(E_STATE::HOLD, O)) {
		_e.GetComponent<Transform>().rotation += 0.05f;
	}
	else if (VI::iInput::CheckKey(E_STATE::HOLD, P)) {
		_e.GetComponent<Transform>().rotation -= 0.05f;
	}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void RotateScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void RotateScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void RotateScript::Exit(Entity const& _e) {
	(void)_e;
	LOG_INFO("RotateScript exit works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void RotateScript::Dead(Entity const& _e) {
	(void)_e;
	LOG_INFO("RotateScript dead works!!!");
}
