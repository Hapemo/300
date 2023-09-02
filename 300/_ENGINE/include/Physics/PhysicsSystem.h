#pragma once
#include <PxPhysicsAPI.h>
#include <PxPhysics.h>

struct PhysX
{
	PhysX();
	physx::PxDefaultAllocator mDefaultAllocator;
	physx::PxDefaultErrorCallback mErrorCallback;
	physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
	physx::PxTolerancesScale mToleranceScale;
	physx::PxFoundation* mFoundation = NULL;
	physx::PxPhysics* mPhysics = NULL;
	physx::PxScene* mScene = NULL;
	physx::PxMaterial* mMaterial = NULL;
	physx::PxPvd* mPvd = NULL;
};

class PhysicsSystem
{
	PhysX px;
};