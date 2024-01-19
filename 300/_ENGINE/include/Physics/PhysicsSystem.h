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
#define HIT_BUFFER_SIZE 32

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
	void SetPosition(Entity e, const glm::vec3& globalpose);
	void SetRotation(Entity e, const glm::vec3& rotation);
	/*!*****************************************************************************
	Set velocity of an entity.
	*******************************************************************************/
	void SetVelocity(Entity e, const glm::vec3 &velocity);
	void RemoveActor(Entity e);

	std::vector<Entity> Visible(const glm::vec3& origin, const glm::vec3& finalpos, float maxdist = FLT_MAX);

private:
	PhysX mPX;
	bool mIsSimulationRunning;
	float mFixedDT;
	std::unordered_map<std::uint32_t, Actor> mActors;
	std::unordered_map<MATERIAL, PxMaterial*> mMaterials;
	std::vector<Entity> mPendingAdd;
	std::vector<std::pair<Entity, glm::vec3>> mPendingTranslate;
	std::vector<std::pair<Entity, glm::vec3>> mPendingRotate;
	std::vector<std::pair<Entity, glm::vec3>> mPendingVelocity;
	/*!*****************************************************************************
	Create materials for entities.
	*******************************************************************************/
	PxMaterial *CreateMaterial(float us, float ud, float res);
	/*!*****************************************************************************
	Helper function to create rigid body for an entity.
	*******************************************************************************/
	void CreateRigidBody(Entity e);

	void CreateActor(PxRigidActor*& actor, const PxTransform& pxform, const RigidBody& rbod);

	void CreateMeshCollider(const std::vector<glm::vec3>& vtx, const std::vector<unsigned int>& idx, PxTriangleMesh*& triMesh,
		bool skipCleanup = false, bool skipEdge = false, const PxU32 triPerLeaf = 4);

	void SetUpCooking(PxCookingParams& params, bool skipMeshCleanup, bool skipEdgeData);

	template <typename T>
	void CreateAndAttachShape(PxRigidActor*& actor, PxShape*& shape, 
		const T& geometry, const PxTransform& pose, 
		const RigidBody& rbod, bool isTrigger);

	void MoveQueuedEntities();

	void Synchronize();

	PxRaycastHit hitBuffer[HIT_BUFFER_SIZE];
	//PxRigidDynamicLockFlags Convert(const glm::ivec3& vec);
};

template <typename T>
void PhysicsSystem::CreateAndAttachShape(PxRigidActor*& actor, PxShape*& shape,
	const T& geometry, const PxTransform& pose,
	const RigidBody& rbod, bool isTrigger)
{
	PxShapeFlags flags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE;
	if (isTrigger)
		flags |= PxShapeFlag::eTRIGGER_SHAPE;
	else
		flags |= PxShapeFlag::eSIMULATION_SHAPE;
	shape = PxRigidActorExt::createExclusiveShape(*actor, geometry, *mMaterials[rbod.mMaterial], flags);
	PxFilterData motionFilter(static_cast<uint32_t>(rbod.mMotion), 0, 0, 0);
	shape->setSimulationFilterData(motionFilter);
	shape->setLocalPose(pose);
}