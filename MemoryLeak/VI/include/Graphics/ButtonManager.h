/*!*****************************************************************************
\file ButtonManager.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-11-2022
\brief
This file contains function declarations for a TransformManager system to
manipulate its components.
*******************************************************************************/
#pragma once
#include "pch.h"
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"

/*!*****************************************************************************
\brief
ButtonManager Class that handles the getting and setting of varibles in the
Button Component.
*******************************************************************************/
class ButtonManager : public System
{
public:
	/*!*****************************************************************************
	\brief
	Default Constructor for ButtonManager class.
	*******************************************************************************/
	ButtonManager() : mWindowWidth(nullptr), mWindowHeight(nullptr), mInitialWidth(0), mInitialHeight(0) {};

	/*!*****************************************************************************
	\brief
	Initializes the ButtonManager.

	\param int* _windowWidth
	The window width pointer.

	\param int* _windowHeight
	The window height pointer.
	*******************************************************************************/
	void Init(int* _windowWidth, int* _windowheight);

	/*!*****************************************************************************
	\brief
	Updates the state of all buttons.
	*******************************************************************************/
	void Update();

	void ResetAllButtons();

	bool AllNotHover();

	bool AllNotClick();

private:
	/*!*****************************************************************************
	\brief
	Check if mouse is hovering over the button in the entity.

	\param const Entity& _e
	The entity to check for.
	*******************************************************************************/
	bool CheckHover(const Entity& _e);
	/*!*****************************************************************************
	\brief
	Check if mouse clicked the button in the entity.

	\param const Entity& _e
	The entity to check for.
	*******************************************************************************/
	bool CheckClick(const Entity& _e);
	/*!*****************************************************************************
	\brief
	Check if mouse activated the button in the entity.

	\param const Entity& _e
	The entity to check for.
	*******************************************************************************/
	bool CheckActivate(const Entity& _e);
	int* mWindowWidth, *mWindowHeight;
	int mInitialWidth, mInitialHeight;
};