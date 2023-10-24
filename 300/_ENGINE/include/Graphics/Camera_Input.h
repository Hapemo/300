#pragma once
#ifndef _CAMERA_INPUT_H
#define _CAMERA_INPUT_H

#include <Camera.hpp>

class Camera_Input
{
public:
	static Camera_Input& getInstance()
	{
		static Camera_Input instance;
		return instance;
	}

	// Member variable
	//float	mCameraSpeed;
	//float	mMouseSensitivity;

	// Member function
	Camera_Input();
	void updateCameraInput(GFX::Camera& input, const float& dt);
};

#endif // !_CAMERA_INPUT_H
