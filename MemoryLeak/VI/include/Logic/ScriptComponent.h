/*!*****************************************************************************
\file ScriptComponent.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-10-2022
\brief
This file contains the function declarations of the class ScriptComponent.
The ScriptComponent class is the base class for the scripting component in the
engine.
*******************************************************************************/

#pragma once
#include "ECS_items.h"
#include "Logger.h"
#include "InternalCalls.h"

class ScriptComponent {
private:
	std::string name = "Base";
public:
	/*!*****************************************************************************
	\brief
	Default constructor and destructor.
	*******************************************************************************/
	ScriptComponent() = default;
	virtual ~ScriptComponent() = default;

	/*!*****************************************************************************
	\brief
	Delete copy constructor.
	*******************************************************************************/
	ScriptComponent(const ScriptComponent&) = delete;
	const ScriptComponent& operator=(const ScriptComponent&) = delete;

	/*!*****************************************************************************
	\brief
	Alive script will run when the gamestate of the entity is activated.
	*******************************************************************************/
	virtual void Alive(Entity const& _e);

	/*!*****************************************************************************
	\brief
	Init script to run on initialisation.
	*******************************************************************************/
	virtual void Init(Entity const& _e);

	/*!*****************************************************************************
	\brief
	Function will run on every update while the entity is active.
	*******************************************************************************/
	virtual void EarlyUpdate(Entity const& _e);

	/*!*****************************************************************************
	\brief
	Update script to run every update function call while the entity is active after
	running all EarlyUpdate	functions from all other active scripts.
	*******************************************************************************/
	virtual void Update(Entity const& _e);

	/*!*****************************************************************************
	\brief
	Fixed update script to run on fixed delta time after running all EarlyUpdate
	and Update functions from all other active scripts.
	*******************************************************************************/
	virtual void FixedUpdate(Entity const& _e);

	/*!*****************************************************************************
	\brief
	Late update script will run on every update while the entity is active after
	running all	EarlyUpdate, Update, and FixedUpdate functions from all other active
	scripts.
	*******************************************************************************/
	virtual void LateUpdate(Entity const& _e);

	/*!*****************************************************************************
	\brief
	Exit script to run when the entity gets destroyed/exit.
	*******************************************************************************/
	virtual void Exit(Entity const& _e);

	/*!*****************************************************************************
	\brief
	Dead script will run when the gamestate of the entity exits.
	*******************************************************************************/
	virtual void Dead(Entity const& _e);

	/*!*****************************************************************************
	\brief
	Retrieve the name of the script.
	*******************************************************************************/
	inline std::string GetName() { return name; }

	/*!*****************************************************************************
	\brief
	Retrieve the script object.
	*******************************************************************************/
	inline ScriptComponent* GetScript() { return this; }
};
