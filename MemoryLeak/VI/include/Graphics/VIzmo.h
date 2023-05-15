/*!*****************************************************************************
\file VIzmo.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for a Gizmo class used in the editor.
*******************************************************************************/
#pragma once
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"
#include "VIzmoProps.h"
#include "Camera.h"

/*!*****************************************************************************
\brief
Enum class for buttons in the gizmo.
*******************************************************************************/
enum class GIZMO_BUTTON { SCALE_X, SCALE_Y, ROTATE, TRANSLATE, NONE };

/*!*****************************************************************************
\brief
Class that encapsulates a gizmo.
*******************************************************************************/
class VIzmo
{
public:
	/*!*****************************************************************************
	\brief
	Default constructor for the Gizmo.
	*******************************************************************************/
	VIzmo();

	/*!*****************************************************************************
	\brief
	Attach the gizmo to an entity.

	\param const Entity& _e
	Entity to attach the gizmo to.
	*******************************************************************************/
	void Attach(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Returns the entity the gizmo is currently attached to.

	\return
	The entity the gizmo is attached to.
	*******************************************************************************/
	Entity GetAttached();

	/*!*****************************************************************************
	\brief
	Detaches the Gizmo from the entity.
	*******************************************************************************/
	void Detach();

	/*!*****************************************************************************
	\brief
	Update loop for gizmo. checks if buttons are pressed and modify the attached 
	entity accordingly.

	\param const Math::Vec2& _mouseCoordinates
	Mouse coordinates to check for, typically editor mouse coordinates.

	\param Camera& _cam
	Camera for view transforms.
	*******************************************************************************/
	void Update(const Math::Vec2& _mouseCoordinates, Camera& _cam);
private:
	Entity mAttached;
	GIZMO_BUTTON mSelected;
	std::unordered_map<GIZMO_BUTTON, Math::Vec2> mButtonPos;
};