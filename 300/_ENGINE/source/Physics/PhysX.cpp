#include "Physics/PhysX.h"

PhysX::PhysX()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocator, mDefaultErrorCallBack);

	if (!mFoundation) throw ("PxCreateFoundation Failed!");

	mPvd = PxCreatePvd(*mFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	mToleranceScale.length = 100;
	mToleranceScale.speed = 981;
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);

	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f);
	mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	mScene = mPhysics->createScene(sceneDesc);

	physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
	
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

	}
}

PhysX::~PhysX()
{
	PX_RELEASE(mScene);
	PX_RELEASE(mDispatcher);
	mPhysics->release();
	if (mPvd)
	{
		physx::PxPvdTransport* transport = mPvd->getTransport();
		mPvd->release();	
		mPvd = NULL;
		PX_RELEASE(transport);
	}
	mFoundation->release();
}
