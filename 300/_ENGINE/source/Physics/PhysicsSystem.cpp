#include "Physics/PhysicsSystem.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"

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
	//physics simulate
}

void PhysicsSystem::Exit()
{
}

physx::PxMaterial* PhysicsSystem::CreateMaterial(float us, float ud, float res)
{
	return mPX.mPhysics->createMaterial(us, ud, res);
}

void PhysicsSystem::CreateActor(Entity e, const Transform& xform, const RigidBody& rbod)
{
	if (e.HasComponent<BoxCollider>())
	{
		BoxCollider collider = e.GetComponent<BoxCollider>();
		physx::PxTransform PXform = physx::PxTransform(Convert(xform.mTranslate + collider.mTranslateOffset));
		physx::PxRigidActor* PActor{};

		if (rbod.mMotion == MOTION::STATIC)
			PActor = mPX.mPhysics->createRigidStatic(PXform);
		else if (rbod.mMotion == MOTION::DYNAMIC)
			PActor = mPX.mPhysics->createRigidDynamic(PXform);

		mActors[static_cast<std::uint32_t>(e.id)] = PActor;

		physx::PxBoxGeometry PGeom(Convert(collider.mScaleOffset * xform.mScale));
		physx::PxShape* shape = mPX.mPhysics->createShape(PGeom, *mMaterials[rbod.mMaterial]);
		PActor->attachShape(*shape);
	}
}

physx::PxVec3T<float> PhysicsSystem::Convert(const glm::vec3& vec)
{
	return physx::PxVec3T<float>(vec.x, vec.y, vec.z);
}