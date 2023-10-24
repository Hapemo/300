/*!*****************************************************************************
\file PhysX.h
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Pepe Production
\date 15/08/2023
\brief
Functions for setting up, initialize and cleaning up PhysX.
*******************************************************************************/
#pragma once
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"

using namespace physx;

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
	/*!*****************************************************************************
	Initialise PhysX
	*******************************************************************************/
	PhysX();
	/*!*****************************************************************************
	Clean up
	*******************************************************************************/
	~PhysX();
};