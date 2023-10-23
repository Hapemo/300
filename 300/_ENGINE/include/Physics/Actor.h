#pragma once
#include "PhysX.h"
#include "ECS/ECS.h"
using namespace physx;

struct Actor
{
	uint32_t mEntity = 0;
	PxRigidActor* mActor = nullptr;
};