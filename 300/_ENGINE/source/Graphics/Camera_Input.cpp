/*!*************************************************************************
****
\file			Camera_Input.cpp
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The function definitions for the Camera_Input class
	This class controls the keyboard and mouse input related to the camera's
	movement

****************************************************************************
***/

#include <Graphics/Camera_Input.h>
#include "Input/Input.h"
#include <FPSManager.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>
#include <Graphics/GraphicsSystem.h>

Camera_Input::Camera_Input()
{}


void Camera_Input::updateCameraInput(GFX::Camera& cam, const float& dt)
{
	glm::vec3 moveVector{};
	auto  side = glm::normalize(glm::cross(cam.direction(), { 0, 1, 0 }));
	auto  up = glm::normalize(glm::cross(side, cam.direction()));

	//!< WASD Camera Movement
	{
		// camera move forward
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::W)) {
			moveVector += glm::normalize(cam.direction()) * dt * cam.mCameraSpeed;
		}

		// camera move backwards
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::S)) {
			moveVector -= glm::normalize(cam.direction()) * dt * cam.mCameraSpeed;
		}

		// camera move right
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::D)) {
			moveVector += side * dt * cam.mCameraSpeed;
		}

		// camera move left
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::A)) {
			moveVector -= side * dt * cam.mCameraSpeed;
		}

		// camera move up
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::SPACE)) {
			moveVector += up * dt * cam.mCameraSpeed;
		}

		// camera move down
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_SHIFT)) {
			moveVector -= up * dt * cam.mCameraSpeed;
		}

		cam.SetPosition(cam.position() + moveVector);
		cam.SetTarget(cam.target() + moveVector);
	}

	//!< Mouse Movement
	{
		// if the right mouse button is being pressed
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::M_BUTTON_R))
		{
			// if the cursor is moving relative to its old position
			vec2 delta = Input::CursorPos() - cam.cursorPosition();
			if (delta != vec2(0.f, 0.f))
			{
				delta *= cam.mSensitivity;		// adjust the mouse movement sensitivity

				cam.mYaw += delta.x;
				cam.mPitch += delta.y;
			}
		}
		
		// update previous cursor pos
		cam.SetCursorPosition(Input::CursorPos());
	}

	//!< Camera Zoom
	if (systemManager->mGraphicsSystem->m_EnableScroll)
	{
		cam.mFovDegree -= static_cast<float>(Input::GetScroll());
		if (cam.mFovDegree < GFX::CameraConstants::minFOV) {
			cam.mFovDegree = GFX::CameraConstants::minFOV;
		}

		if (cam.mFovDegree > GFX::CameraConstants::maxFOV) {
			cam.mFovDegree = GFX::CameraConstants::maxFOV;
		}
	}
}


void Camera_Scripting::SetPosition(Entity cameraEntity, const vec3& newposition)
{
	assert(cameraEntity.HasComponent<Transform>());
	cameraEntity.GetComponent<Transform>().mTranslate = newposition;
}


void Camera_Scripting::SetTarget(Entity cameraEntity, const vec3& newtarget)
{
	assert(cameraEntity.HasComponent<Camera>());
	cameraEntity.GetComponent<Camera>().mCamera.mTarget = newtarget;
}


void Camera_Scripting::SetCameraSpeed(Entity cameraEntity, const float& speed)
{
	assert(cameraEntity.HasComponent<Camera>());
	cameraEntity.GetComponent<Camera>().mCamera.mCameraSpeed = speed;
}


void Camera_Scripting::SetSensitivity(Entity cameraEntity, const float& sensitivity)
{
	assert(cameraEntity.HasComponent<Camera>());
	cameraEntity.GetComponent<Camera>().mCamera.mSensitivity = sensitivity;
}


glm::vec3 Camera_Scripting::GetPosition(Entity cameraEntity)
{
	assert(cameraEntity.HasComponent<Transform>());
	return cameraEntity.GetComponent<Transform>().mTranslate;
}


glm::vec3 Camera_Scripting::GetTarget(Entity cameraEntity)
{
	assert(cameraEntity.HasComponent<Camera>());
	return cameraEntity.GetComponent<Camera>().mCamera.mTarget;
}


glm::vec3 Camera_Scripting::GetDirection(Entity cameraEntity)
{
	assert(cameraEntity.HasComponent<Camera>());
	return cameraEntity.GetComponent<Camera>().mCamera.direction();
}


float Camera_Scripting::GetCameraSpeed(Entity cameraEntity)
{
	assert(cameraEntity.HasComponent<Camera>());
	return cameraEntity.GetComponent<Camera>().mCamera.mCameraSpeed;
}


float Camera_Scripting::GetSensitivity(Entity cameraEntity)
{
	assert(cameraEntity.HasComponent<Camera>());
	return cameraEntity.GetComponent<Camera>().mCamera.mSensitivity;
}


void Camera_Scripting::RotateCameraView(Entity cameraEntity, const vec2& cursorposition)
{
	assert(cameraEntity.HasComponent<Camera>());
	auto& caminst = cameraEntity.GetComponent<Camera>().mCamera;


	vec2 delta = caminst.mCursorPos - cursorposition;
	if (delta == vec2(0.f, 0.f))
		return;

	delta *= caminst.mSensitivity;		// adjust the mouse movement sensitivity
	caminst.mYaw += delta.x;
	caminst.mPitch += delta.y;

	if (caminst.mPitch > 89.0f) {
		caminst.mPitch = 89.0f;
	}

	if (caminst.mPitch < -89.0f) {
		caminst.mPitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(caminst.mYaw)) * cos(glm::radians(caminst.mPitch));
	direction.y = sin(glm::radians(caminst.mPitch));
	direction.z = sin(glm::radians(caminst.mYaw)) * cos(glm::radians(caminst.mPitch));
	direction.y *= -1;

	// update the camera's target based on the normalized direction
	caminst.SetTarget(caminst.mPosition + glm::normalize(direction));
}
