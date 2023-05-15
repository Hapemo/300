/*!*****************************************************************************
\file VIzmo.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definitions for a Gizmo class used in the editor.
*******************************************************************************/
#include "VIzmo.h"
#include "Input.h"
#include "Helper.h"
/*!*****************************************************************************
\brief
Default constructor for the Gizmo.
*******************************************************************************/
VIzmo::VIzmo() : mAttached(0), mSelected(GIZMO_BUTTON::NONE)
{
	mButtonPos[GIZMO_BUTTON::SCALE_X] = Math::Vec2{ 0, 0 };
	mButtonPos[GIZMO_BUTTON::SCALE_Y] = Math::Vec2{ 0, 0 };
	mButtonPos[GIZMO_BUTTON::ROTATE] = Math::Vec2{ 0, 0 };
	mButtonPos[GIZMO_BUTTON::TRANSLATE] = Math::Vec2{ 0, 0 };
}
/*!*****************************************************************************
\brief
Attach the gizmo to an entity.

\param const Entity& _e
Entity to attach the gizmo to.
*******************************************************************************/
void VIzmo::Attach(const Entity& _e)
{
	mAttached = _e;
}
/*!*****************************************************************************
\brief
Returns the entity the gizmo is currently attached to.

\return
The entity the gizmo is attached to.
*******************************************************************************/
Entity VIzmo::GetAttached()
{
	return mAttached;
}
/*!*****************************************************************************
\brief
Detaches the Gizmo from the entity.
*******************************************************************************/
void VIzmo::Detach()
{
	mAttached = Entity(0);
}
/*!*****************************************************************************
\brief
Update loop for gizmo. checks if buttons are pressed and modify the attached
entity accordingly.

\param const Math::Vec2& _mouseCoordinates
Mouse coordinates to check for, typically editor mouse coordinates.

\param Camera& _cam
Camera for view transforms.
*******************************************************************************/
void VIzmo::Update(const Math::Vec2& _mouseCoordinates, Camera& _cam)
{
	static float scaleX{};
	static float scaleY{};
	if (!mAttached.id)
		return;
	Math::Vec2 mPos = _mouseCoordinates * _cam.GetZoom() + _cam.GetPos();
	Transform entityTransform = mAttached.GetComponent<Transform>();
	mButtonPos[GIZMO_BUTTON::SCALE_X] = entityTransform.translation + Math::Vec2{ GIZMO_LINE_SIZE / 2.f, 0.f } * _cam.GetZoom();
	mButtonPos[GIZMO_BUTTON::SCALE_Y] = entityTransform.translation + Math::Vec2{ 0.f, GIZMO_LINE_SIZE / 2.f }*_cam.GetZoom();
	mButtonPos[GIZMO_BUTTON::ROTATE] = entityTransform.translation + Math::Vec2(cosf(entityTransform.rotation + (float)Math::PI / 2.f),
		sinf(entityTransform.rotation + (float)Math::PI / 2.f)) * GIZMO_CIRCLE_SIZE / 2.f * _cam.GetZoom();
	mButtonPos[GIZMO_BUTTON::TRANSLATE] = entityTransform.translation;

	if (Input::CheckKey(E_STATE::PRESS, E_KEY::M_BUTTON_L))
	{
		for (auto itr = mButtonPos.begin(); itr != mButtonPos.end(); ++itr)
			if (std::powf(mPos.x - itr->second.x, 2) + std::powf(mPos.y - itr->second.y, 2) < std::powf(GIZMO_BUTTON_SIZE / 2.f * _cam.GetZoom(), 2))
				mSelected = itr->first;
		if (mSelected == GIZMO_BUTTON::SCALE_X)
			scaleX = mAttached.GetComponent<Transform>().scale.x;
		if (mSelected == GIZMO_BUTTON::SCALE_Y)
			scaleY = mAttached.GetComponent<Transform>().scale.y;
		return;
	}
	if (Input::CheckKey(E_STATE::RELEASE, E_KEY::M_BUTTON_L))
	{
		mSelected = GIZMO_BUTTON::NONE;
		return;
	}
	if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_L))
	{
		switch (mSelected)
		{
		case GIZMO_BUTTON::SCALE_X:
		{
			float difference = mPos.x - mAttached.GetComponent<Transform>().translation.x;
			mAttached.GetComponent<Transform>().scale.x = scaleX + difference * 2.f - 
				(mButtonPos[GIZMO_BUTTON::SCALE_X].x - mAttached.GetComponent<Transform>().translation.x) * 2.f;
			break;
		}
		case GIZMO_BUTTON::SCALE_Y:
		{
			float difference = mPos.y - mAttached.GetComponent<Transform>().translation.y;
			mAttached.GetComponent<Transform>().scale.y = scaleY + difference * 2.f - 
				(mButtonPos[GIZMO_BUTTON::SCALE_Y].y - mAttached.GetComponent<Transform>().translation.y) * 2.f;
			break;
		}
		case GIZMO_BUTTON::ROTATE:
		{
			Math::Vec2 entityToMouse = mPos - mAttached.GetComponent<Transform>().translation;
			if (entityToMouse.y != 0.f && entityToMouse.x >= 0.f)
				mAttached.GetComponent<Transform>().rotation = atan2f(entityToMouse.y, entityToMouse.x) - (float)Math::PI / 2.f;
			else if (entityToMouse.y == 0.f && entityToMouse.x > 0.f)
				mAttached.GetComponent<Transform>().rotation = (float)Math::PI / 2.f - (float)Math::PI / 2.f;
			else if (entityToMouse.y != 0.f && entityToMouse.x < 0.f)
				mAttached.GetComponent<Transform>().rotation = (float)Math::PI * 2.f + atan2f(entityToMouse.y, entityToMouse.x) - (float)Math::PI / 2.f;
			else
				mAttached.GetComponent<Transform>().rotation = 3.f * (float)Math::PI / 2.f - (float)Math::PI / 2.f;
			break;
		}
		case GIZMO_BUTTON::TRANSLATE:
			mAttached.GetComponent<Transform>().translation = mPos;
			break;
		default:
			break;
		}
	}
}