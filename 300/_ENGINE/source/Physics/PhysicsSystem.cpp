#include "Physics/PhysicsSystem.h"
#include "ECS/ECS.h"
#include "Debug/EnginePerformance.h"
#include "Physics/Accumulator.h"

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
		mPX.mScene->removeActor(*itr->second);
	mActors.clear();
	auto view = systemManager->ecs->GetEntitiesWith<Transform, RigidBody>();
	
	for (Entity e : view)
	{
		if (e.HasComponent<RigidBody>())
			CreateRigidBody(e);
	}
}

void PhysicsSystem::Update(float dt)
{
	if (dt <= 0)
		return;

	for (unsigned step = 0; step < Accumulator::mSteps; ++step)
	{
		mPX.mScene->simulate(Accumulator::mFixedDT);
		mPX.mScene->fetchResults(true);
	}

	for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
	{
		physx::PxTransform PXform = itr->second->getGlobalPose();
		Transform& xform = Entity(itr->first).GetComponent<Transform>();
		xform.mTranslate = Convert(PXform.p);
		if (Entity(itr->first).HasComponent<BoxCollider>())
			xform.mTranslate -= Entity(itr->first).GetComponent<BoxCollider>().mTranslateOffset;
		xform.mRotate = glm::vec3(0,0,0);
		RigidBody& rbod = Entity(itr->first).GetComponent<RigidBody>();
		if (rbod.mMotion != MOTION::DYNAMIC)
			continue;
		rbod.mVelocity = Convert(static_cast<physx::PxRigidDynamic*>(itr->second)->getLinearVelocity());
	}
}

void PhysicsSystem::Exit()
{
	mActors.clear();
}

void PhysicsSystem::AddActor(Entity e)
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
	std::cout << "Set velocity to: " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl;
	if (e.HasComponent<PlaneCollider>()) return;
	if (!e.HasComponent<RigidBody>()) return;
	RigidBody rbod = e.GetComponent<RigidBody>();
	if (rbod.mMotion == MOTION::STATIC) return;

	physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)]);
	actor->setAngularDamping(0.5f);
	actor->setLinearVelocity(Convert(velocity));
}

void PhysicsSystem::RemoveActor(Entity e)
{
	mPX.mScene->removeActor(*mActors[static_cast<uint32_t>(e.id)]);
	mActors.erase(static_cast<uint32_t>(e.id));
}

physx::PxMaterial* PhysicsSystem::CreateMaterial(float us, float ud, float res)
{
	return mPX.mPhysics->createMaterial(us, ud, res);
}

void PhysicsSystem::CreateRigidBody(Entity e)
{
	if (e.HasComponent<PlaneCollider>())
	{
		//PlaneCollider col = e.GetComponent<PlaneCollider>();
		//PxRigidStatic* plane = PxCreatePlane(*mPX.mPhysics, PxPlane(Convert(col.mNormal), glm::length(xform.mTranslate + xtraxlate) + col.mTranslateOffset), *mMaterials[rbod.mMaterial]);
		//mActors[static_cast<uint32_t>(e.id)] = plane;
		//mPX.mScene->addActor(*plane);
		return;
	}

	Transform xform = e.GetComponent<Transform>();
	glm::vec3 xtranslate = e.HasParent() ? Entity(e.GetParent()).GetComponent<Transform>().mTranslate : glm::vec3(0);
	RigidBody rbod = e.GetComponent<RigidBody>();
	PxRigidActor* actor{};
	PxShape* shape{};

	if (e.HasComponent<CapsuleCollider>())
	{
		CapsuleCollider cap = e.GetComponent<CapsuleCollider>();
		shape = mPX.mPhysics->createShape(PxCapsuleGeometry(cap.mRadius, cap.mHalfHeight), *mMaterials[rbod.mMaterial]);
		AttachMotionType(actor, shape, PxTransform(Convert(xform.mTranslate + cap.mTranslateOffset + xtranslate)), rbod, glm::ivec3(1, 1, 1));
	}
	else if (e.HasComponent<BoxCollider>())
	{
		BoxCollider col = e.GetComponent<BoxCollider>();
		shape = mPX.mPhysics->createShape(PxBoxGeometry(Convert(xform.mScale * col.mScaleOffset) / 2.f), *mMaterials[rbod.mMaterial]);
		AttachMotionType(actor, shape, PxTransform(Convert(xform.mTranslate + col.mTranslateOffset + xtranslate)), rbod);
	}
	else if (e.HasComponent<SphereCollider>())
	{
		SphereCollider col = e.GetComponent<SphereCollider>();
		shape = mPX.mPhysics->createShape(PxSphereGeometry(std::max({ xform.mScale.x, xform.mScale.y, xform.mScale.z }) * col.mScaleOffset / 2.f), *mMaterials[rbod.mMaterial]);
		AttachMotionType(actor, shape, PxTransform(Convert(xform.mTranslate + col.mTranslateOffset + xtranslate)), rbod);
	}
	else if (e.HasComponent<AABBCollider>())
	{
		AABBCollider col = e.GetComponent<AABBCollider>();
		shape = mPX.mPhysics->createShape(PxBoxGeometry(Convert(xform.mScale * col.mScaleOffset) / 2.f), *mMaterials[rbod.mMaterial]);
		AttachMotionType(actor, shape, PxTransform(Convert(xform.mTranslate + col.mTranslateOffset + xtranslate)), rbod, glm::ivec3(1, 1, 1));
	}
	else return;

	mActors[static_cast<uint32_t>(e.id)] = actor;
	mPX.mScene->addActor(*actor);
	shape->release();
}

void PhysicsSystem::AttachMotionType(PxRigidActor*& actor, PxShape*& shape, const PxTransform& pxform, const RigidBody& rbod, const glm::ivec3& axisLocks)
{
	if (rbod.mMotion == MOTION::DYNAMIC)
	{
		actor = mPX.mPhysics->createRigidDynamic(pxform);
		actor->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), rbod.mDensity);
		static_cast<PxRigidDynamic*>(actor)->setLinearVelocity(Convert(rbod.mVelocity));
		PxRigidDynamicLockFlags axis;
		if (axisLocks.x)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
		if (axisLocks.y)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
		if (axisLocks.z)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
		static_cast<PxRigidDynamic*>(actor)->setRigidDynamicLockFlags(axis);
	}
	else
	{
		actor = mPX.mPhysics->createRigidStatic(pxform);
		actor->attachShape(*shape);
	}
}

physx::PxVec3T<float> PhysicsSystem::Convert(const glm::vec3& vec)
{
	return physx::PxVec3T<float>(vec.x, vec.y, vec.z);
}

physx::PxVec4T<float> PhysicsSystem::Convert(const glm::vec4& vec)
{
	return physx::PxVec4T<float>(vec.x, vec.y, vec.z, vec.w);
}

glm::vec3 PhysicsSystem::Convert(const physx::PxVec3T<float>& vec)
{
	return { vec.x, vec.y, vec.z };
}

glm::quat PhysicsSystem::Convert(const physx::PxQuatT<float>& vec)
{
	return { vec.x, vec.y, vec.z, vec.w };
}