#include "Physics/PhysicsSystem.h"
#include "ECS/ECS.h"
#include "Debug/EnginePerformance.h"
#include "Physics/Accumulator.h"
#include "Physics/PhysXUtils.h"

std::unordered_map<std::uint32_t, std::vector<uint32_t>> PhysicsSystem::mTriggerCollisions;

PhysicsSystem::PhysicsSystem()
{
	mMaterials[MATERIAL::RUBBER] = CreateMaterial(0.9f, 0.8f, 0.f);
	mMaterials[MATERIAL::WOOD] = CreateMaterial(0.5f, 0.4f, 0.f);
	mMaterials[MATERIAL::METAL] = CreateMaterial(0.7f, 0.6f, 0.0f);
	mMaterials[MATERIAL::ICE] = CreateMaterial(0.1f, 0.05f, 0.f);
	mMaterials[MATERIAL::CONCRETE] = CreateMaterial(0.6f, 0.5f, 0.f);
	mMaterials[MATERIAL::GLASS] = CreateMaterial(0.4f, 0.3f, 0.f);
}

void PhysicsSystem::Init()
{
	//get all entities with physics components
	for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
		mPX.mScene->removeActor(*itr->second.mActor);
	mActors.clear();
	mTriggerCollisions.clear();

	auto view = systemManager->ecs->GetEntitiesWith<Transform, RigidBody>();
	for (Entity e : view)
		CreateRigidBody(e);
}

void PhysicsSystem::Update(float dt)
{
	if (dt <= 0)
		return;

	// fixed time step
	for (unsigned step = 0; step < Accumulator::mSteps; ++step)
	{
		mPX.mScene->simulate(Accumulator::mFixedDT);
		mPX.mScene->fetchResults(true);
	}

	// sync with ecs
	Synchronize();
}

void PhysicsSystem::Exit()
{
	mActors.clear();
	mTriggerCollisions.clear();
}

void PhysicsSystem::AddEntity(Entity e)
{
	if (mActors.find(static_cast<uint32_t>(e.id)) != mActors.end())
	{
		PWARNING("Tried to add actor that is already in simulation!");
		return;
	}
	if (e.HasComponent<RigidBody>())
		CreateRigidBody(e);
	else
		PWARNING("Tried to add actor without rigid body to simulation!");
}

void PhysicsSystem::SetPosition(Entity e, const glm::vec3& globalpose)
{
	if (!e.HasComponent<RigidBody>()) return;
	RigidBody rbod = e.GetComponent<RigidBody>();
	if (rbod.mMotion == MOTION::STATIC) return;

	PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
	actor->setGlobalPose(PxTransform(Convert(globalpose), Convert(glm::quat(glm::radians(e.GetComponent<Transform>().mRotate)))));
}

void PhysicsSystem::SetRotation(Entity e, const glm::vec3& rotation)
{
	if (!e.HasComponent<RigidBody>()) return;
	RigidBody rbod = e.GetComponent<RigidBody>();
	if (rbod.mMotion == MOTION::STATIC) return;

	PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
	actor->setGlobalPose(PxTransform(Convert(e.GetComponent<Transform>().mTranslate), Convert(glm::quat(glm::radians(rotation)))));
}

void PhysicsSystem::SetVelocity(Entity e, const glm::vec3& velocity)
{
	if (!e.HasComponent<RigidBody>()) return;
	RigidBody rbod = e.GetComponent<RigidBody>();
	if (rbod.mMotion == MOTION::STATIC) return;

	physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
	actor->setAngularDamping(0.5f);
	actor->setLinearVelocity(Convert(velocity));
}

void PhysicsSystem::RemoveActor(Entity e)
{
	if (!e.HasComponent<RigidBody>())
		return;
	//auto itr = mTriggerCollisions.find(static_cast<uint32_t>(e.id));
	//if (itr != mTriggerCollisions.end())
	//{
	//	mTriggerCollisions.erase(itr);
	//}
	//else
	//{
	//	for (itr = mTriggerCollisions.begin(); itr != mTriggerCollisions.end(); ++itr)
	//	{
	//		Entity trigger = itr->first;
	//		std::vector<uint32_t>& vec = itr->second;
	//		auto temp = std::find(vec.begin(), vec.end(), static_cast<uint32_t>(e.id));
	//		if (temp == vec.end())
	//			continue;
	//		trigger.GetComponent<Scripts>().RunFunctionForAllScripts("OnTriggerExit", e);
	//		vec.erase(temp);
	//	}
	//}
	if (mActors.count(static_cast<uint32_t>(e.id)) == 0)
		return;
	mPX.mScene->removeActor(*mActors[static_cast<uint32_t>(e.id)].mActor);
	mActors.erase(static_cast<uint32_t>(e.id));
}

physx::PxMaterial* PhysicsSystem::CreateMaterial(float us, float ud, float res)
{
	return mPX.mPhysics->createMaterial(us, ud, res);
}

void PhysicsSystem::CreateRigidBody(Entity e)
{
	// retrieve entity info
	Transform xform = e.GetComponent<Transform>();
	RigidBody rbod = e.GetComponent<RigidBody>();
	glm::vec3 childOffset = e.HasParent() ? Entity(e.GetParent()).GetComponent<Transform>().mTranslate : glm::vec3(0);

	PxRigidActor* actor{};
	CreateActor(actor, PxTransform(Convert(xform.mTranslate + childOffset), 
		Convert(glm::quat(glm::radians(xform.mRotate)))), rbod);

	if (e.HasComponent<CapsuleCollider>())
	{
		actor->setGlobalPose(PxTransform(Convert(xform.mTranslate + childOffset)));
		PxShape* shape{};
		CapsuleCollider cap = e.GetComponent<CapsuleCollider>();
		CreateAndAttachShape(actor,
			shape, 
			PxCapsuleGeometry(cap.mRadius, cap.mHalfHeight), 
			PxTransform(Convert(cap.mTranslateOffset), PxQuat(PxHalfPi, PxVec3T<float>(0, 0, 1))),
			rbod, 
			cap.mIsTrigger);
		if (cap.mIsTrigger)
			mTriggerCollisions[static_cast<uint32_t>(e.id)] = std::vector<uint32_t>();
	}

	if (e.HasComponent<BoxCollider>())
	{
		PxShape* shape{};
		BoxCollider col = e.GetComponent<BoxCollider>();
		CreateAndAttachShape(actor,
			shape, 
			PxBoxGeometry(Convert(xform.mScale * col.mScaleOffset) / 2.f), 
			PxTransform(Convert(col.mTranslateOffset)),
			rbod, 
			col.mIsTrigger); //PxRigidActorExt::createExclusiveShape();
		if (col.mIsTrigger)
			mTriggerCollisions[static_cast<uint32_t>(e.id)] = std::vector<uint32_t>();
	}

	if (e.HasComponent<SphereCollider>())
	{
		PxShape* shape{};
		SphereCollider col = e.GetComponent<SphereCollider>();
		CreateAndAttachShape(actor,
			shape, 
			PxSphereGeometry(col.mScaleOffset), 
			PxTransform(Convert(col.mTranslateOffset)),
			rbod, 
			col.mIsTrigger);
		if (col.mIsTrigger)
			mTriggerCollisions[static_cast<uint32_t>(e.id)] = std::vector<uint32_t>();
	}
	
	if (rbod.mMotion == MOTION::DYNAMIC)
		PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), rbod.mDensity);

	// create actor object
	Actor actorObject;
	actorObject.mEntity = static_cast<uint32_t>(e.id);
	actorObject.mActor = actor;


	// add to scene
	mActors[static_cast<uint32_t>(e.id)] = actorObject;

	// assign user data
	actorObject.mActor->userData = &mActors[static_cast<uint32_t>(e.id)].mEntity;

	mPX.mScene->addActor(*actorObject.mActor);
}

void PhysicsSystem::CreateActor(PxRigidActor*& actor, const PxTransform& pxform, const RigidBody& rbod)
{
	if (rbod.mMotion == MOTION::DYNAMIC)
	{
		actor = mPX.mPhysics->createRigidDynamic(pxform);
		float temp = rbod.mDensity;
		static_cast<PxRigidDynamic*>(actor)->setLinearVelocity(Convert(rbod.mVelocity));

		PxRigidDynamicLockFlags axis;
		if (rbod.mRotationConstraints.x)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
		if (rbod.mRotationConstraints.y)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
		if (rbod.mRotationConstraints.z)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
		static_cast<PxRigidDynamic*>(actor)->setRigidDynamicLockFlags(axis);
		static_cast<PxRigidDynamic*>(actor)->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !rbod.mGravity);
		return;
	}
	actor = mPX.mPhysics->createRigidStatic(pxform);
}

void PhysicsSystem::Synchronize()
{
	for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
	{
		Entity e = itr->first;

		if (e.GetComponent<RigidBody>().mMotion == MOTION::STATIC)
			continue;

		PxRigidActor* actor = itr->second.mActor;

		// retrieving positions in physx
		PxTransform PXform = actor->getGlobalPose();
		Transform& refXform = e.GetComponent<Transform>();

		// overwriting entities with updated positions
		if (e.HasParent())
		{
			Transform parentXform = e.GetParent().GetComponent<Transform>();
			refXform.mTranslate = Convert(PXform.p) - parentXform.mTranslate;
		}
		else
		{
			refXform.mTranslate = Convert(PXform.p);
		}

		RigidBody& rbod = e.GetComponent<RigidBody>();
		glm::vec3 rotation = glm::eulerAngles(Convert(PXform.q));
		
		if (!rbod.mRotationConstraints.x)
			refXform.mRotate.x = rotation.x;
		if (!rbod.mRotationConstraints.y)
			refXform.mRotate.y = rotation.y;
		if (!rbod.mRotationConstraints.z)
			refXform.mRotate.z = rotation.z;

		// update velocity
		rbod.mVelocity = Convert(static_cast<physx::PxRigidDynamic*>(itr->second.mActor)->getLinearVelocity());
	}
}
