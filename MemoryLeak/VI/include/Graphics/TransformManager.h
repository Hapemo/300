/*!*****************************************************************************
\file TransformManager.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for a TransformManager system to
manipulate its components.
*******************************************************************************/
#pragma once
#include <glm/glm.hpp>
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"

/*!*****************************************************************************
\brief
TransformManager Class that handles the getting and setting of varibles in the
Transform Component.
*******************************************************************************/
class TransformManager : public System
{
public:
	/*!*****************************************************************************
	\brief
	Default Constructor for TransformManager class.
	*******************************************************************************/
	TransformManager() = default;

	//------------------------------------------------------------------------------
	// Getter and Setters
	//------------------------------------------------------------------------------
	void			SetScale		(const Entity& _e, const Math::Vec2& _scale);
	void			SetRotation		(const Entity& _e, float _rotation);
	void			SetTranslate	(const Entity& _e, const Math::Vec2& _translation);

	Math::Vec2		GetScale		(const Entity& _e)		{ return _e.GetComponent<Transform>().scale; }
	float			GetRotation		(const Entity& _e)		{ return _e.GetComponent<Transform>().rotation; }
	Math::Vec2		GetTranslate	(const Entity& _e)		{ return _e.GetComponent<Transform>().translation; }
};									 