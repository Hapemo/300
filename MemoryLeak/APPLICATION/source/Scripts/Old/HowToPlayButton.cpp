/*!*****************************************************************************
\file HowToPlayButton.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-11-2022
\brief
The HowToPlayButton script opens the "How To Play" screen on button press.
*******************************************************************************/

#include "HowToPlayButton.h"

REGISTER_SCRIPT(ScriptComponent, HowToPlayButton);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void HowToPlayButton::Alive(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void HowToPlayButton::Init(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script starts works!!!");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void HowToPlayButton::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void HowToPlayButton::Update(Entity const& _e) {
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
		if (_e.GetComponent<Button>().activated) {
			(VI::iScene::Select("Menu_Main")).Pause(true);
			(VI::iScene::Select("Pause")).Pause(true);
			(VI::iScene::Select("How_To_Play")).Pause(false);
		}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void HowToPlayButton::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void HowToPlayButton::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void HowToPlayButton::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void HowToPlayButton::Dead(Entity const& _e) {
	(void)_e;
}
