/*!*****************************************************************************
\file SpriteVariable.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains enums and structs for what is contained in a Sprite 
Component.
*******************************************************************************/
#pragma once
#include "pch.h"

/*!*****************************************************************************
\brief
A Sprite enum that is used to determine what type of sprite should be rendered.
*******************************************************************************/
enum class SPRITE
{
	SQUARE,
	CIRCLE,
	TEXTURE,
	DEBUG_POINT,
	DEBUG_LINE,
	DEBUG_SQUARE,
	DEBUG_CIRCLE,
	DEBUG_ARROW
};

/*!*****************************************************************************
\brief
A struct containing the color attributes of an Entity.
*******************************************************************************/
struct Color
{
	GLubyte r, g, b, a;
};