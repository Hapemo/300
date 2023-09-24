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
	void CreateCollider(physx::PxRigidActor*& actor, const Transform& xform, const RigidBody& rbod, const BoxCollider& collider);
	void CreateCollider(physx::PxRigidActor*& actor, const Transform& xform, const RigidBody& rbod, const SphereCollider& collider);
	void CreateCollider(physx::PxRigidActor*& actor, const Transform& xform, const RigidBody& rbod, const PlaneCollider& collider);
	void CreateActorType(physx::PxRigidActor*& actor, const physx::PxTransform& p_xform, MOTION motion);

	physx::PxVec3T<float> Convert(const glm::vec3& vec);
	physx::PxVec4T<float> Convert(const glm::vec4& vec);

	glm::vec3 Convert(const physx::PxVec3T<float>& vec);
	glm::quat Convert(const physx::PxQuatT<float>& vec);

	// for performance
	double startTime, endTime;
};