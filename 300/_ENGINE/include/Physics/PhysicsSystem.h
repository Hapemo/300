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
	std::unordered_map<std::uint32_t, physx::PxRigidActor*> mActors;
	std::unordered_map<MATERIAL, physx::PxMaterial*> mMaterials;
	physx::PxMaterial* CreateMaterial(float us, float ud, float res);
	void CreateActor(Entity e, const Transform& xform, const RigidBody& rbod);
	physx::PxVec3T<float> Convert(const glm::vec3& vec);
};