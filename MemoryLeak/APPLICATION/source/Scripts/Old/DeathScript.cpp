/*!*****************************************************************************
\file DeathScript.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 25-11-2022
\brief
The DeathScript runs the player's death animation on the boat collides with
the enemy.

Press "CTRL+SHIFT+M" to toggle player invincible mode.
*******************************************************************************/

#include "DeathScript.h"

REGISTER_SCRIPT(ScriptComponent, DeathScript);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void DeathScript::Alive(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void DeathScript::Init(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Death script starts works!!!");
	player = VI::iEntity::GetEntity("Boat", "Level1");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void DeathScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void DeathScript::Update(Entity const& _e) {
	if (VI::iInput::CheckKey(HOLD, LEFT_CONTROL) && VI::iInput::CheckKey(HOLD, LEFT_SHIFT) && VI::iInput::CheckKey(PRESS, M)) canDie = !canDie;

	static bool onEntry = false;
	if (VI::iPhysics::IsCollided(player, _e)) onEntry = true;
	if (onEntry && canDie) {
		static bool capsized = false;
		VI::iTexture::SetTexture(player, "Textures\\Spritesheets\\BOAT\\capsize\\Props_Boat_NE_Capsize_Spritesheet.png");
		if (capsized == false) {
			player.GetComponent<SheetAnimation>().frameCount = 11;
			player.GetComponent<SheetAnimation>().timePerFrame = 0.15f;
			player.GetComponent<SheetAnimation>().currFrameIndex = 0;
			capsized = true;
		}

		if (player.GetComponent<SheetAnimation>().currFrameIndex == player.GetComponent<SheetAnimation>().frameCount - 1) {
			player.Deactivate();
			(VI::iScene::Select("Level1")).Pause(true);
			(VI::iScene::Select("Game Over")).Pause(false);
			onEntry = false;
			capsized = false;
		}
	}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void DeathScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void DeathScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void DeathScript::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Death script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void DeathScript::Dead(Entity const& _e) {
	(void)_e;
}
