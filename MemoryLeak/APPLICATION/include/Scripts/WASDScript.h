#pragma once
/*!*****************************************************************************
\file WASDScript.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-10-2022
\brief
This file contains the function declarations of the class WASDScript.
The WASDScript class is an example to test the scripting component in the
engine.
*******************************************************************************/

#pragma once
#include "ECS_tools.h"
#include "ScriptComponent.h"
#include "ScriptManager.h"

class WASDScript : public ScriptComponent {
private:
	std::string name = "WASDScript";

public:
	WASDScript() = default;
	virtual ~WASDScript() = default;

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
	Fixed update script to run on fixed delta time after running all EarlyUpdate
	and Update functions from all other active scripts.
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
