/*!*****************************************************************************
\file TransformManager.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definitions for a TransformManager system to 
manipulate its components.
*******************************************************************************/
#include "Graphics/TransformManager.h"

//------------------------------------------------------------------------------
// Getter and Setters
//------------------------------------------------------------------------------
void TransformManager::SetScale(const Entity& _e, const Math::Vec2& _scale)
{
	_e.GetComponent<Transform>().scale = _scale;
}

void TransformManager::SetRotation(const Entity& _e, float _rotation)
{
	_e.GetComponent<Transform>().rotation = _rotation;
}

void TransformManager::SetTranslate(const Entity& _e, const Math::Vec2& _translation)
{
	_e.GetComponent<Transform>().translation = _translation;
}
