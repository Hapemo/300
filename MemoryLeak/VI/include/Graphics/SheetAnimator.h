/*!*****************************************************************************
\file SheetAnimator.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for an Animator system that operates on
Entities with the Sprite and SheetAnimation Component. It uses !SPRITESHEETS! to 
animate.
*******************************************************************************/
#pragma once
#include "pch.h"
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"
#include "Helper.h"

/*!*****************************************************************************
\brief
SheetAnimator class that operates on Entities containing the Sprite and 
SheetAnimation Component.
*******************************************************************************/
class SheetAnimator : public System
{
public:
	/*!*****************************************************************************
	\brief
	Default constructor for SheetAnimator class.
	*******************************************************************************/
	SheetAnimator() = default;

	/*!*****************************************************************************
	\brief
	Animates a all Entities with Components Sprite and SheetAnimation.

	\return
	None.
	*******************************************************************************/
	void Animate();

	/*!*****************************************************************************
	\brief
	Animates a single Entity with Components Sprite and SheetAnimation.

	\param const Entity& e
	The Entity to be animated.

	\return
	None.
	*******************************************************************************/
	void Animate(const Entity& _e);
private:
	double mAccumulatedDT{ 0.0 };
	const double mFixedDT{ 1.0 / 60.0 };
	const double mAccumulatedDTCap{ 1.0 };
};