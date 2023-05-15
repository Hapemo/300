/*!*****************************************************************************
\file AttackScript.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 25-11-2022
\brief
The AttackScript handles the detection of when the player is nearby the enemy,
causing the enemy's rising out of the water animation to run and then run the
attacking animation.
*******************************************************************************/

#pragma once
#include "ECS_tools.h"
#include "ScriptManager.h"
#include "ScriptComponent.h"

class AttackScript : public ScriptComponent {
private:
	std::string name = "AttackScript";
	Entity enemy;
	Entity player;
	size_t visible = 0;
public:
	AttackScript() = default;
	~AttackScript() override = default;

	/*!*****************************************************************************
	\brief
	Function will run when the gamestate of the entity is activated.
	*******************************************************************************/
	void Alive(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function will run on initialisation of the entity.
	*******************************************************************************/
	void Init(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function will run on every update while the entity is active.
	*******************************************************************************/
	void EarlyUpdate(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function will run on every update while the entity is active after running all
	EarlyUpdate functions from all other active scripts.
	*******************************************************************************/
	void Update(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function will run on fixed delta time after running all EarlyUpdate and Update
	functions from all other active scripts.
	*******************************************************************************/
	void FixedUpdate(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function will run on every update while the entity is active after running all
	EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
	*******************************************************************************/
	void LateUpdate(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function will run on exit or when the entity is destroyed.
	*******************************************************************************/
	void Exit(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function will run when the gamestate of the entity exits.
	*******************************************************************************/
	void Dead(Entity const& _e) override;
};
