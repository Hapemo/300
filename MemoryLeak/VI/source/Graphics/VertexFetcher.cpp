/*!*****************************************************************************
\file VertexFetcher.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 29-10-2022
\brief
This file contains a helper class for fetching vertices from Entities with
Transform and Sprite or Collider Components.
*******************************************************************************/
#include "VertexFetcher.h"
#include "RenderProps.h"
#include "Logger.h"

/*!*****************************************************************************
\brief
Fetch vertices from an Entity depending on its Sprite (Square/Texture/Circle).

\param const Entity& _e
The entity to fetch sprite vertices from.

\return
A vector containing the vertices of the sprite.
*******************************************************************************/
std::vector<Math::Vec2> VertexFetcher::FetchVertices(const Entity& _e)
{
	if (!_e.HasComponent<Transform>())
	{
		PrintDebug("Transform");
		return std::vector<Math::Vec2>();
	}
	if (!_e.HasComponent<Sprite>())
	{
		PrintDebug("Sprite");
		return std::vector<Math::Vec2>();
	}

	Transform xform = _e.GetComponent<Transform>();
	Math::Vec2 pos = xform.translation;
	Sprite sprite = _e.GetComponent<Sprite>();

	switch (sprite.sprite)
	{
	case (SPRITE::SQUARE):
	case (SPRITE::TEXTURE):
	{
		float cosTheta = cosf(xform.rotation);
		float sinTheta = sinf(xform.rotation);
		Math::Vec2 i(cosTheta * xform.scale.x * 0.5f, sinTheta * xform.scale.x * 0.5f),
			j(-sinTheta * xform.scale.y *0.5f, cosTheta * xform.scale.y *0.5f);
		return { pos + i + j, pos - i + j, pos - i - j, pos + i - j };
	}
	case (SPRITE::CIRCLE):
	{
		return { pos };
	}
	default:
	{
		PrintDebug("Non-Debug");
		return std::vector<Math::Vec2>();
	}
	}
}

/*!*****************************************************************************
\brief
Prints debug messages in the case of wrongful use of any functions.

\param const std::string& _missingComponent
The component that is not present in the Entity.
*******************************************************************************/
void VertexFetcher::PrintDebug(const std::string& _missingComponent)
{
	LOG_DEBUG("Entity does not contain " + _missingComponent + " component! Unable to fetch vertices.");
}