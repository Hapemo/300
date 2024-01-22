#include <ECS/ECS_Components.h>

void Transform::parentChildRotateInit(char axis, float angle)
{
	
}


void Transform::parentChildRotateUpdate(float dt)
{
	// if this has ended, 
	if (mCumulativeTime <= 0) {
		mCumulativeTime = 0;
		return;
	}

	
}