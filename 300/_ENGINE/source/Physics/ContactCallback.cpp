#include "Physics/ContactCallback.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Debug/AssertException.h"

void ContactCallback::onConstraintBreak(PxConstraintInfo*, PxU32)
{
}

void ContactCallback::onWake(PxActor**, PxU32)
{
}

void ContactCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count)
{
	int countt = count;
	countt -= 0;
}

void ContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	while (count--)
	{
		PxTriggerPair& current = *pairs++;
		uint32_t triggerEntity = *static_cast<uint32_t*>(current.triggerActor->userData);
		uint32_t otherEntity = *static_cast<uint32_t*>(current.otherActor->userData);

		if (triggerEntity == otherEntity)
			continue;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//lua OnTriggerEnter(thisEntity, otherEntity)
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			//lua OnTrigger(thisEntity, otherEntity)
		}
		else if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//lua OnTriggerExit(thisEntity, otherEntity)
		}
	}
}

void ContactCallback::onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32)
{
}

void ContactCallback::onSleep(PxActor**, PxU32)
{
}
