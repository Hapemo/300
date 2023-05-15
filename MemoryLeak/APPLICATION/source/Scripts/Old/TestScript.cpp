/*!*****************************************************************************
\file TestScript.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 02-11-2022
\brief
This file contains the function definitions of the class TestScript.
The TestScript class is an example to test the scripting component in the
engine.
*******************************************************************************/

#include "TestScript.h"

REGISTER_SCRIPT(ScriptComponent, TestScript);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void TestScript::Alive(Entity const& _e) {
	(void)_e;
	LOG_INFO("Test script alive works!!!");
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void TestScript::Init(Entity const& _e) {
	(void)_e;
	LOG_INFO("Test script init works!!!");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void TestScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void TestScript::Update(Entity const& _e) {
	(void)_e;
	static int counter{};
	if (++counter > 500) {
		LOG_INFO("Test script updating works!!!");
		counter = 0;
	}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void TestScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void TestScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void TestScript::Exit(Entity const& _e) {
	(void)_e;
	LOG_INFO("Test script exit works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void TestScript::Dead(Entity const& _e) {
	(void)_e;
	LOG_INFO("Test script dead works!!!");
}
