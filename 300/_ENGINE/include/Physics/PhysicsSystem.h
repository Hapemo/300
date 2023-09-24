#pragma once
#include "PhysX.h"
#include "pch.h"
#include "PhysicsTypes.h"
#include "ECS/ECS_Components.h"

using namespace physx;

class PhysicsSystem
{
public:
	PhysicsSystem();
	void Init();
	void Update(float dt);
	void Exit();

	void SetVelocity(Entity e, const glm::vec3& velocity);

private:
	PhysX mPX;
	std::unordered_map<std::uint32_t, PxRigidActor*> mActors;
	std::unordered_map<MATERIAL, PxMaterial*> mMaterials;
	PxMaterial* CreateMaterial(float us, float ud, float res);

	PxVec3T<float> Convert(const glm::vec3& vec);
	PxVec4T<float> Convert(const glm::vec4& vec);

	glm::vec3 Convert(const PxVec3T<float>& vec);
	glm::quat Convert(const PxQuatT<float>& vec);
};