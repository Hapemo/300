#pragma once
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"

struct PhysX
{
	physx::PxDefaultAllocator mDefaultAllocator;
	physx::PxDefaultErrorCallback mDefaultErrorCallBack;
	physx::PxTolerancesScale mToleranceScale;
	physx::PxDefaultCpuDispatcher* mDispatcher;
	physx::PxFoundation* mFoundation;
	physx::PxPhysics* mPhysics;
	physx::PxScene* mScene;
	physx::PxMaterial* mMaterial;
	physx::PxPvd* mPvd;
	PhysX();
	~PhysX();
};