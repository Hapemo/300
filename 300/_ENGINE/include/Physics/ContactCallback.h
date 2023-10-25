#pragma once
#include "PhysX.h"
#include "pch.h"

using namespace physx;

class ContactCallback : public PxSimulationEventCallback
{
public:
	void onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/) override;
	void onWake(PxActor** /*actors*/, PxU32 /*count*/) override;
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count) override;
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override;
	void onSleep(PxActor** /*actors*/, PxU32 /*count*/) override;
private:
};

static ContactCallback gContactCallback;