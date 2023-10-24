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
#include "Actor.h"

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
	void AddEntity(Entity e);

	/*!*****************************************************************************
	Set velocity of an entity.
	*******************************************************************************/
	void SetVelocity(Entity e, const glm::vec3 &velocity);
	void RemoveActor(Entity e);

private:
	PhysX mPX;
	float mFixedDT;
	std::unordered_map<std::uint32_t, Actor> mActors;
	std::unordered_map<MATERIAL, PxMaterial*> mMaterials;

	/*!*****************************************************************************
	Create materials for entities.
	*******************************************************************************/
	PxMaterial *CreateMaterial(float us, float ud, float res);
	/*!*****************************************************************************
	Helper function to create rigid body for an entity.
	*******************************************************************************/
	void CreateRigidBody(Entity e);

	void CreateActor(PxRigidActor*& actor, const PxTransform& pxform, const RigidBody& rbod);

	template <typename T>
	void CreateShape(PxShape*& shape, const T& geometry, const RigidBody& rbod, bool isTrigger);

	void AttachShape(PxRigidActor*& actor, PxShape*& shape, const PxTransform& localPose);
	
	void Synchronize();
	//PxRigidDynamicLockFlags Convert(const glm::ivec3& vec);

	// for performance
	double startTime, endTime;
};

template <typename T>
void PhysicsSystem::CreateShape(PxShape*& shape, const T& geometry, const RigidBody& rbod, bool isTrigger)
{
	shape = mPX.mPhysics->createShape(geometry, *mMaterials[rbod.mMaterial]);
	shape->setFlag(PxShapeFlag::Enum::eSIMULATION_SHAPE, !isTrigger);
	shape->setFlag(PxShapeFlag::Enum::eTRIGGER_SHAPE, isTrigger);
}