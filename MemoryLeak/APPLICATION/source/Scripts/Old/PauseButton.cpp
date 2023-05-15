/*!*****************************************************************************
\file PauseButton.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
The PauseButton script pauses the game on button press.
*******************************************************************************/

#include "PauseButton.h"

REGISTER_SCRIPT(ScriptComponent, PauseButton);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void PauseButton::Alive(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void PauseButton::Init(Entity const& _e) {
	(void)_e;
	init = false;
	//LOG_INFO("Back to Main Menu button script starts works!!!");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void PauseButton::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void PauseButton::Update(Entity const& _e) {
	if (!init && _e.HasComponent<Button>()) {
		init = true;
		_e.GetComponent<Button>().activated = false;
	}
	if (_e.HasComponent<Button>()) {
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
		if (_e.GetComponent<Button>().activated) {
			init = false;
			Scene& menuScene{ VI::iScene::Select("Menu_Main") };
			if (menuScene.mName != "Error") { // If it's at main menu
				(VI::iScene::Select("Quit Confirmation")).Pause(true);
				menuScene.Pause(false);
			} else { // if it's game pause
				(VI::iScene::Select("Settings")).Pause(true);
				(VI::iScene::Select("How_To_Play")).Pause(true);
				(VI::iScene::Select("Level1")).Pause(true);
				(VI::iScene::Select("Quit Confirmation")).Pause(true);
				(VI::iScene::Select("Pause")).Pause(false);
			}
		}
	}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void PauseButton::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void PauseButton::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void PauseButton::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Back to Main Menu button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void PauseButton::Dead(Entity const& _e) {
	(void)_e;
}
