/*!*****************************************************************************
\file BackToGameButton.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-11-2022
\brief
The BackToGameButton script returns the scene to the gameplay on button press.
Press "ESC" to toggle the pause menu.
*******************************************************************************/

#include "BackToGameButton.h"

REGISTER_SCRIPT(ScriptComponent, BackToGameButton);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void BackToGameButton::Alive(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void BackToGameButton::Init(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Back to Main Menu button script starts works!!!");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void BackToGameButton::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void BackToGameButton::Update(Entity const& _e) {
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

	if (_e.HasComponent<Button>() && _e.GetComponent<Button>().activated) {
		activate = false;
		(VI::iScene::Select("Settings")).Pause(true);
		(VI::iScene::Select("How_To_Play")).Pause(true);
		(VI::iScene::Select("Pause")).Pause(true);
		(VI::iScene::Select("Quit Confirmation")).Pause(true);
		(VI::iScene::Select("Game Over")).Pause(true);
		(VI::iScene::Select("Level1")).Pause(false);
	}

	if (activate && VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::ESCAPE)) {
		activate = false;
		(VI::iScene::Select("Settings")).Pause(true);
		(VI::iScene::Select("How_To_Play")).Pause(true);
		(VI::iScene::Select("Pause")).Pause(true);
		(VI::iScene::Select("Quit Confirmation")).Pause(true);
		(VI::iScene::Select("Game Over")).Pause(true);
		(VI::iScene::Select("Level1")).Pause(false);
	}

	if (VI::iInput::CheckKey(E_STATE::RELEASE, E_KEY::ESCAPE)) activate = true;
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time.
*******************************************************************************/
void BackToGameButton::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void BackToGameButton::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void BackToGameButton::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Back to Main Menu button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void BackToGameButton::Dead(Entity const& _e) {
	(void)_e;
}
