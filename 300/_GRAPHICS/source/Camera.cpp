/*!*****************************************************************************
\file Camera.cpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Camera class implementation. Consists of the functions to get/set position,
target, direction, cursor position, view projection and projection of the camera
instance
*******************************************************************************/

#include "Camera.hpp"
#include <iostream>

GFX::Camera::Camera()
{
	mYaw = -90.f ;
	mPitch = 0.f;
	mAspectRatio = 0.f;
	mNear = 0.f;   
	mFar = 0.f;    
	mFovDegree = 0.f;
	mManualCameraSet = false;
}

 /**---------------------------------------------------------------------------/
  * @brief
  *  Set the position of the camera
  * @param newPosition
  *  New position of the camera
  * @return
  *  None
  *---------------------------------------------------------------------------*/
void GFX::Camera::SetPosition(vec3 newPosition)
{
	mPosition = newPosition;
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Set the target of the camera
 * @param newPosition
 *  New target of the camera
 * @return
 *  None
 *---------------------------------------------------------------------------*/
void GFX::Camera::SetTarget(vec3 newTarget)
{
	mTarget = newTarget;
}

void GFX::Camera::SetSize(ivec2 size)
{
	mSize = size;
	if(size.x == 0 && size.y == 0)
		return;

	mAspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Set the cursor position
 * @param newPosition
 *  New position of the cursor
 * @return
 *  None
 *---------------------------------------------------------------------------*/
void GFX::Camera::SetCursorPosition(vec2 newPosition)
{
	mCursorPos = newPosition;
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Gets the right vector of the camera. Unnormalized
 * @return
 *  Right vector of the camera
 *---------------------------------------------------------------------------*/
vec3 GFX::Camera::GetRightVector()
{
	vec3 forward = mTarget - mPosition;

	return glm::cross(forward, vec3(0, 1, 0));
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Gets the up vector of the camera. Unnormalized
 * @return
 *  Up vector of the camera
 *---------------------------------------------------------------------------*/
vec3 GFX::Camera::GetUpVector()
{
	vec3 forward = mTarget - mPosition;
	vec3 right = GetRightVector();

	return glm::cross(right, forward);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Gets the position of the camera
 * @return
 *  Position of the camera
 *---------------------------------------------------------------------------*/
vec3 GFX::Camera::position()
{
	return mPosition;
}

vec3 GFX::Camera::target()
{
	return mTarget;
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Gets the direction vector of the camera
 * @return
 *  Direction vector of the camera
 *---------------------------------------------------------------------------*/
vec3 GFX::Camera::direction()
{
	return mTarget - mPosition;
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Gets the positin of the cursor
 * @return
 *  Position of the cursor
 *---------------------------------------------------------------------------*/
vec2 GFX::Camera::cursorPosition()
{
	return mCursorPos;
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Sets the perspective projection matrix of the camera
 * @param fovAngle
 *	angle of the field of view in radians
 * @param size
 *	width and height of the camera's view
 * @param nearZ
 *	Z value of the near plane of camera
 * @param farZ
 *	Z value of the far plane of camera
 * @return
 *  none
 *---------------------------------------------------------------------------*/
//void GFX::Camera::SetProjection(float left, float right, float bottom, float top, float near, float far)
void GFX::Camera::SetProjection(float fovDegree, ivec2 size, float nearZ, float farZ)
{
	mFovDegree = fovDegree;
	mSize = size;
	mNear = nearZ;
	mFar = farZ;
	mAspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Updates the camera's view projection matrix based on its current position,
 *	current target and projection matrix
 * @return
 *  none
 *---------------------------------------------------------------------------*/
void GFX::Camera::Update(bool freeMoving)
{
	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	}

	if (mPitch < -89.0f) {
		mPitch = -89.0f;
	}

	if (mManualCameraSet) {
		mManualCameraSet = !mManualCameraSet;
	} else if (freeMoving) {
		glm::vec3 direction;
		direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		direction.y = sin(glm::radians(mPitch));
		direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		direction.y *= -1;

		// update the camera's target based on the normalized direction
		SetTarget(position() + glm::normalize(direction));
	}

	mProjection = glm::perspective(glm::radians(mFovDegree), mAspectRatio, mNear, mFar);
	mView = glm::lookAt(glm::vec3(mPosition), glm::vec3(mTarget), glm::vec3(0.0f, 1.0f, 0.0f));
	mViewProjection = mProjection * mView;
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Gets the view projection matrix of the camera
 * @return
 *  View projection matrix of the camera
 *---------------------------------------------------------------------------*/
glm::mat4 GFX::Camera::viewProj()
{
	return mViewProjection;
}


glm::mat4 GFX::Camera::Proj()
{
	return mProjection;
}

void GFX::Camera::RotateCameraView(vec2 cursorposition)
{
	vec2 delta = mCursorPos - cursorposition;
	if (delta == vec2(0.f, 0.f))
		return;

	delta *= mSensitivity;		// adjust the mouse movement sensitivity
	mYaw += delta.x;
	mPitch += delta.y;

	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	}

	if (mPitch < -89.0f) {
		mPitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	direction.y = sin(glm::radians(mPitch));
	direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	direction.y *= -1;

	// update the camera's target based on the normalized direction
	SetTarget(position() + glm::normalize(direction));
}
