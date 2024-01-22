#include <ECS/ECS_Components.h>
#include <iostream>

// [1. Axis]  - Indicates the axis around which the rotation will occur
// [2. Angle] - Specifies the angle of rotationin degrees 

// This function is responsible for initializing the rotation parameters based on the input axis and angle
// - Setting up the initial rotation of the gun (child)
void Transform::parentChildRotateInit(char axis, float angle)
{
	std::cout << "Parent Child Init" << std::endl;
	std::cout << "Axis Selected: " << axis << std::endl;
	std::cout << "Rotating by : " << angle << std::endl;

	mRotationAxis = axis; 
	mRotationDegrees = angle; 
}


void Transform::parentChildRotateUpdate(float dt)
{
	std::cout << "Parent Child Update" << std::endl;

	// if this has ended, 
	if (mCumulativeTime <= 0) {
		mCumulativeTime = 0;
		return;
	}
	
	else
	{

	}

	
}