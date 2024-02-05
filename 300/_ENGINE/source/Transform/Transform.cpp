#include <ECS/ECS_Components.h>
#include <iostream>

// [1. Axis]  - Indicates the axis around which the rotation will occur
// [2. Angle] - Specifies the angle of rotationin degrees 

// This function is responsible for initializing the rotation parameters based on the input axis and angle
// - Setting up the initial rotation of the gun (child)
void Transform::parentChildRotateInit(char axis, float angle)
{
	/*std::cout << "Parent Child Init" << std::endl;
	std::cout << "Axis Selected: " << axis << std::endl;
	std::cout << "Rotating by : " << angle << std::endl;*/

	mRotationAxis = axis;
	mRotationDegrees = angle;
	mCumulativeTime = 0.0f;
	mInitialRotation = mRotate;

	//std::cout << "(" << mInitialRotation.x << " , " << mInitialRotation.y << " , " << mInitialRotation.z << std::endl;

	// Selecting the axis that we want to rotate on
	switch (axis)
	{
	case 'x':
	case 'X':
		mRotationAxis = 'x';
		mRotateAxisVector = glm::vec3(angle, 0.0f, 0.0f);
		break;
	case 'y':
	case 'Y':
		mRotationAxis = 'y';
		mRotateAxisVector = glm::vec3(0.0f, angle, 0.0f);
		break;
	case 'z':
	case 'Z':
		mRotationAxis = 'z';
		mRotateAxisVector = glm::vec3(0.0f, 0.0f, angle);
		break;
	default:
		//std::cout << "INVALID AXIS." << std::endl;
		break;
	}
}


void Transform::parentChildRotateUpdate(float dt)
{
	//std::cout << "Parent Child Update" << std::endl;

	// if this has ended, if skill has ceased.
	if (mCumulativeTime <= 0 || mSkillTimer <= 0) { 
		mCumulativeTime = 0;
		mRotate = mInitialRotation;
		return;
	}

	else
	{
		// Calculate the rotation amount for the current frame
		//std::cout << "Rotation Amount: " << (mRotationDegrees / mDuration) * dt << std::endl;
		float rotation_amount = (mRotationDegrees / mDuration) * dt * mRotateSpeed;
		//std::cout << "Rotation Amount (W speed): " << rotation_amount << std::endl;
		//std::cout << "Rotate Amount: " << rotation_amount << std::endl;

		// Create a quaternion from the additional rotation
		glm::quat additional_rotation = glm::angleAxis(glm::radians(rotation_amount), glm::normalize(mRotateAxisVector));

		//std::cout << "Previous Rottion (GLM::VEC3): " << mRotate.x << "," << mRotate.y << "," << mRotate.z << std::endl;

		// Convert the existing [mRotate - glm::vec3] to a quarternion
		glm::quat current_rotation = glm::quat(glm::radians(mRotate));

		//std::cout << "Quat Rotation: " << current_rotation.x << "," << current_rotation.y << "," << current_rotation.z << "," << current_rotation.w << std::endl;

		// Combine the existing rotation with the additional rotation (matrix * matrix)
		glm::quat combined_rotation = additional_rotation * current_rotation;

		// Convert the combined rotation back to <Euler Angles> and Update the mRotate.
		mRotate = glm::degrees(glm::eulerAngles(combined_rotation));

		// Decreament delta time.
		mCumulativeTime -= dt;
		mSkillTimer -= dt;
		//std::cout << "Cumultive Timer: " << mCumulativeTime << std::endl;
	}

}

// This function is called everytime the [FIRE] button is pressed. 
void Transform::gunAnimationUpdate(std::string gun_type, float skill_timer, float recoil_angle, float recoil_speed, float recoil_duration)
{
	//std::cout << "HANDLING GUN ANIMATION" << std::endl;
	
	//mCumulativeTime = 10.0f;

	if (gun_type == "REVOLVER")
	{	
	
		// Ensure the recoil angle is within the specified bounds
		//recoil_angle = glm::clamp(recoil_angle, -max_recoil_angle, max_recoil_angle);
		//std::cout << "Recoil Angle: " << recoil_angle << std::endl;

		// Set up the parent-child rotation for the recoil
		mRotationDegrees = recoil_angle;
		mDuration = recoil_duration;				    // Set the total elapsed time for the recoil animation to run. (doesnt change)
		mCumulativeTime = recoil_duration;			    // This will decrease as time elapses
		mSkillTimer = skill_timer;					    // [2/4] To sync up with skill system (weapon swap)
		mRotateSpeed = recoil_speed;					// Set the speed of the recoil animation to go off. 

		//std::cout << "Cumulative Time udpated to: " << mCumulativeTime << std::endl;
		//std::cout << "Rotate Speed: " << mRotateSpeed << std::endl;

		//std::cout << "ROTATION Degrees: " << mRotationDegrees << std::endl;

	}


}

void Transform::setInitialRotation()
{
	mRotate = mInitialRotation;
}


//void Transform::parentChildRotateUpdate(float dt)
//{
//	std::cout << "Parent Child Update" << std::endl;
//
//	// if this has ended, 
//	if (mCumulativeTime <= 0) {
//		mCumulativeTime = 0;
//		return;
//	}
//
//	else
//	{
//		// Calculate the rotation amount for the current frame
//
//		float rotation_amount = (mRotationDegrees / mCumulativeTime) * dt;
//		;
//		std::cout << "Rotate Amount: " << rotation_amount << std::endl;
//
//		// Create a quaternion from the additional rotation
//		glm::quat additional_rotation = glm::angleAxis(glm::radians(rotation_amount), glm::normalize(mRotateAxisVector));
//
//		//std::cout << "Previous Rottion (GLM::VEC3): " << mRotate.x << "," << mRotate.y << "," << mRotate.z << std::endl;
//
//		// Convert the existing [mRotate - glm::vec3] to a quarternion
//		glm::quat current_rotation = glm::quat(glm::radians(mRotate));
//
//		//std::cout << "Quat Rotation: " << current_rotation.x << "," << current_rotation.y << "," << current_rotation.z << "," << current_rotation.w << std::endl;
//
//		// Combine the existing rotation with the additional rotation (matrix * matrix)
//		glm::quat combined_rotation = additional_rotation * current_rotation;
//
//		// Convert the combined rotation back to <Euler Angles> and Update the mRotate.
//		mRotate = glm::degrees(glm::eulerAngles(combined_rotation));
//
//		// Decreament delta time.
//		mCumulativeTime -= dt;
//	}
//
//}