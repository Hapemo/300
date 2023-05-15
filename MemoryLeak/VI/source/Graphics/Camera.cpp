/*!*****************************************************************************
\file Camera.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 17-10-2022
\brief
This file contains a basic camera system for moving and zooming in/out.
*******************************************************************************/
#include <Camera.h>

/*!*****************************************************************************
\brief
Default constructor for Camera class.
*******************************************************************************/
Camera::Camera() : mPos(0.f, 0.f), mZoom(1.f), 
	mWindowWidth(0), mWindowHeight(0), mInitialPos(0, 0), mInitialZoom(1.f) {}

/*!*****************************************************************************
\brief
Initializes the camera class with width and height.

\param int _windowWidth
width of the camera.

\param int _windowHeight
height of the camera.
*******************************************************************************/
void Camera::Init(int _windowWidth, int _windowHeight)
{
	mWindowWidth = _windowWidth;
	mWindowHeight = _windowHeight;
}
/*!*****************************************************************************
\brief
Operator overload for +=.

\param const Math::Vec2& _translate
The amount to translate the camera by.

\return
A reference to the camera.
*******************************************************************************/
Camera& Camera::operator+=(const Math::Vec2& translate)
{
	mPos += translate;
	return *this;
}
/*!*****************************************************************************
\brief
Operator overload for -=.

\param const Math::Vec2& _translate
The amount to translate the camera by.

\return
A reference to the camera.
*******************************************************************************/
Camera& Camera::operator-=(const Math::Vec2& translate)
{
	mPos -= translate;
	return *this;
}
/*!*****************************************************************************
\brief
Operator overload for *=.

\param const Math::Vec2& _zoom
The amount to zoom the camera. +ve amounts to zoom in, -ve to zoom out.

\return
A reference to the camera.
*******************************************************************************/
Camera& Camera::operator*=(float zoom)
{
	mZoom += zoom;
	if (mZoom < 0.1f)
		mZoom = 0.1f;
	return *this;
}
/*!*****************************************************************************
\brief
Gets the world to camera (view) transform for ImGuizmo.

\return
A vector containing the matrix.
*******************************************************************************/
std::vector<float> Camera::GetImGuizmoWorldToCam()
{
	return std::vector<float>
	{
		1.f / mZoom, 0.f, 0.f, 0.f, 
		0.f, 1.f / mZoom, 0.f, 0.f, 
		0.f, 0.f, 1.f, 0.f, 
		-Math::Dot({ 1.f / mZoom, 0 }, mPos), -Math::Dot({ 0, 1.f / mZoom }, mPos), 0.f, 1.f
	};
}
/*!*****************************************************************************
\brief
Gets the camera to world transform for ImGuizmo.

\return
A vector containing the matrix.
*******************************************************************************/
std::vector<float> Camera::GetImGuizmoCamToWorld()
{
	return std::vector<float>
	{
		mZoom, 0.f, 0.f, 0.f,
		0.f, mZoom, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		mPos.x, mPos.y, 0.f, 1.f
	};
}

/*!*****************************************************************************
\brief
Resets the camera to its default position and zoom.
*******************************************************************************/
void Camera::Reset()
{
	mPos = mInitialPos;
	mZoom = mInitialZoom;
}

//------------------------------------------------------------------------------
// Getter and Setters
//------------------------------------------------------------------------------
void Camera::SetPos(const Math::Vec2& _mPos)
{
	mPos = _mPos;
}

void Camera::SetZoom(float _mZoom)
{
	mZoom = _mZoom;
	if (mZoom < 0.1f)
		mZoom = 0.1f;
}
float Camera::GetCameraWidth()
{
	return mWindowWidth * GetZoom();
}
void Camera::SetCameraWidth(int _width)
{
	if (!mWindowWidth)
		return;
	SetZoom(_width / (float)mWindowWidth);
}
float Camera::GetCameraHeight()
{
	return mWindowHeight * GetZoom();
}
void Camera::SetCameraHeight(int _height)
{
	if (!mWindowHeight)
		return;
	SetZoom(_height / (float)mWindowHeight);
}