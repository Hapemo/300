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


Camera_Input::Camera_Input()
	: mCameraSpeed{ 100.f }, mMouseSensitivity{ 0.1f }
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
			moveVector += glm::normalize(cam.direction()) * dt * mCameraSpeed;
		}

		// camera move backwards
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::S)) {
			moveVector -= glm::normalize(cam.direction()) * dt * mCameraSpeed;
		}

		// camera move right
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::D)) {
			moveVector += side * dt * mCameraSpeed;
		}

		// camera move left
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::A)) {
			moveVector -= side * dt * mCameraSpeed;
		}

		// camera move up
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::SPACE)) {
			moveVector += up * dt * mCameraSpeed;
		}

		// camera move down
		if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_SHIFT)) {
			moveVector -= up * dt * mCameraSpeed;
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
				delta *= mMouseSensitivity;		// adjust the mouse movement sensitivity

				cam.mYaw += delta.x;
				cam.mPitch += delta.y;
			}
		}
		
		// update previous cursor pos
		cam.SetCursorPosition(Input::CursorPos());

		if (cam.mPitch > 89.0f) {
			cam.mPitch = 89.0f;
		}

		if (cam.mPitch < -89.0f) {
			cam.mPitch = -89.0f;
		}

		glm::vec3 direction;
		direction.x = cos(glm::radians(cam.mYaw)) * cos(glm::radians(cam.mPitch));
		direction.y = sin(glm::radians(cam.mPitch));
		direction.z = sin(glm::radians(cam.mYaw)) * cos(glm::radians(cam.mPitch));
		direction.y *= -1;

		// update the camera's target based on the normalized direction
		cam.SetTarget(cam.position() + glm::normalize(direction));
	}

	//!< Camera Zoom
	cam.mFovDegree -= static_cast<float>(Input::GetScroll());
	if (cam.mFovDegree < 1.0f) {
		cam.mFovDegree = 1.0f;
	}

	if (cam.mFovDegree > 45.0f) {
		cam.mFovDegree = 45.0f;
	}


	//std::cout << cam.direction().x << ", " << cam.direction().y << ", " << cam.direction().z << "\n";
	//std::cout << cam.position().x << ", " << cam.position().y << ", " << cam.position().z << "\n";
	//std::cout << moveVector.x << ", " << moveVector.y << ", " << moveVector.z << "\n";
}