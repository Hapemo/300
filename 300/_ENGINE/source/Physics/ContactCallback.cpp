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
	while (count--)
	{
		const PxContactPair& current = *pairs++;
		Entity playerEntity = *static_cast<uint32_t*>(current.shapes[0]->getActor()->userData);
		Entity otherEntity = *static_cast<uint32_t*>(current.shapes[1]->getActor()->userData);
		
		if (playerEntity.id == otherEntity.id)
			continue;

		if (playerEntity.GetComponent<General>().GetTag() != "PLAYER")
		{
			if (otherEntity.GetComponent<General>().GetTag() != "PLAYER")
				continue;
			Entity temp = playerEntity;
			playerEntity = otherEntity;
			otherEntity = temp;
		}

		if (current.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND))
		{
			// call scripting
		}
		else if (current.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			// call scripting
		}
	}
}

void ContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	while (count--)
	{
		PxTriggerPair& current = *pairs++;
		Entity triggerEntity = *static_cast<uint32_t*>(current.triggerActor->userData);
		Entity otherEntity = *static_cast<uint32_t*>(current.otherActor->userData);

		if (triggerEntity.id == otherEntity.id)
			continue;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			triggerEntity.GetComponent<Scripts>().RunFunctionForAllScripts("OnTriggerEnter", otherEntity);
		}
		else if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			triggerEntity.GetComponent<Scripts>().RunFunctionForAllScripts("OnTriggerExit", otherEntity);
		}
	}
}

void ContactCallback::onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32)
{
}

void ContactCallback::onSleep(PxActor**, PxU32)
{
}
