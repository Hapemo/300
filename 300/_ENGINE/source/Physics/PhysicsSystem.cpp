#include "Physics/PhysicsSystem.h"
#include "ECS/ECS.h"
#include "Debug/EnginePerformance.h"
#include "Physics/Accumulator.h"
#include "Physics/PhysXUtils.h"

PhysicsSystem::PhysicsSystem()
{
	mMaterials[MATERIAL::RUBBER] = CreateMaterial(0.9f, 0.8f, 0.2f);
	mMaterials[MATERIAL::WOOD] = CreateMaterial(0.5f, 0.4f, 0.3f);
	mMaterials[MATERIAL::METAL] = CreateMaterial(0.7f, 0.6f, 0.05f);
	mMaterials[MATERIAL::ICE] = CreateMaterial(0.1f, 0.05f, 0.1f);
	mMaterials[MATERIAL::CONCRETE] = CreateMaterial(0.6f, 0.5f, 0.2f);
	mMaterials[MATERIAL::GLASS] = CreateMaterial(0.4f, 0.3f, 0.7f);
}

void PhysicsSystem::Init()
{
	//get all entities with physics components
	for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
		mPX.mScene->removeActor(*itr->second.mActor);
	mActors.clear();

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
	CreateActor(actor, PxTransform(Convert(xform.mTranslate + childOffset)), rbod);

	if (e.HasComponent<CapsuleCollider>())
	{
		PxShape* shape{};
		CapsuleCollider cap = e.GetComponent<CapsuleCollider>();
		CreateShape(shape, 
			PxCapsuleGeometry(cap.mRadius, cap.mHalfHeight), 
			rbod, 
			cap.mIsTrigger);
		AttachShape(actor,
			shape,
			PxTransform(Convert(cap.mTranslateOffset), PxQuat(PxHalfPi, PxVec3T<float>(0, 0, 1))));
	}

	if (e.HasComponent<BoxCollider>())
	{
		PxShape* shape{};
		BoxCollider col = e.GetComponent<BoxCollider>();
		CreateShape(shape, 
			PxBoxGeometry(Convert(xform.mScale * col.mScaleOffset) / 2.f), 
			rbod, 
			col.mIsTrigger); //PxRigidActorExt::createExclusiveShape();
		AttachShape(actor, 
			shape, 
			PxTransform(Convert(col.mTranslateOffset)));
	}

	if (e.HasComponent<SphereCollider>())
	{
		PxShape* shape{};
		SphereCollider col = e.GetComponent<SphereCollider>();
		CreateShape(shape, 
			PxSphereGeometry(std::max({ xform.mScale.x, xform.mScale.y, xform.mScale.z }) * col.mScaleOffset / 2.f), 
			rbod, 
			col.mIsTrigger);
		AttachShape(actor,
			shape,
			PxTransform(Convert(col.mTranslateOffset)));
	}
	
	if (rbod.mMotion == MOTION::DYNAMIC)
		PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), rbod.mDensity);

	// create actor object
	Actor actorObject;
	actorObject.mEntity = static_cast<uint32_t>(e.id);
	actorObject.mActor = actor;

	// assign user data
	actorObject.mActor->userData = &actorObject.mEntity;

	// add to scene
	mActors[static_cast<uint32_t>(e.id)] = actorObject;
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
		axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
		axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
		axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
		static_cast<PxRigidDynamic*>(actor)->setRigidDynamicLockFlags(axis);

		return;
	}
	actor = mPX.mPhysics->createRigidStatic(pxform);
}

void PhysicsSystem::AttachShape(PxRigidActor*& actor, PxShape*& shape, const PxTransform& localPose)
{
	shape->setLocalPose(localPose);
	actor->attachShape(*shape);
	shape->release();
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

		// update velocity
		RigidBody& rbod = e.GetComponent<RigidBody>();
		rbod.mVelocity = Convert(static_cast<physx::PxRigidDynamic*>(itr->second.mActor)->getLinearVelocity());
	}
}
