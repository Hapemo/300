#pragma once
#include "PhysX.h"
#include "pch.h"

using namespace physx;

class ContactCallback : public PxSimulationEventCallback
{
public:
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
private:
};