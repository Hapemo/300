/*!*****************************************************************************
\file AnimationVariable.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains enums and structs for what is contained in a Sprite
Component.
*******************************************************************************/
#pragma once
/*!*****************************************************************************
\brief
A struct containing the spritesheet info.
*******************************************************************************/
struct SpriteSheet
{
	GLuint sheet = 0;
	int frameCount = 1;
	float timePerFrame = 1.f;
};