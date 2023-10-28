#pragma once
#ifndef _CAMERA_INPUT_H
#define _CAMERA_INPUT_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>
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

class Camera_Scripting
{
	// Setters
	static void SetPosition(Entity cameraEntity, const vec3& newposition);
	static void SetTarget(Entity cameraEntity, const vec3& newtarget);
	static void SetCameraSpeed(Entity cameraEntity, const float& speed);
	static void SetSensitivity(Entity cameraEntity, const float& sensitivity);

	// Getters
	static glm::vec3 GetPosition(Entity cameraEntity);
	static glm::vec3 GetTarget(Entity cameraEntity);
	static glm::vec3 GetDirection(Entity cameraEntity);
	static float GetCameraSpeed(Entity cameraEntity);
	static float GetSensitivity(Entity cameraEntity);

	// Update
	static void RotateCameraView(Entity cameraEntity, const vec2& cursorposition);

};

#endif // !_CAMERA_INPUT_H
