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
	mActors.clear();
	auto view = systemManager->ecs->GetEntitiesWith<Transform, RigidBody>();
	
	for (Entity e : view)
		CreateRigidBody(e);
}

void PhysicsSystem::Update(float dt)
{
	if (dt <= 0)
		return;

	for (int step = 0; step < Accumulator::mSteps; ++step)
	{
		mPX.mScene->simulate(Accumulator::mFixedDT);
		mPX.mScene->fetchResults(true);
	}

	for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
	{
		if (Entity(itr->first).HasComponent<BoxCollider>())	//remove rotation from aabb
			static_cast<PxRigidDynamic*>(itr->second)->setGlobalPose(PxTransform(static_cast<PxRigidDynamic*>(itr->second)->getGlobalPose().p));
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

void PhysicsSystem::SetVelocity(Entity e, const glm::vec3& velocity)
{
	if (e.HasComponent<PlaneCollider>()) return;
	if (!e.HasComponent<RigidBody>()) return;
	RigidBody rbod = e.GetComponent<RigidBody>();
	if (rbod.mMotion == MOTION::STATIC) return;

	physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)]);
	actor->setAngularDamping(0.5f);
	actor->setLinearVelocity(Convert(velocity));
}

physx::PxMaterial* PhysicsSystem::CreateMaterial(float us, float ud, float res)
{
	return mPX.mPhysics->createMaterial(us, ud, res);
}

void PhysicsSystem::CreateRigidBody(Entity e)
{
	Transform xform = e.GetComponent<Transform>();
	if (e.HasComponent<PlaneCollider>())
	{
		RigidBody rbod = e.GetComponent<RigidBody>();
		PlaneCollider col = e.GetComponent<PlaneCollider>();
		PxRigidStatic* plane = PxCreatePlane(*mPX.mPhysics, PxPlane(Convert(col.mNormal), glm::length(xform.mTranslate) + col.mTranslateOffset), *mMaterials[rbod.mMaterial]);
		mActors[static_cast<uint32_t>(e.id)] = plane;
		mPX.mScene->addActor(*plane);
		return;
	}
	if (e.HasComponent<BoxCollider>())
	{
		RigidBody rbod = e.GetComponent<RigidBody>();
		BoxCollider col = e.GetComponent<BoxCollider>();
		PxShape* shape = mPX.mPhysics->createShape(PxBoxGeometry(Convert(xform.mScale * col.mScaleOffset) / 2.f), *mMaterials[rbod.mMaterial]);
		PxRigidActor* actor{};
		if (rbod.mMotion == MOTION::DYNAMIC)
		{
			actor = mPX.mPhysics->createRigidDynamic(PxTransform(Convert(xform.mTranslate + col.mTranslateOffset)));
			actor->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), rbod.mDensity);
			static_cast<PxRigidDynamic*>(actor)->setLinearVelocity(Convert(rbod.mVelocity));
		}
		else
		{
			actor = mPX.mPhysics->createRigidStatic(PxTransform(Convert(xform.mTranslate + col.mTranslateOffset)));
			actor->attachShape(*shape);
		}
		mActors[static_cast<uint32_t>(e.id)] = actor;
		mPX.mScene->addActor(*actor);
		shape->release();
		return;
	}
	if (e.HasComponent<SphereCollider>())
	{
		RigidBody rbod = e.GetComponent<RigidBody>();
		SphereCollider col = e.GetComponent<SphereCollider>();
		PxShape* shape = mPX.mPhysics->createShape(PxSphereGeometry(std::max({ xform.mScale.x, xform.mScale.y, xform.mScale.z }) * col.mScaleOffset / 2.f), *mMaterials[rbod.mMaterial]);
		PxRigidActor* actor{};
		if (rbod.mMotion == MOTION::DYNAMIC)
		{
			actor = mPX.mPhysics->createRigidDynamic(PxTransform(Convert(xform.mTranslate + col.mTranslateOffset)));
			actor->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), rbod.mDensity);
			static_cast<PxRigidDynamic*>(actor)->setLinearVelocity(Convert(rbod.mVelocity));
		}
		else
		{
			actor = mPX.mPhysics->createRigidStatic(PxTransform(Convert(xform.mTranslate + col.mTranslateOffset)));
			actor->attachShape(*shape);
		}
		mActors[static_cast<uint32_t>(e.id)] = actor;
		mPX.mScene->addActor(*actor);
		shape->release();
		return;
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