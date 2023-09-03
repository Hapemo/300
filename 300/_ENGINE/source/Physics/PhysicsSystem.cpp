#include "Physics/PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}

void PhysicsSystem::Init()
{
	CreateMaterial(MATERIAL::RUBBER, 0.9f, 0.8f, 0.2f);
	CreateMaterial(MATERIAL::WOOD, 0.5f, 0.4f, 0.3f);
	CreateMaterial(MATERIAL::METAL, 0.7f, 0.6f, 0.05f);
	CreateMaterial(MATERIAL::ICE, 0.1f, 0.05f, 0.1f);
	CreateMaterial(MATERIAL::CONCRETE, 0.6f, 0.5f, 0.2f);
	CreateMaterial(MATERIAL::GLASS, 0.4f, 0.3f, 0.7f);
}

void PhysicsSystem::Update(float dt)
{
}

void PhysicsSystem::Exit()
{
}

void PhysicsSystem::CreateMaterial(MATERIAL material, float us, float uk, float e)
{
	mMaterialList.insert(std::pair<MATERIAL, physx::PxMaterial*>(material, mPhysX.mPhysics->createMaterial(us, uk, e)));
}
