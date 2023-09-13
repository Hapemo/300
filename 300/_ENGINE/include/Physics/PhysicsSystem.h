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


	// Helper Functions
	void CreateBoxCollider(physx::PxRigidActor* actor, const Transform& xform, const BoxCollider& collider);
	void CreateSphereCollider(physx::PxRigidActor* actor, const Transform& xform, const SphereCollider& collider);
	void CreatePlaneCollider(physx::PxRigidActor* actor, const Transform& xform, const PlaneCollider& collider);
	physx::PxVec3T<float> Convert(const glm::vec3& vec);
};