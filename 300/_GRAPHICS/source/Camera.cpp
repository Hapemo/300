/**-------------------------------------------------------------------------------------/
 * @file
 *  Camera.cpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class implementation of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#include "Camera.hpp"

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
 *  Gets the position of the camera
 * @return
 *  Position of the camera
 *---------------------------------------------------------------------------*/
vec3 GFX::Camera::position()
{
	return mPosition;
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
void GFX::Camera::SetProjection(float fovAngle, ivec2 size, float nearZ, float farZ)
{
	float aspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
	mProjection = glm::perspective(glm::radians(fovAngle), aspectRatio, nearZ, farZ);
	//mProjection = glm::ortho(left, right, bottom, top, near, far);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Updates the camera's view projection matrix based on its current position,
 *	current target and projection matrix
 * @return
 *  none
 *---------------------------------------------------------------------------*/
void GFX::Camera::Update()
{
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
