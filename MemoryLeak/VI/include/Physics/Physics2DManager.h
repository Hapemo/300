/*!*****************************************************************************
\file	Physics2DManager.cpp
\author Lee Hsien Wei, Joachim
\par	DP email: l.hsienweijoachim@digipen.edu
\par	Course: GAM200
\par	Group: Memory Leak Studios
\date	27-09-2022
\brief  This file contains the declaration of the Physics System Class and its
		member functions which handles the dynamics of entities stored in its list
		as well as the management of forces acting on the entities
*******************************************************************************/
#pragma once

//#define MultiThread 1 // 1 to multi thread, 0 to not multi thread.

// -----------------------------
// Include files
// -----------------------------
#include "ECS_systems.h"

/*!*****************************************************************************
\brief Physics system class that handles the updating of object's accumulated
	   forces & positions through the use of forces.
*******************************************************************************/
class Physics2DManager : public System {
public:
// -----------------------------
// System functions
// -----------------------------
	/*!*****************************************************************************
	\brief
	Update function that simulates physics by stepping it in fixedDT when enough
	time has passed
	\param const float &
	A reference to a read-only variable that tells us the application's current
	delta time
	\return void
	NULL
	*******************************************************************************/
	void Update(const double& _appDT);

	/*!*****************************************************************************
	\brief
	Step function that executes fixed delta time physics stepping
	\param void
	NULL
	\return void
	NULL
	*******************************************************************************/
	void Step(const double& _stepDT);

// -----------------------------
// Component-related functions
// -----------------------------
	/*!*****************************************************************************
	\brief
	Ignore for now
	\return void
	NULL
	*******************************************************************************/
	//void AddPhysicsComponent();

	/*!*****************************************************************************
	\brief
	RemovePhysicsComponent function that removes the physics component from the
	entity itself if it exists
	\param const Entity &
	A reference to a read-only Entity to remove the physics component from
	\return void
	NULL
	*******************************************************************************/
	void RemovePhysicsComponent(const Entity& _e);

	/*!*****************************************************************************
	\brief
	GetPhysicsComponent function that getsand returns the physics component of the
	given entity
	\param const Entity &
	A reference to a read-only Entity to get from
	\return Physics2D &
	A reference to the Physics2D component in the given entity
	*******************************************************************************/
	Physics2D& GetPhysicsComponent(const Entity& _e);

// -----------------------------
// Get / Set functions
// -----------------------------
	/*!*****************************************************************************
	\brief
	GetDynamicsEnabled function that returns the stored value of the entity's
	dynamics enabled flag
	\param const Entity &
	A reference to a read-only Entity to
	\return bool
	The value of the entity's dynamics enabled flag
	*******************************************************************************/
	bool GetDynamicsEnabled(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetDynamicsEnabled function that sets the stored value of the entity's
	dynamics enabled flag to the given value
	\param const Entity &
	A reference to a read-only Entity to
	\param const bool &
	A reference to a read-only value containing value to set
	\return void
	NULL
	*******************************************************************************/
	void SetDynamicsEnabled(const Entity& _e, const bool& _dynamicsEnabled);

	/*!*****************************************************************************
	\brief
	GetMass function that returns the stored value of the entity's mass
	\param const Entity &
	A reference to a read-only Entity to get from
	\return float
	A copy of the value of the entity's mass
	*******************************************************************************/
	float GetMass(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetMass function that sets the stored value of the entity's mass to the given
	value. It also updates the stored value of the entity's inverse mass
	\param const Entity &
	A reference to a read-only Entity to set
	\param const float &
	A reference to a read-only value containing the mass to set to
	\return void
	NULL
	*******************************************************************************/
	void SetMass(const Entity& _e, const float& _mass);

	/*!*****************************************************************************
	\brief
	GetInertia function that returns the stored value of the entity's inertia
	\param const Entity &
	A reference to a read-only Entity to get from
	\return float
	A copy of the value of the entity's inertia
	*******************************************************************************/
	float GetInertia(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetInertia function that sets the stored value of the entity's inertia to the
	given value. It also updates the stored value of the entity's inverse inertia
	\param const Entity &
	A reference to a read-only Entity to set
	\param const float &
	A reference to a read-only value containing the inertia to set to
	\return void
	NULL
	*******************************************************************************/
	void SetInertia(const Entity& _e, const float& _inertia);

	/*!*****************************************************************************
	\brief
	GetRestitution function that returns the stored value of the entity's restitution
	\param const Entity &
	A reference to a read-only Entity to get from
	\return float
	A copy of the value of the entity's restitution
	*******************************************************************************/
	float GetRestitution(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetRestitution function that sets the stored value of the entity's restitution
	to the given value
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const float &
	A reference to a read-only value containing the inverse inertia to set to
	\return void
	NULL
	*******************************************************************************/
	void SetRestitution(const Entity& _e, const float& _restitution);

	/*!*****************************************************************************
	\brief
	GetFriction function that returns the stored value of the entity's friction
	\param const Entity &
	A reference to a read-only Entity to get from
	\return float
	A copy of the value of the entity's friction
	*******************************************************************************/
	float GetFriction(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetFriction function that sets the stored value of the entity's friction
	to the given value
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const float &
	A reference to a read-only value containing the friction to set to
	\return void
	NULL
	*******************************************************************************/
	void SetFriction(const Entity& _e, const float& _friction);

	/*!*****************************************************************************
	\brief
	GetDamping function that returns the stored value of the entity's damping
	\param const Entity &
	A reference to a read-only Entity to get from
	\return float
	A copy of the value of the entity's damping
	*******************************************************************************/
	float GetDamping(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetDamping function that sets the stored value of the entity's damping to the
	given value
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const float &
	A reference to a read-only value containing the damping to set to
	\return void
	NULL
	*******************************************************************************/
	void SetDamping(const Entity& _e, const float& _damping);

	//float GetSpeed(const Entity& _e);
	//void SetSpeed(const Entity& _e, const float& _speed);

	//float GetMoveDirection(const Entity& _e);
	//void SetMoveDirection(const Entity& _e, const float& _moveDirection);

	/*!*****************************************************************************
	\brief
	GetAccumulatedForce function that returns the stored value of the entity's
	final force
	\param const Entity &
	A reference to a read-only Entity to get from
	\return Math::Vec2
	A copy of the value of the entity's final force
	*******************************************************************************/
	Math::Vec2 GetAccumulatedForce(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetAccumulatedForce function that sets the stored value of the entity's
	accumulated force to the given value
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const Math::Vec2 &
	A reference to a read-only value containing the force to set to
	\return void
	NULL
	*******************************************************************************/
	void SetAccumulatedForce(const Entity& _e, const Math::Vec2& _accumulatedForce);

	/*!*****************************************************************************
	\brief
	GetVelocity function that returns the stored value of the entity's velocity
	\param const Entity &
	A reference to a read-only Entity to get from
	\return Math::Vec2
	A copy of the value of the entity's velocity
	*******************************************************************************/
	Math::Vec2 GetVelocity(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetVelocity function that sets the stored value of the entity's velocity
	to the given value
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const Math::Vec2 &
	A reference to a read-only value containing the friction to set to
	\return void
	NULL
	*******************************************************************************/
	void SetVelocity(const Entity& _e, const Math::Vec2& _velocity);
	/*!*****************************************************************************
	\brief
	AddVelocity function that adds a given vector containing a velocity vector to add
	to he stored value of the entity's velocity
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const Math::Vec2 &
	A reference to a read-only value containing the velocity vector to add
	\return void
	NULL
	*******************************************************************************/
	void AddVelocity(const Entity& _e, const Math::Vec2& _velocity);
	/*!*****************************************************************************
	\brief
	ScaleVelocity function that scales the entity's velocity by the given scalar
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const float &
	A reference to a read-only value containing the scalar value
	\return void
	NULL
	*******************************************************************************/
	void ScaleVelocity(const Entity& _e, const float& _scalar);

	/*!*****************************************************************************
	\brief
	GetAcceleration function that returns the stored value of the entity's acceleration
	\param const Entity &
	A reference to a read-only Entity to get from
	\return Math::Vec2
	A copy of the value of the entity's acceleration
	*******************************************************************************/
	Math::Vec2 GetAcceleration(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetAcceleration function that sets the stored value of the entity's velocity
	to the given value
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const Math::Vec2 &
	A reference to a read-only value containing the friction to set to
	\return void
	NULL
	*******************************************************************************/
	void SetAcceleration(const Entity& _e, const Math::Vec2& _acceleration);

	/*!*****************************************************************************
	\brief
	GetAngularVelocity function that returns the stored value of the entity's angular
	velocity
	\param const Entity &
	A reference to a read-only Entity to get from
	\return float
	A copy of the value of the entity's angular velocity
	*******************************************************************************/
	float GetAngularVelocity(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetAngularVelocity function that sets the stored value of the entity's angular
	velocity to the given value. The value is assumed to be in radians
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const float &
	A reference to a read-only value containing the angular velocity to set to
	\return void
	NULL
	*******************************************************************************/
	void SetAngularVelocity(const Entity& _e, const float& _angVel);

	/*!*****************************************************************************
	\brief
	GetAngularTorque function that returns the stored value of the entity's angular
	acceleration
	\param const Entity &
	A reference to a read-only Entity to get from
	\return Math::Vec2
	A copy of the value of the entity's angular acceleration
	*******************************************************************************/
	float GetAngularTorque(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetAngularTorque function that sets the stored value of the entity's angular
	torque to the given value. The value is assumed to be in radians
	\param const Entity &
	A reference to a read-only Entity to get from
	\param const float &
	A reference to a read-only value containing the angular velocity to set to
	\return void
	NULL
	*******************************************************************************/
	void SetAngularTorque(const Entity& _e, const float& _angTorque);

	/*!*****************************************************************************
	\brief
	GetPhysicsRenderFlag function that returns the stored value of the entity's
	physics render flag
	\param const Entity &
	A reference to a read-only Entity to
	\return bool
	The value of the entity's physics render flag
	*******************************************************************************/
	bool GetPhysicsRenderFlag(const Entity& _e);
	/*!*****************************************************************************
	\brief
	SetPhysicsRenderFlag function that sets the stored value of the entity's
	physics render flag to the given value
	\param const Entity &
	A reference to a read-only Entity to
	\param const bool &
	A reference to a read-only value containing value to set
	\return void
	NULL
	*******************************************************************************/
	void SetPhysicsRenderFlag(const Entity& _e, const bool& _renderFlag);

	/*!*****************************************************************************
	\brief
	UpdateEntitiesAccumulatedForce function that updates the entity's final acting
	force based on the list of forces acting on the entity at frame time. It also
	deactivates and removes forces that have aged/expired
	\param const Entity &
	A reference to a read-only Entity to
	\return void
	NULL
	*******************************************************************************/
	void UpdateEntitiesAccumulatedForce(const Entity& _e);

	/*!*****************************************************************************
	\brief
	AddLinearForceToList function that adds a linear force to an entity's list of forces
	to act on it
	\param const Entity &
	A reference to a read-only Entity to
	\param const Math::Vec2 &
	A reference to a read-only variable containing the direction of the force
	\param const float &
	A reference to a read-only variable containing the magnitude of the force
	\param const float &
	A reference to a read-only variable containing the lifespan of the force.
	Omission of this param assumes a lifespan of 0 which is assumed to be infinite
	lifespan
	\param const float &
	A reference to a read-only variable containing the starting age of the force.
	Omission of this param assumes it starts from 0
	\param const bool &
	A reference to a read-only variable containing the active status of the force.
	Omission of this param assumes it to be active
	\return void
	NULL
	*******************************************************************************/
	void AddLinearForceToList(const Entity& _e, const Math::Vec2& _unitDirection, const float& _magnitude,
							  const double& _lifetimeLimit = 0.0, const double& _age = 0.0, const bool& _isActive = true);
	/*!*****************************************************************************
	\brief
	AddRotationForceToList function that adds a rotation force to an entity's list of forces
	to act on it
	\param const Entity &
	A reference to a read-only Entity to
	\param const float &
	A reference to a read-only variable containing the torque
	\param const float &
	A reference to a read-only variable containing the lifespan of the force.
	Omission of this param assumes a lifespan of 0 which is assumed to be infinite
	lifespan
	\param const float &
	A reference to a read-only variable containing the starting age of the force.
	Omission of this param assumes it starts from 0
	\param const bool &
	A reference to a read-only variable containing the active status of the force.
	Omission of this param assumes it to be active
	\return void
	NULL
	*******************************************************************************/
	void AddRotationForceToList(const Entity& _e, const float& _torque,
								const double& _lifetimeLimit = 0.0, const double& _age = 0.0, const bool& _isActive = true);
	/*!*****************************************************************************
	\brief
	AddDragForceToList function that adds a drag/resistance force to an entity's list of forces
	to act on it
	\param const Entity &
	A reference to a read-only Entity to
	\param const float &
	A reference to a read-only variable containing the directional drag force. It has
	a default value of 1
	\param const float &
	A reference to a read-only variable containing the rotational drag force. It has
	a default value of 1
	\param const float &
	A reference to a read-only variable containing the lifespan of the force.
	Omission of this param assumes a lifespan of 0 which is assumed to be infinite
	lifespan
	\param const float &
	A reference to a read-only variable containing the starting age of the force.
	Omission of this param assumes it starts from 0
	\param const bool &
	A reference to a read-only variable containing the active status of the force.
	Omission of this param assumes it to be active
	\return void
	NULL
	*******************************************************************************/
	void AddDragForceToList(const Entity& _e, const float& _directionDrag = 1.f, const float& _rotationDrag = 1.f,
							const double& _lifetimeLimit = 0.0, const double& _age = 0.0, const bool& _isActive = true);
	//void ApplyLinearForceToAccumulated(const Entity& _e, const Math::Vec2& _unitDirection, const float& _magnitude);
	//void ApplyRotationForceToAccumulated(const Entity& _e, const float& _torque);
	//void ApplyDragForceToAccumulated(const Entity& _e, const float& _directionDrag, const float& _rotationDrag);


	/*!*****************************************************************************
	\brief
	ApplyImpulse function that adds a velocity impulse to the entity
	\param const Entity &
	A reference to a read-only Entity to
	\param const Math::Vec2 &
	A reference to a read-only variable containing the velocity impulse
	\param const Math::Vec2 &
	A reference to a read-only variable containing the rotation impulse
	calculation
	\return void
	NULL
	*******************************************************************************/
	void ApplyImpulse(const Entity& _e, const Math::Vec2& _impulse, const Math::Vec2& _rotation);
//	std::mutex mPhysicsStepLock;	// Variable for testing multi-threading
private:
	// -----------------------------
	// Constant values
	// -----------------------------
	double mAccumulatedDT{ 0.0 };					// Member variable storing accumulatedDT
	const double mFixedDT{ 1.0 / 60.0 };			// Fixed delta time step of 1/60 steps a second
	const double mAccumulatedDTCap{ 1.0 };			// Accumulated cannot store more than 1 second worth of updates
	const float  mVelocityCap{ 1000.f };			// Global velocity cap
	const float	 mAngularVelocityCap{ 30.f };		// Global angular velocity cap
	bool mStepMode{ false };						// Flag variable containing whether physics update is in step mode
	bool mAdvanceStep{ false };						// Flag variable containing whether physics should step when its in step mode
	bool mFirstUpdate{ true };						// First Update flag for hacking vector direction
};
