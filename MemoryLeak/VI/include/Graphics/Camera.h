/*!*****************************************************************************
\file Camera.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 17-10-2022
\brief
This file contains a basic camera system for moving and zooming in/out.
*******************************************************************************/
#pragma once
#include <pch.h>

/*!*****************************************************************************
\brief
Class that encapsulates a Camera.
*******************************************************************************/
class Camera
{
public:
	/*!*****************************************************************************
	\brief
	Default constructor for Camera class.
	*******************************************************************************/
	Camera();
	/*!*****************************************************************************
	\brief
	Default destrcutor for FontRenderer class.
	*******************************************************************************/
	~Camera() = default;
	/*!*****************************************************************************
	\brief
	Initializes the camera class with width and height.

	\param int _windowWidth
	width of the camera.

	\param int _windowHeight
	height of the camera.
	*******************************************************************************/
	void Init(int _windowWidth, int _windowHeight);
	/*!*****************************************************************************
	\brief
	Operator overload for +=.

	\param const Math::Vec2& _translate
	The amount to translate the camera by.

	\return
	A reference to the camera.
	*******************************************************************************/
	Camera& operator+=(const Math::Vec2& _translate);
	/*!*****************************************************************************
	\brief
	Operator overload for -=.

	\param const Math::Vec2& _translate
	The amount to translate the camera by.

	\return
	A reference to the camera.
	*******************************************************************************/
	Camera& operator-=(const Math::Vec2& _translate);
	/*!*****************************************************************************
	\brief
	Operator overload for *=.

	\param const Math::Vec2& _zoom
	The amount to zoom the camera. +ve amounts to zoom in, -ve to zoom out.

	\return
	A reference to the camera.
	*******************************************************************************/
	Camera& operator*=(float _zoom);
	/*!*****************************************************************************
	\brief
	Resets the camera to its default position and zoom.
	*******************************************************************************/
	void Reset();

	/*!*****************************************************************************
	\brief
	Gets the world to camera (view) transform for ImGuizmo.

	\return
	A vector containing the matrix.
	*******************************************************************************/
	std::vector<float> GetImGuizmoWorldToCam();
	/*!*****************************************************************************
	\brief
	Gets the camera to world transform for ImGuizmo.

	\return
	A vector containing the matrix.
	*******************************************************************************/
	std::vector<float> GetImGuizmoCamToWorld();	

	//------------------------------------------------------------------------------
	// Getter and Setters
	//------------------------------------------------------------------------------
	Math::Vec2 GetWindowDim() { return Math::Vec2( static_cast<float>(mWindowWidth), static_cast<float>(mWindowHeight)); }
	Math::Vec2 GetPos() { return mPos; }
	float GetZoom() { return mZoom; }
	void SetPos(const Math::Vec2& _mPos);
	void SetZoom(float _mZoom);
	Math::Vec2 GetInitialPos() { return mInitialPos; }
	void SetInitialPos(const Math::Vec2& _initialPos) { mInitialPos = _initialPos; }
	float GetInitialZoom() { return mInitialZoom; }
	void SetInitialZoom(float _initialZoom) { mInitialZoom = _initialZoom; }
	float GetCameraWidth();
	void SetCameraWidth(int _width);
	float GetCameraHeight();
	void SetCameraHeight(int _height);

private:
	int mWindowWidth, mWindowHeight;
	Math::Vec2 mPos;
	float mZoom;
	Math::Vec2 mInitialPos;
	float mInitialZoom;
};