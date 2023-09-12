#pragma once
#include "PhysX.h"
#include "pch.h"
#include "PhysicsTypes.h"
#include "ECS/ECS_Components.h"

class PhysicsSystem
{
public:
	PhysicsSystem();
	void Init();
	void Update(float dt);
	void Exit();

private:
	PhysX mPX;
	std::unordered_map<Entity, physx::PxActor*> mActors;
	std::unordered_map<MATERIAL, physx::PxMaterial*> mMaterials;
	physx::PxMaterial* CreateMaterial(float us, float ud, float res);
	void CreateActor(Entity e, const Transform& xform, const RigidBody& rbod);
	template <typename Type>
	physx::PxVec3T<Type> Convert(const glm::vec3& vec);
};

template <typename Type>
physx::PxVec3T<Type> PhysicsSystem::Convert(const glm::vec3& vec)
{
	return physx<Type>(vec.x, vec.y, vec.z);
}