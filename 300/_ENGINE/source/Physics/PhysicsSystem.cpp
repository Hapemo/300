#include "Physics/PhysicsSystem.h"
#include "ECS/ECS.h"
#include "Debug/EnginePerformance.h"
#include "Physics/Accumulator.h"
#include "Physics/PhysXUtils.h"

PhysicsSystem::PhysicsSystem()
{
	mMaterials[MATERIAL::RUBBER] = CreateMaterial(0.9f, 0.8f, 0.f);
	mMaterials[MATERIAL::WOOD] = CreateMaterial(0.5f, 0.4f, 0.f);
	mMaterials[MATERIAL::METAL] = CreateMaterial(0.7f, 0.6f, 0.0f);
	mMaterials[MATERIAL::ICE] = CreateMaterial(0.1f, 0.05f, 0.f);
	mMaterials[MATERIAL::CONCRETE] = CreateMaterial(0.6f, 0.5f, 0.f);
	mMaterials[MATERIAL::GLASS] = CreateMaterial(0.4f, 0.3f, 0.8f);
	mIsSimulationRunning = false;
}

void PhysicsSystem::Init()
{
	//get all entities with physics components
	for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
		mPX.mScene->removeActor(*itr->second.mActor);
	mActors.clear();

	auto view = systemManager->ecs->GetEntitiesWith<Transform, RigidBody>();
	for (Entity e : view)
		CreateRigidBody(e);
}

void PhysicsSystem::ResetEntity(Entity e) {
	mPX.mScene->removeActor(*mActors[(uint32_t)e.id].mActor);
	mActors.erase((uint32_t)e.id);
	CreateRigidBody(e);
}

void PhysicsSystem::Update(float dt)
{
	if (dt <= 0)
		return;

	// fixed time step
	mIsSimulationRunning = true;
	for (unsigned step = 0; step < Accumulator::mSteps; ++step)
	{
		mPX.mScene->simulate(Accumulator::mFixedDT);
		mPX.mScene->fetchResults(true);
	}
	mIsSimulationRunning = false;

	MoveQueuedEntities();

	// sync with ecs
	Synchronize();
}

void PhysicsSystem::Exit()
{
	mActors.clear();
}

void PhysicsSystem::AddEntity(Entity e)
{
	//std::cout << "adding entity:" << (int)e.id << std::endl;
	//std::cout << "adding entity:" << e.GetComponent<General>().name << std::endl;
	if (mIsSimulationRunning)
	{
		mPendingAdd.push_back(e);
		return;
	}
	if (mActors.find(static_cast<uint32_t>(e.id)) != mActors.end())
	{
		PWARNING("Tried to add actor that is already in simulation!");
		return;
	}
	if (e.HasComponent<RigidBody>())
		CreateRigidBody(e);
	else
		PWARNING("Tried to add actor without rigid body to simulation!");
}

void PhysicsSystem::SetPosition(Entity e, const glm::vec3& globalpose)
{
	mPendingTranslate.push_back(std::pair<Entity, glm::vec3>(e, globalpose));
}

void PhysicsSystem::SetRotation(Entity e, const glm::vec3& rotation)
{
	mPendingRotate.push_back(std::pair<Entity, glm::vec3>(e, rotation));
}

void PhysicsSystem::SetVelocity(Entity e, const glm::vec3& velocity)
{
	if (!e.HasComponent<RigidBody>()) return;
	RigidBody rbod = e.GetComponent<RigidBody>();
	if (rbod.mMotion == MOTION::STATIC) return;
	if (mIsSimulationRunning)
	{
		mPendingVelocity.push_back(std::pair<Entity, glm::vec3>(e, velocity));
		return;
	}

	physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
	if (actor == nullptr) return;
	actor->setAngularDamping(0.5f);
	actor->setLinearVelocity(Convert(velocity));
}

void PhysicsSystem::RemoveActor(Entity e)
{
	if (!e.HasComponent<RigidBody>())
		return;
	//auto itr = mTriggerCollisions.find(static_cast<uint32_t>(e.id));
	//if (itr != mTriggerCollisions.end())
	//{
	//	mTriggerCollisions.erase(itr);
	//}
	//else
	//{
	//	for (itr = mTriggerCollisions.begin(); itr != mTriggerCollisions.end(); ++itr)
	//	{
	//		Entity trigger = itr->first;
	//		std::vector<uint32_t>& vec = itr->second;
	//		auto temp = std::find(vec.begin(), vec.end(), static_cast<uint32_t>(e.id));
	//		if (temp == vec.end())
	//			continue;
	//		trigger.GetComponent<Scripts>().RunFunctionForAllScripts("OnTriggerExit", e);
	//		vec.erase(temp);
	//	}
	//}
	if (mActors.count(static_cast<uint32_t>(e.id)) == 0)
		return;
	mPX.mScene->removeActor(*mActors[static_cast<uint32_t>(e.id)].mActor);
	mActors.erase(static_cast<uint32_t>(e.id));
}

std::vector<Entity> PhysicsSystem::Visible(const glm::vec3& origin, const glm::vec3& finalpos, float maxdist)
{
	PxRaycastBuffer hits(hitBuffer, HIT_BUFFER_SIZE);
	bool status = mPX.mScene->raycast(Convert(origin), Convert(glm::normalize(finalpos - origin)), maxdist, hits);

	std::vector<Entity> entitiesHit;

	for (PxU32 i = 0; i < hits.nbTouches; ++i)
		entitiesHit.push_back(*static_cast<uint32_t*>(hits.touches[i].actor->userData));

	return entitiesHit;
}

physx::PxMaterial* PhysicsSystem::CreateMaterial(float us, float ud, float res)
{
	return mPX.mPhysics->createMaterial(us, ud, res);
}

void PhysicsSystem::CreateRigidBody(Entity e)
{

	// retrieve entity info
	Transform xform = e.GetComponent<Transform>();
	RigidBody rbod = e.GetComponent<RigidBody>();
	glm::vec3 childOffset = e.HasParent() ? Entity(e.GetParent()).GetComponent<Transform>().mTranslate : glm::vec3(0);

	PxRigidActor* actor{};
	if (e.GetComponent<General>().name == "Camera")
		CreateActor(actor, PxTransform(Convert(xform.mTranslate + childOffset),
			PxQuat(PxIdentity)), rbod);
	else
		CreateActor(actor, PxTransform(Convert(xform.mTranslate + childOffset), 
			Convert(glm::quat(glm::radians(xform.mRotate)))), rbod);

	if (e.HasComponent<MeshCollider>() && e.HasComponent<MeshRenderer>() && rbod.mMotion == MOTION::STATIC) 
	{
		std::vector<glm::vec3> vtx;
		std::vector<unsigned int> idx;
		PxTriangleMesh* mesh = nullptr;

		std::pair<glm::vec3, glm::vec3> bbox = systemManager->mResourceTySystem->mesh_GetVerticesAndIndices(e.GetComponent<MeshRenderer>().mMeshPath, vtx, idx);
		glm::vec3 diff = bbox.second - bbox.first;
		CreateMeshCollider(vtx, idx, mesh);
		PxMeshScale scale(Convert(xform.mScale / diff), Convert(xform.GetQuaternion()));
		PxTriangleMeshGeometry geom(mesh, scale);
		PxShapeFlags flags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE;
		PxShape* MeshShape = PxRigidActorExt::createExclusiveShape(*actor, geom, *mMaterials[rbod.mMaterial], flags);
		PxFilterData motionFilter(static_cast<uint32_t>(rbod.mMotion), 0, 0, 0);
		MeshShape->setSimulationFilterData(motionFilter);
	}

	if (e.HasComponent<CapsuleCollider>())
	{
		actor->setGlobalPose(PxTransform(Convert(xform.mTranslate + childOffset)));
		PxShape* shape{};
		CapsuleCollider cap = e.GetComponent<CapsuleCollider>();
		CreateAndAttachShape(actor,
			shape, 
			PxCapsuleGeometry(cap.mRadius, cap.mHalfHeight), 
			PxTransform(Convert(cap.mTranslateOffset), PxQuat(PxHalfPi, PxVec3T<float>(0, 0, 1))),
			rbod, 
			cap.mIsTrigger);
	}

	if (e.HasComponent<BoxCollider>())
	{
		PxShape* shape{};
		BoxCollider col = e.GetComponent<BoxCollider>();
		CreateAndAttachShape(actor,
			shape, 
			PxBoxGeometry(Convert(xform.mScale * col.mScaleOffset) / 2.f), 
			PxTransform(Convert(col.mTranslateOffset)),
			rbod, 
			col.mIsTrigger); //PxRigidActorExt::createExclusiveShape();
	}

	if (e.HasComponent<SphereCollider>())
	{
		PxShape* shape{};
		SphereCollider col = e.GetComponent<SphereCollider>();
		CreateAndAttachShape(actor,
			shape, 
			PxSphereGeometry(col.mScaleOffset), 
			PxTransform(Convert(col.mTranslateOffset)),
			rbod, 
			col.mIsTrigger);
	}
	
	if (rbod.mMotion == MOTION::DYNAMIC)
		PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), rbod.mDensity);

	// create actor object
	Actor actorObject;
	actorObject.mEntity = static_cast<uint32_t>(e.id);
	actorObject.mActor = actor;


	// add to scene
	mActors[static_cast<uint32_t>(e.id)] = actorObject;

	// assign user data
	actorObject.mActor->userData = &mActors[static_cast<uint32_t>(e.id)].mEntity;

	mPX.mScene->addActor(*actorObject.mActor);
}

void PhysicsSystem::CreateActor(PxRigidActor*& actor, const PxTransform& pxform, const RigidBody& rbod)
{
	if (rbod.mMotion == MOTION::DYNAMIC)
	{
		actor = mPX.mPhysics->createRigidDynamic(pxform);
		//float temp = rbod.mDensity;
		static_cast<PxRigidDynamic*>(actor)->setLinearVelocity(Convert(rbod.mVelocity));

		PxRigidDynamicLockFlags axis;
		if (rbod.mRotationConstraints.x)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
		if (rbod.mRotationConstraints.y)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
		if (rbod.mRotationConstraints.z)
			axis |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
		static_cast<PxRigidDynamic*>(actor)->setRigidDynamicLockFlags(axis);
		static_cast<PxRigidDynamic*>(actor)->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !rbod.mGravity);
		return;
	}
	actor = mPX.mPhysics->createRigidStatic(pxform);
}

void PhysicsSystem::CreateMeshCollider(const std::vector<glm::vec3>& vtx, const std::vector<unsigned int>& idx, PxTriangleMesh*& triMesh,
	bool skipCleanup, bool skipEdge, const PxU32 triPerLeaf)
{
	PxTriangleMeshDesc desc;
	desc.points.count = vtx.size();
	desc.points.data = vtx.data();
	desc.points.stride = sizeof(glm::vec3);
	desc.triangles.count = idx.size() / 3;
	desc.triangles.data = idx.data();
	desc.triangles.stride = 3 * sizeof(unsigned int);

	PxTolerancesScale scale;
	PxCookingParams params(scale);

	params.midphaseDesc = PxMeshMidPhase::eBVH34;
	SetUpCooking(params, skipCleanup, skipEdge);
	params.midphaseDesc.mBVH34Desc.numPrimsPerLeaf = triPerLeaf;
	PX_ASSERT(PxValidateTriangleMesh(params, desc));
	PxU32 meshSize = 0;
	triMesh = PxCreateTriangleMesh(params, desc, mPX.mPhysics->getPhysicsInsertionCallback());
}

void PhysicsSystem::SetUpCooking(PxCookingParams& params, bool skipMeshCleanup, bool skipEdgeData)
{
	// we suppress the triangle mesh remap table computation to gain some speed, as we will not need it 
	// in this snippet
	params.suppressTriangleMeshRemapTable = true;

	// If DISABLE_CLEAN_MESH is set, the mesh is not cleaned during the cooking. The input mesh must be valid. 
	// The following conditions are true for a valid triangle mesh :
	//  1. There are no duplicate vertices(within specified vertexWeldTolerance.See PxCookingParams::meshWeldTolerance)
	//  2. There are no large triangles(within specified PxTolerancesScale.)
	// It is recommended to run a separate validation check in debug/checked builds, see below.

	if (!skipMeshCleanup)
		params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
	else
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;

	// If eDISABLE_ACTIVE_EDGES_PRECOMPUTE is set, the cooking does not compute the active (convex) edges, and instead 
	// marks all edges as active. This makes cooking faster but can slow down contact generation. This flag may change 
	// the collision behavior, as all edges of the triangle mesh will now be considered active.
	if (!skipEdgeData)
		params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);
	else
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
}

void PhysicsSystem::MoveQueuedEntities()
{
	for (auto e : mPendingAdd)
	{
		if (mActors.find(static_cast<uint32_t>(e.id)) != mActors.end())
		{
			PWARNING("Tried to add actor that is already in simulation!");
			return;
		}
		if (e.HasComponent<RigidBody>())
			CreateRigidBody(e);
		else
			PWARNING("Tried to add actor without rigid body to simulation!");
	}
	for (auto e_pos : mPendingTranslate)
	{
		Entity e = e_pos.first;
		glm::vec3 globalpose = e_pos.second;
		if (!e.HasComponent<RigidBody>()) continue;
		RigidBody rbod = e.GetComponent<RigidBody>();
		if (rbod.mMotion == MOTION::DYNAMIC) {
			PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
			if (actor == nullptr) continue;
			if (e.GetComponent<General>().name == "Camera") {
				actor->setGlobalPose(PxTransform(Convert(globalpose), PxQuat(PxIdentity)));
				continue;
			}
			actor->setGlobalPose(PxTransform(Convert(globalpose), Convert(glm::quat(glm::radians(e.GetComponent<Transform>().mRotate)))));
		}
		else if (rbod.mMotion == MOTION::STATIC) {
			PxRigidStatic* actor = (physx::PxRigidStatic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
			if (actor == nullptr) continue;
			actor->setGlobalPose(PxTransform(Convert(globalpose), Convert(glm::quat(glm::radians(e.GetComponent<Transform>().mRotate)))));
		}
		
	}
	for (auto e_pos : mPendingRotate)
	{
		Entity e = e_pos.first;
		glm::vec3 rotation = e_pos.second;
		if (!e.HasComponent<RigidBody>()) continue;
		RigidBody rbod = e.GetComponent<RigidBody>();
		if (rbod.mMotion == MOTION::DYNAMIC) {
			PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
			if (actor == nullptr) continue;
			actor->setGlobalPose(PxTransform(Convert(e.GetComponent<Transform>().mTranslate), Convert(glm::quat(glm::radians(rotation)))));
		}
		else if (rbod.mMotion == MOTION::STATIC) {
			PxRigidStatic* actor = (physx::PxRigidStatic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
			if (actor == nullptr) continue;
			actor->setGlobalPose(PxTransform(Convert(e.GetComponent<Transform>().mTranslate), Convert(glm::quat(glm::radians(rotation)))));
		}

		
	}
	for (auto e_col : mPendingVelocity)
	{
		Entity e = e_col.first;
		glm::vec3 velocity = e_col.second;
		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)(mActors[static_cast<uint32_t>(e.id)].mActor);
		if (actor == nullptr) continue;
		actor->setAngularDamping(0.5f);
		actor->setLinearVelocity(Convert(velocity));
	}
	mPendingTranslate.clear();
	mPendingRotate.clear();
	mPendingAdd.clear();
	mPendingVelocity.clear();
}

void PhysicsSystem::Synchronize()
{
	for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
	{
		Entity e = itr->first;

		if (e.GetComponent<RigidBody>().mMotion == MOTION::STATIC)
			continue;

		PxRigidActor* actor = itr->second.mActor;
		if (actor == nullptr) continue;

		// retrieving positions in physx
		PxTransform PXform = actor->getGlobalPose();
		Transform& refXform = e.GetComponent<Transform>();

		// overwriting entities with updated positions
		if (e.HasParent())
		{
			Transform parentXform = e.GetParent().GetComponent<Transform>();
			refXform.mTranslate = Convert(PXform.p) - parentXform.mTranslate;
		}
		else
		{
			refXform.mTranslate = Convert(PXform.p);
		}

		RigidBody& rbod = e.GetComponent<RigidBody>();
		rbod.mVelocity = Convert(static_cast<physx::PxRigidDynamic*>(itr->second.mActor)->getLinearVelocity());
		

		if (e.GetComponent<General>().name == "Camera") continue;
		glm::vec3 rotation = glm::eulerAngles(Convert(PXform.q));
		if (!rbod.mRotationConstraints.x)
			refXform.mRotate.x = rotation.x;
		if (!rbod.mRotationConstraints.y)
			refXform.mRotate.y = rotation.y;
		if (!rbod.mRotationConstraints.z)
			refXform.mRotate.z = rotation.z;

		// update velocity
	}
}
