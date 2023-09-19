#include "Physics/PhysicsSystem.h"
#include "ECS/ECS.h"

PhysicsSystem::PhysicsSystem()
{
	mMaterials[MATERIAL::RUBBER] = CreateMaterial(0.9, 0.8, 0.2);
	mMaterials[MATERIAL::WOOD] = CreateMaterial(0.5, 0.4, 0.3);
	mMaterials[MATERIAL::METAL] = CreateMaterial(0.7, 0.6, 0.05);
	mMaterials[MATERIAL::ICE] = CreateMaterial(0.1, 0.05, 0.1);
	mMaterials[MATERIAL::CONCRETE] = CreateMaterial(0.6, 0.5, 0.2);
	mMaterials[MATERIAL::GLASS] = CreateMaterial(0.4, 0.3, 0.7);
}

void PhysicsSystem::Init()
{
	//get all entities with physics components
	auto view = systemManager->ecs->GetEntitiesWith<Transform, RigidBody>();
	
	for (Entity e : view)
	{
		auto [xform, rbod] = view.get<Transform, RigidBody>(e.id);
		CreateActor(e, xform, rbod);
	}
}

void PhysicsSystem::Update(float dt)
{
	if (dt <= 0)
		return;
	mPX.mScene->simulate(dt);
	mPX.mScene->fetchResults(true);

	//for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
	//{
	//	physx::PxTransform PXform = itr->second->getGlobalPose();
	//	Transform& xform = Entity(itr->first).GetComponent<Transform>();
	//	xform.mTranslate = Convert(PXform.p);
	//	xform.mRotate = glm::eulerAngles(Convert(PXform.q));
	//}
}

void PhysicsSystem::Exit()
{
	mActors.clear();
}

physx::PxMaterial* PhysicsSystem::CreateMaterial(float us, float ud, float res)
{
	return mPX.mPhysics->createMaterial(us, ud, res);
}

void PhysicsSystem::CreateActor(Entity e, const Transform& xform, const RigidBody& rbod)
{
	physx::PxRigidActor* PActor{};

	if (e.HasComponent<BoxCollider>())
		CreateCollider(PActor, xform, rbod, e.GetComponent<BoxCollider>());
	else if (e.HasComponent<SphereCollider>())
		CreateCollider(PActor, xform, rbod, e.GetComponent<SphereCollider>());
	else if (e.HasComponent<PlaneCollider>())
		CreateCollider(PActor, xform, rbod, e.GetComponent<PlaneCollider>());

	mActors[static_cast<std::uint32_t>(e.id)] = PActor; 
	mPX.mScene->addActor(*PActor);
}

void PhysicsSystem::CreateCollider(physx::PxRigidActor*& actor, const Transform& xform, const RigidBody& rbod, const BoxCollider& collider)
{
	physx::PxTransform PXform = physx::PxTransform(Convert(xform.mTranslate + collider.mTranslateOffset));
	CreateActorType(actor, PXform, rbod.mMotion);

	physx::PxBoxGeometry PGeom(Convert(collider.mScaleOffset * xform.mScale));
	physx::PxShape* shape = mPX.mPhysics->createShape(PGeom, *mMaterials[rbod.mMaterial]);
	actor->attachShape(*shape);
}

void PhysicsSystem::CreateCollider(physx::PxRigidActor*& actor, const Transform& xform, const RigidBody& rbod, const SphereCollider& collider)
{
	physx::PxTransform PXform = physx::PxTransform(Convert(xform.mTranslate + collider.mTranslateOffset));
	CreateActorType(actor, PXform, rbod.mMotion);

	physx::PxSphereGeometry PGeom(collider.mScaleOffset * std::max({ xform.mScale.x, xform.mScale.y, xform.mScale.z }));
	physx::PxShape* shape = mPX.mPhysics->createShape(PGeom, *mMaterials[rbod.mMaterial]);
	actor->attachShape(*shape);
}

void PhysicsSystem::CreateCollider(physx::PxRigidActor*& actor, const Transform& xform, const RigidBody& rbod, const PlaneCollider& collider)
{
	glm::vec3 nml = collider.mNormal;
	actor = physx::PxCreatePlane(*mPX.mPhysics, physx::PxPlane(nml.x, nml.y, nml.z, glm::length(xform.mTranslate) + collider.mTranslateOffset), *mMaterials[rbod.mMaterial]);
}

void PhysicsSystem::CreateActorType(physx::PxRigidActor*& actor, const physx::PxTransform& p_xform, MOTION motion)
{
	if (motion == MOTION::STATIC)
		actor = mPX.mPhysics->createRigidStatic(p_xform);
	else if (motion == MOTION::DYNAMIC)
	{
		actor = mPX.mPhysics->createRigidDynamic(p_xform);
		physx::PxRigidBodyExt::updateMassAndInertia(static_cast<physx::PxRigidBody&>(*actor), 10.f);
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