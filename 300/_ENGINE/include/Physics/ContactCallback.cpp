#include "ContactCallback.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"

void ContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	while (count--)
	{
		PxTriggerPair& current = *pairs++;
		Entity triggerEntity = *static_cast<uint32_t*>(current.triggerActor->userData);
		General otherEntityGeneral = Entity(*static_cast<uint32_t*>(current.otherActor->userData)).GetComponent<General>();
		std::string tag = otherEntityGeneral.tag[otherEntityGeneral.tagid];

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (triggerEntity.HasComponent<BoxCollider>() && triggerEntity.GetComponent<BoxCollider>().mIsTrigger)
			{
				BoxCollider& trigger = triggerEntity.GetComponent<BoxCollider>();
				trigger.mIsTriggerCollide = true;
				trigger.mTriggerCollidingWith = tag;
			}
			else if (triggerEntity.HasComponent<SphereCollider>() && triggerEntity.GetComponent<SphereCollider>().mIsTrigger)
			{
				SphereCollider& trigger = triggerEntity.GetComponent<SphereCollider>();
				trigger.mIsTriggerCollide = true;
				trigger.mTriggerCollidingWith = tag;
			}
			else if (triggerEntity.HasComponent<CapsuleCollider>() && triggerEntity.GetComponent<CapsuleCollider>().mIsTrigger)
			{
				CapsuleCollider& trigger = triggerEntity.GetComponent<CapsuleCollider>();
				trigger.mIsTriggerCollide = true;
				trigger.mTriggerCollidingWith = tag;
			}
		}
		else if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (triggerEntity.HasComponent<BoxCollider>() && triggerEntity.GetComponent<BoxCollider>().mIsTrigger)
			{
				BoxCollider& trigger = triggerEntity.GetComponent<BoxCollider>();
				trigger.mIsTriggerCollide = false;
				trigger.mTriggerCollidingWith = "";
			}
			else if (triggerEntity.HasComponent<SphereCollider>() && triggerEntity.GetComponent<SphereCollider>().mIsTrigger)
			{
				SphereCollider& trigger = triggerEntity.GetComponent<SphereCollider>();
				trigger.mIsTriggerCollide = false;
				trigger.mTriggerCollidingWith = "";
			}
			else if (triggerEntity.HasComponent<CapsuleCollider>() && triggerEntity.GetComponent<CapsuleCollider>().mIsTrigger)
			{
				CapsuleCollider& trigger = triggerEntity.GetComponent<CapsuleCollider>();
				trigger.mIsTriggerCollide = false;
				trigger.mTriggerCollidingWith = "";
			}
		}
	}
}
