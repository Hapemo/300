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

void ContactCallback::onContact(const PxContactPairHeader&, const PxContactPair* pairs, PxU32 count)
{
}

void ContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	while (count--)
	{
		PxTriggerPair& current = *pairs++;
		Entity triggerEntity = *static_cast<uint32_t*>(current.triggerActor->userData);
		uint32_t otherEntity = *static_cast<uint32_t*>(current.otherActor->userData);

		if (static_cast<uint32_t>(triggerEntity.id) == otherEntity)
			continue;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (triggerEntity.HasComponent<BoxCollider>() && triggerEntity.GetComponent<BoxCollider>().mIsTrigger)
			{
				BoxCollider& trigger = triggerEntity.GetComponent<BoxCollider>();
				trigger.mIsTriggerCollide = true;
				trigger.mTriggerCollidingWith = otherEntity;
			}
			else if (triggerEntity.HasComponent<SphereCollider>() && triggerEntity.GetComponent<SphereCollider>().mIsTrigger)
			{
				SphereCollider& trigger = triggerEntity.GetComponent<SphereCollider>();
				trigger.mIsTriggerCollide = true;
				trigger.mTriggerCollidingWith = otherEntity;
			}
			else if (triggerEntity.HasComponent<CapsuleCollider>() && triggerEntity.GetComponent<CapsuleCollider>().mIsTrigger)
			{
				CapsuleCollider& trigger = triggerEntity.GetComponent<CapsuleCollider>();
				trigger.mIsTriggerCollide = true;
				trigger.mTriggerCollidingWith = otherEntity;
			}
			else
				PWARNING("OnTrigger callback called for entities without OnTrigger events!")
		}
		else if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (triggerEntity.HasComponent<BoxCollider>() && triggerEntity.GetComponent<BoxCollider>().mIsTrigger)
			{
				BoxCollider& trigger = triggerEntity.GetComponent<BoxCollider>();
				trigger.mIsTriggerCollide = false;
				trigger.mTriggerCollidingWith = 0;
			}
			else if (triggerEntity.HasComponent<SphereCollider>() && triggerEntity.GetComponent<SphereCollider>().mIsTrigger)
			{
				SphereCollider& trigger = triggerEntity.GetComponent<SphereCollider>();
				trigger.mIsTriggerCollide = false;
				trigger.mTriggerCollidingWith = 0;
			}
			else if (triggerEntity.HasComponent<CapsuleCollider>() && triggerEntity.GetComponent<CapsuleCollider>().mIsTrigger)
			{
				CapsuleCollider& trigger = triggerEntity.GetComponent<CapsuleCollider>();
				trigger.mIsTriggerCollide = false;
				trigger.mTriggerCollidingWith = 0;
			}
			else
				PWARNING("OnTrigger callback called for entities without OnTrigger events!")
		}
	}
}

void ContactCallback::onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32)
{
}

void ContactCallback::onSleep(PxActor**, PxU32)
{
}
