/*!*****************************************************************************
\file	MoveMinimapScript.h
\author Kew Yu Jun
\par	DP email: k.yujun@digipen.edu
\par	Course: GAM200
\par	Group: Memory Leak Studios
\date	26-11-2022
\brief  This file contains the declaration of the MoveMinimapScript class
		that defines the game logic used to move the minimap
*******************************************************************************/
#include "MoveMinimapScript.h"

REGISTER_SCRIPT(ScriptComponent, MoveMinimapScript);

void MoveMinimapScript::Alive(const Entity& _e) {
	(void)_e;
}

void MoveMinimapScript::Init(const Entity& _e) {
	(void)_e;
	inited = false;
}

void MoveMinimapScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void MoveMinimapScript::Update(const Entity& _e) {
	if (!inited)
	{
		currScene = &(VI::iScene::Select("Level1"));
		currCamera = &VI::iCamera::CurrentCamera();
		originalTranslate = _e.GetComponent<Transform>().translation;
		originalScale = _e.GetComponent<Transform>().scale;
		inited = true;
	}
	_e.GetComponent<Transform>().translation = currCamera->translation + originalTranslate / currCamera->rotation;
	_e.GetComponent<Transform>().scale = originalScale / currCamera->rotation;
}

void MoveMinimapScript::FixedUpdate(const Entity& _e) {
	(void)_e;
}

void MoveMinimapScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

void MoveMinimapScript::Exit(const Entity& _e) {
	(void)_e;
}

void MoveMinimapScript::Dead(const Entity& _e) {
	(void)_e;
}
