/*!*****************************************************************************
\file ExitGameButton.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-11-2022
\brief
The ExitGameButton script close the program on button press.
*******************************************************************************/

#include "ExitGameButton.h"

REGISTER_SCRIPT(ScriptComponent, ExitGameButton);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void ExitGameButton::Alive(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void ExitGameButton::Init(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script starts works!!!");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void ExitGameButton::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void ExitGameButton::Update(Entity const& _e) {
	static float x = _e.GetComponent<Transform>().scale.x;
	static float y = _e.GetComponent<Transform>().scale.y;
	if (_e.GetComponent<Button>().isClick)
	{
		if (_e.GetComponent<Transform>().scale.x > x - 10.f)
			_e.GetComponent<Transform>().scale.x -= 250.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.y > y - 10.f)
			_e.GetComponent<Transform>().scale.y -= 250.f * (float)FPSManager::dt;

	}
	else if (_e.GetComponent<Button>().isHover) {
		if (_e.GetComponent<Transform>().scale.x < x + 15.f)
			_e.GetComponent<Transform>().scale.x += 100.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.y < y + 15.f)
			_e.GetComponent<Transform>().scale.y += 100.f * (float)FPSManager::dt;

	}
	else
	{
		if (_e.GetComponent<Transform>().scale.x < x)
			_e.GetComponent<Transform>().scale.x += 250.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.y < y)
			_e.GetComponent<Transform>().scale.y += 250.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.x > x)
			_e.GetComponent<Transform>().scale.x -= 250.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.y > y)
			_e.GetComponent<Transform>().scale.y -= 250.f * (float)FPSManager::dt;
	}
	if(_e.HasComponent<Button>())
		if (_e.GetComponent<Button>().activated)
			VI::iGameState::GameStateExit();

	if (VI::iInput::CheckKey(E_STATE::RELEASE, E_KEY::ENTER)) VI::iGameState::GameStateExit();
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void ExitGameButton::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void ExitGameButton::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void ExitGameButton::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void ExitGameButton::Dead(Entity const& _e) {
	(void)_e;
}
