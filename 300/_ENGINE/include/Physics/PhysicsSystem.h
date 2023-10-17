/*!*****************************************************************************
\file PhysicsSystem.h
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Pepe Production
\date 15/08/2023
\brief
Physics system.
*******************************************************************************/
#pragma once
#include "PhysX.h"
#include "pch.h"
#include "PhysicsTypes.h"
#include "ECS/ECS_Components.h"

using namespace physx;

class PhysicsSystem
{
public:
	/*!*****************************************************************************
	Construct physics system
	*******************************************************************************/
	PhysicsSystem();
	/*!*****************************************************************************
	Initialize physics system, called on play.
	*******************************************************************************/
	void Init();
	/*!*****************************************************************************
	Update, called every frame unless paused.
	*******************************************************************************/
	void Update(float dt);
	/*!*****************************************************************************
	Clean up if any.
	*******************************************************************************/
	void Exit();
	/*!*****************************************************************************
	Adds a new entity to the physics simulation
	*******************************************************************************/
	void AddActor(Entity e);
	/*!*****************************************************************************
	Set velocity of an entity.
	*******************************************************************************/
	void SetVelocity(Entity e, const glm::vec3 &velocity);

private:
	PhysX mPX;
	float mFixedDT;
	std::unordered_map<std::uint32_t, PxRigidActor*> mActors;
	std::unordered_map<MATERIAL, PxMaterial*> mMaterials;
	/*!*****************************************************************************
	Create materials for entities.
	*******************************************************************************/
	PxMaterial *CreateMaterial(float us, float ud, float res);
	/*!*****************************************************************************
	Helper function to create rigid body for an entity.
	*******************************************************************************/
	void CreateRigidBody(Entity e);

	/*!*****************************************************************************
	Helper functions to convert glm to physx and vice versa.
	*******************************************************************************/
	PxVec3T<float> Convert(const glm::vec3 &vec);
	PxVec4T<float> Convert(const glm::vec4 &vec);
	glm::vec3 Convert(const PxVec3T<float> &vec);
	glm::quat Convert(const PxQuatT<float> &vec);

	// for performance
	double startTime, endTime;
};