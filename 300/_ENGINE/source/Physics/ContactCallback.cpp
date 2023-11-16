#include "Physics/ContactCallback.h"
#include "Physics/PhysicsSystem.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Debug/AssertException.h"

void ContactCallback::onConstraintBreak(PxConstraintInfo*, PxU32)
{
}

void ContactCallback::onWake(PxActor**, PxU32)
{
}

void ContactCallback::onContact(const PxContactPairHeader&, const PxContactPair* pairs, PxU32 count)
{
	while (count--)
	{
		const PxContactPair& current = *pairs++;
		Entity playerEntity = *static_cast<uint32_t*>(current.shapes[0]->getActor()->userData);
		Entity otherEntity = *static_cast<uint32_t*>(current.shapes[1]->getActor()->userData);
		
		if (playerEntity.id == otherEntity.id)
			continue;

		if (!playerEntity.HasComponent<General>())
			continue;

		if (!otherEntity.HasComponent<General>())
			continue;

		if (playerEntity.GetComponent<General>().isDelete)
			continue;

		if (otherEntity.GetComponent<General>().isDelete)
			continue;

		if (current.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND))
		{
			if (playerEntity.HasComponent<Scripts>())
				playerEntity.GetComponent<Scripts>().RunFunctionForAllScripts("OnContactEnter", otherEntity);
			if (otherEntity.HasComponent<Scripts>())
				otherEntity.GetComponent<Scripts>().RunFunctionForAllScripts("OnContactEnter", playerEntity);

		}
		else if (current.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (playerEntity.HasComponent<Scripts>())
				playerEntity.GetComponent<Scripts>().RunFunctionForAllScripts("OnContactExit", otherEntity);
			if (otherEntity.HasComponent<Scripts>())
				otherEntity.GetComponent<Scripts>().RunFunctionForAllScripts("OnContactExit", playerEntity);
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

		if (!triggerEntity.HasComponent<General>())
			continue;

		if (!otherEntity.HasComponent<General>())
			continue;

		if (!triggerEntity.HasComponent<Scripts>())
			continue;

		if (triggerEntity.GetComponent<General>().isDelete)
			continue;

		if (otherEntity.GetComponent<General>().isDelete)
			continue;

		if (otherEntity.GetComponent<General>().name == "Camera")
			continue;

		/*std::vector<uint32_t>& triggeredEntities = PhysicsSystem::mTriggerCollisions[static_cast<uint32_t>(triggerEntity.id)];
		uint32_t otherID = static_cast<uint32_t>(otherEntity.id);*/

		if (current.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{

			triggerEntity.GetComponent<Scripts>().RunFunctionForAllScripts("OnTriggerEnter", otherEntity);
			//triggeredEntities.push_back(otherID);
		}
		else if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//auto itr = std::find(triggeredEntities.begin(), triggeredEntities.end(), otherID);
			//if (itr != triggeredEntities.end())
			//{
				triggerEntity.GetComponent<Scripts>().RunFunctionForAllScripts("OnTriggerExit", otherEntity);
			/*	triggeredEntities.erase(itr);
			}*/
		}
	}
}

void ContactCallback::onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32)
{
}

void ContactCallback::onSleep(PxActor**, PxU32)
{
}
