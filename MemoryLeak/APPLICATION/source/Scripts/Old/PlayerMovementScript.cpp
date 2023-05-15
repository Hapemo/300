/*!*****************************************************************************
\file PlayerMovementScript.cpp
\author Lee Hsien Wei, Joachim, Kew Yu Jun, Chen Jia Wen
\par DP email: l.hsienweijoachim@digipen.edu, c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-11-2022
\brief
The PlayerMovementScript allows the player to move around the map according to
the mouse click position. It also handles the sprites changes according to the
direction the boat is facing. The end dialogue scene camera zoom out is also
handled here.

Press "CTRL+SHIFT+L" to skip to the game over screen.
Press "CTRL+SHIFT+M" to toggle player invincible mode.
Press "CTRL+SHIFT+B" to toggle player's speed increase.
Press "ESC" to toggle the pause menu.
*******************************************************************************/

#include "PlayerMovementScript.h"

REGISTER_SCRIPT(ScriptComponent, PlayerMovementScript);

namespace {
	static bool speedCheat{ false };
}

void PlayerMovementScript::Alive(const Entity& _e) {
	(void)_e;
}

void PlayerMovementScript::Init(const Entity& _e) {
	(void)_e;
	inited = false;
}

void PlayerMovementScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void PlayerMovementScript::Update(const Entity& _e) {
	//if (Input::CheckKey(PRESS, SPACE)) std::cout << "MousePos: " << FUNC->GetWorldMousePos() << '\n';
	/*if (!VI::iAudio::IsBgmPlaying((int)E_AUDIO_CHANNEL::MAINBACKGROUND))
		VI::iAudio::PlayBGM("Bon_Voyage_BGM", (int)E_AUDIO_CHANNEL::MAINBACKGROUND);*/
	//if (_e.HasComponent<Audio>())
		//_e.GetComponent<Audio>().sound.volume = 0.0f;

	if (!inited) {
		littleGirl = VI::iEntity::GetEntity("ActivateLittleGirlScript", "Level1");
		dialogueText = VI::iEntity::GetEntity("DialogueText", "Level1");
		water = VI::iEntity::GetEntity("Water", "Level1");
		enemy = VI::iEntity::GetEntity("Enemy", "Level1");
		if (_e.HasComponent<Audio>()) {
			_e.GetComponent<Audio>().sound.toPlay = true;
			_e.GetComponent<Audio>().sound.isLoop = true;
		}
		currScene = &(VI::iScene::Select("Level1"));
		currCamera = &VI::iCamera::CurrentCamera();
		initialCamScale = currCamera->scale;
		inited = true;
	}
	if (dialogueZoomOut == true)
		if (currCamera->scale.x < initialCamScale.x)
			currCamera->scale.x += 500 * (float)FUNC->GetDeltaTime();
	_e.GetComponent<Transform>().scale.x = std::abs(_e.GetComponent<Transform>().scale.x);

	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::ESCAPE)) {
		(VI::iScene::Select("Settings")).Pause(true);
		(VI::iScene::Select("How_To_Play")).Pause(true);
		(VI::iScene::Select("Level1")).Pause(true);
		(VI::iScene::Select("Game Over")).Pause(true);
		(VI::iScene::Select("Pause")).Pause(false);
		VI::iAudio::ForcePlay("Button_Click_SFX");
	}

	if (VI::iInput::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL) && VI::iInput::CheckKey(E_STATE::HOLD, E_KEY::LEFT_SHIFT)) {
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::L)) {
			(VI::iScene::Select("Level1")).Pause(true);
			(VI::iScene::Select("Game Over")).Pause(false);
			_e.Deactivate();
		}
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::M)) canDie = !canDie;
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::B)) speedCheat = !speedCheat; // speed cheat toggle
	}

	if (VI::iInput::CheckKey(E_STATE::HOLD, M_BUTTON_L) /* && (!VI::iPhysics::IsCollided(enemy, _e) || !canDie) */) {
		if ((dialogueText.HasComponent<General>() && dialogueText.GetComponent<General>().isActive == false)/*|| _e.GetComponent<General>().name != "Boat"*/) {
			/*Math::Vec2 dirVector{ FUNC->GetWorldMousePos() + currCamera->translation - _e.GetComponent<Transform>().translation };
			if (dirVector.SqMagnitude() > FLT_EPSILON * FLT_EPSILON)
				FUNC->ApplyImpulseByEntity(_e, (dirVector.Normalized() * playerSpeed * (speedCheat ? speedCheatMultiplier : 1)) * (float)FUNC->GetDeltaTime(), Math::Vec2{ 0.f, 0.f });
			if (_e.HasComponent<Audio>())
				_e.GetComponent<Audio>().sound.volume = 0.2f;
			float pi = (float) Math::PI;
			float rotation{};
			if (dirVector.y != 0.f && dirVector.x >= 0.f)
				rotation = atan2f(dirVector.y, dirVector.x);
			else if (dirVector.y == 0.f && dirVector.x > 0.f)
				rotation = (float)Math::PI / 2.f;
			else if (dirVector.y != 0.f && dirVector.x < 0.f) {
				rotation = atan2f(dirVector.y, dirVector.x);
				rotation += rotation < 0.f ? pi * 2.f : 0.f;
			} else rotation = 3.f * pi / 2.f;

			static const float miniAngle{ pi / 8.f };
			float tempRotation{ rotation };
			if (tempRotation < 0) tempRotation += 2 * pi;

			if (tempRotation >= 15.f * miniAngle || tempRotation <= miniAngle) FUNC->SetSpriteSheetIndexByEntity(_e, 6);
			else if (tempRotation <= 3.f * miniAngle) FUNC->SetSpriteSheetIndexByEntity(_e, 5);
			else if (tempRotation <= 5.f * miniAngle) FUNC->SetSpriteSheetIndexByEntity(_e, 4);
			else if (tempRotation <= 7.f * miniAngle) FUNC->SetSpriteSheetIndexByEntity(_e, 3);
			else if (tempRotation <= 9.f * miniAngle) FUNC->SetSpriteSheetIndexByEntity(_e, 2);
			else if (tempRotation <= 11.f * miniAngle) FUNC->SetSpriteSheetIndexByEntity(_e, 1);
			else if (tempRotation <= 13.f * miniAngle) FUNC->SetSpriteSheetIndexByEntity(_e, 0);
			else FUNC->SetSpriteSheetIndexByEntity(_e, 7);*/

			if (VI::iPhysics::IsCollided(_e, littleGirl)) {
				if (dialogueActivated == false) {
					dialogueText.Activate();
					dialogueActivated = true;
					dialogueZoomOut = false;
				}
			}
			else {
				dialogueActivated = false;
				dialogueZoomOut = true;
			}
		}
		else {
			if (VI::iPhysics::IsCollided(_e, littleGirl)) {
				dialogueZoomOut = true;
			}
		}
	}

	Transform eXform = _e.GetComponent<Transform>();
	Transform waterXform = water.GetComponent<Transform>();
	Math::Vec2 waterMin = waterXform.translation - waterXform.scale / 2.f + currCamera->scale / 2.f;
	Math::Vec2 waterMax = waterXform.translation + waterXform.scale / 2.f - currCamera->scale / 2.f;

	if (eXform.translation.x < waterMin.x)
		_e.GetComponent<Transform>().translation.x = waterMin.x;
	else if (eXform.translation.x > waterMax.x)
		_e.GetComponent<Transform>().translation.x = waterMax.x;
	if (eXform.translation.y < waterMin.y)
		_e.GetComponent<Transform>().translation.y = waterMin.y;
	else if (eXform.translation.y > waterMax.y)
		_e.GetComponent<Transform>().translation.y = waterMax.y;

	if (currScene)
		currCamera->translation += (_e.GetComponent<Transform>().translation - currCamera->translation) * static_cast<float>(FUNC->GetDeltaTime()) * (speedCheat ? speedCheatMultiplier : 1);
}

void PlayerMovementScript::FixedUpdate(const Entity& _e) {
	(void)_e;
}

void PlayerMovementScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

void PlayerMovementScript::Exit(const Entity& _e) {
	(void)_e;
}

void PlayerMovementScript::Dead(const Entity& _e) {
	(void)_e;
}
