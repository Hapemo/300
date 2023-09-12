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
	auto view = ECS::GetInstance()->GetEntitiesWith<Transform, RigidBody>();
	
	for (Entity e : view)
	{
		auto [xform, rbod] = view.get<Transform, RigidBody>(e.id);
		CreateActor(xform, rbod);
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

void PhysicsSystem::CreateActor(const Transform& xform, const RigidBody& rbod)
{
	
}