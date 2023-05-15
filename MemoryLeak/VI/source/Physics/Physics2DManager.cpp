/*!*****************************************************************************
\file	Physics2DManager.cpp
\author Lee Hsien Wei, Joachim
\par	DP email: l.hsienweijoachim@digipen.edu
\par	Course: GAM200
\par	Group: Memory Leak Studios
\date	27-09-2022
\brief  This file contains the definition of the Physics System Class member
		functions which handles the dynamics of entities stored in its list
		as well as the management of forces acting on the entities
*******************************************************************************/

// -----------------------------
// Include files
// -----------------------------
#include "ECSManager.h"
#include "Input.h"

void Physics2DManager::Update(const double& _appDT) {

	// Input stepping mode check
	if (Input::CheckKey(E_STATE::PRESS, E_KEY::GRAVE_ACCENT) && (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_SHIFT) || Input::CheckKey(E_STATE::HOLD, E_KEY::RIGHT_SHIFT)))
		mStepMode = !mStepMode;

	// Input stepping mode to step
	if (Input::CheckKey(E_STATE::PRESS, E_KEY::GRAVE_ACCENT))	// To shift into else block indicating step mode
		mAdvanceStep = true;


	// Check if system is not in step mode
	if (!mStepMode) {
		// Increment accumulatedDT by the application's DT
		mAccumulatedDT += _appDT;

		// Prevent spiral of death
		if (mAccumulatedDT > mAccumulatedDTCap)
			mAccumulatedDT = mAccumulatedDTCap;

		// If the accumlatedDT is larger than or equal to the defined fixedDT,
		//	Execute a simulation tick of the physics using the defined fixedDT and subtract that value from accumulatedDT 
		while (mAccumulatedDT >= mFixedDT) {
			Step(mFixedDT);
			mAccumulatedDT -= mFixedDT;
		}
	}
	// In step mode
	else {
		// Reset accumulatedDT for next time we are not in step mode
		mAccumulatedDT = 0.0;	// To shift into if condition checking for step mode toggle
		// Check if we should step (key pressed)
		if (mAdvanceStep) {
			// Execute a simulation tick of physics using defined fixedDT
			Step(mFixedDT);
			// Set advance flag to false;
			mAdvanceStep = false;
		}
	}
}

void Physics2DManager::Step(const double& _stepDT) {

	// Update all required entities physics based on object rotation/orientation
	for (const Entity& e : mEntities) {

		if (mFirstUpdate)
			ApplyImpulse(e, Math::Vec2{ 0.f, 1.f }, Math::Vec2{ 0.f, 0.f });

		// Skip if entity should not be run
		if (!e.ShouldRun())
			continue;
		//if (!e.GetComponent<General>().isActive)
			//continue;

		// Skip if entity does not have physics component
		if (!e.HasComponent<Physics2D>())
			continue;

		// Skip if entity is a static object
		if (!GetDynamicsEnabled(e))
			continue;

		// Skip infinite mass objects
		if (GetMass(e) == 0.f)
			continue;

		// Update accumulated forces acting on entity
		UpdateEntitiesAccumulatedForce(e);
		
		// Determine acceleration
		SetAcceleration(e, GetAccumulatedForce(e) * (GetMass(e) == 0.f ? 0.f : static_cast<float>(1.f / GetMass(e))));
		
		// Determine velocity
		SetVelocity(e, GetVelocity(e) + GetAcceleration(e) * static_cast<float>(_stepDT));
		SetAngularVelocity(e, GetAngularVelocity(e) + GetAngularTorque(e) * (GetInertia(e) == 0.f ? 0.f : (1.f / GetInertia(e))) * static_cast<float>(_stepDT));

		if (fabs(GetVelocity(e).x) < FLT_EPSILON && fabs(GetVelocity(e).y) < FLT_EPSILON) {
			// Do nothing
		}
		else {
			// Dampen velocity (for soft drag)
			ScaleVelocity(e, GetDamping(e));
			SetAngularVelocity(e, GetAngularVelocity(e) * static_cast<float>(std::pow(GetDamping(e), _stepDT)));
			SetAngularTorque(e, GetAngularTorque(e) * static_cast<float>(std::pow(GetDamping(e), _stepDT)));

			// Cap simulation velocity
			if (Math::Dot(GetVelocity(e), GetVelocity(e)) > Physics2DManager::mVelocityCap * Physics2DManager::mVelocityCap)
				SetVelocity(e, GetVelocity(e).Normalize() * Physics2DManager::mVelocityCap);

			if (GetAngularVelocity(e) > Physics2DManager::mAngularVelocityCap)
				SetAngularVelocity(e, Physics2DManager::mAngularVelocityCap);

			// Move entity by velocity
			e.GetComponent<Transform>().translation += GetVelocity(e) * static_cast<float>(_stepDT);
			e.GetComponent<Transform>().rotation += static_cast<float>(GetAngularVelocity(e) * _stepDT);

			// Check if object exits the quadtree bounds
			//if (!collision2DManager->GetQuadTree().GetBox().Contains(QuadBox(e))) {
			//	// Resize QuadTree dimensions

			//	// Reinsert all entities

			//}
			
			// Update entity in quad tree
			//collision2DManager->UpdateEntityInQuadTree(e);
		}

		// Clamp rotation
		if (e.GetComponent<Transform>().rotation > static_cast<float>((2.0 * Math::PI)))
			e.GetComponent<Transform>().rotation -= static_cast<float>((2.0 * Math::PI));
		else if (e.GetComponent<Transform>().rotation < static_cast<float>(-(2.0 * Math::PI)))
			e.GetComponent<Transform>().rotation += static_cast<float>((2.0 * Math::PI));

		// Reset forces on the object for next step
		SetAccumulatedForce(e, Math::Vec2{ 0.f, 0.f });
	}

	// Hack
	if (mFirstUpdate)
		mFirstUpdate = false;
}

void Physics2DManager::RemovePhysicsComponent(const Entity& _e) {
	// Remove component if component exists
	if (_e.HasComponent<Physics2D>())
		_e.RemoveComponent<Physics2D>();
}

Physics2D& Physics2DManager::GetPhysicsComponent(const Entity& _e) {
	return _e.GetComponent<Physics2D>();
}

bool Physics2DManager::GetDynamicsEnabled(const Entity& _e) {
	return GetPhysicsComponent(_e).dynamicsEnabled;
}

void Physics2DManager::SetDynamicsEnabled(const Entity& _e, const bool& _dynamicsEnabled) {
	GetPhysicsComponent(_e).dynamicsEnabled = _dynamicsEnabled;
}

float Physics2DManager::GetMass(const Entity& _e) {
	return GetPhysicsComponent(_e).mass;
}

void Physics2DManager::SetMass(const Entity& _e, const float& _mass) {
	GetPhysicsComponent(_e).mass = _mass;
}

float Physics2DManager::GetInertia(const Entity& _e) {
	return GetPhysicsComponent(_e).inertia;
}

void Physics2DManager::SetInertia(const Entity& _e, const float& _inertia) {
	GetPhysicsComponent(_e).inertia = _inertia;
}

float Physics2DManager::GetRestitution(const Entity& _e) {
	return GetPhysicsComponent(_e).restitution;
}

void Physics2DManager::SetRestitution(const Entity& _e, const float& _restitution) {
	GetPhysicsComponent(_e).restitution = _restitution;
}

float Physics2DManager::GetFriction(const Entity& _e) {
	return GetPhysicsComponent(_e).friction;
}

void Physics2DManager::SetFriction(const Entity& _e, const float& _friction) {
	GetPhysicsComponent(_e).friction = _friction;
}

float Physics2DManager::GetDamping(const Entity& _e) {
	return GetPhysicsComponent(_e).damping;
}

void Physics2DManager::SetDamping(const Entity& _e, const float& _damping) {
	GetPhysicsComponent(_e).damping = _damping;
}

Math::Vec2 Physics2DManager::GetAccumulatedForce(const Entity& _e) {
	return GetPhysicsComponent(_e).accumulatedForce;
}

void Physics2DManager::SetAccumulatedForce(const Entity& _e, const Math::Vec2& _accumulatedForce) {
	GetPhysicsComponent(_e).accumulatedForce = _accumulatedForce;
}

Math::Vec2 Physics2DManager::GetVelocity(const Entity& _e) {
	return GetPhysicsComponent(_e).velocity;
}

void Physics2DManager::SetVelocity(const Entity& _e, const Math::Vec2& _velocity) {
	GetPhysicsComponent(_e).velocity = _velocity;
}

void Physics2DManager::AddVelocity(const Entity& _e, const Math::Vec2& _velocity) {
	GetPhysicsComponent(_e).velocity += _velocity;
}

void Physics2DManager::ScaleVelocity(const Entity& _e, const float& _scalar) {
	GetPhysicsComponent(_e).velocity *= _scalar;
}

Math::Vec2 Physics2DManager::GetAcceleration(const Entity& _e) {
	return GetPhysicsComponent(_e).acceleration;
}

void Physics2DManager::SetAcceleration(const Entity& _e, const Math::Vec2& _acceleration) {
	GetPhysicsComponent(_e).acceleration = _acceleration;
}

float Physics2DManager::GetAngularVelocity(const Entity& _e) {
	return GetPhysicsComponent(_e).angularVelocity;
}

void Physics2DManager::SetAngularVelocity(const Entity& _e, const float& _angVel) {
	GetPhysicsComponent(_e).angularVelocity = _angVel;
}

float Physics2DManager::GetAngularTorque(const Entity& _e) {
	return GetPhysicsComponent(_e).angularTorque;
}

void Physics2DManager::SetAngularTorque(const Entity& _e, const float& _angTorque) {
	GetPhysicsComponent(_e).angularTorque = _angTorque;
}

bool Physics2DManager::GetPhysicsRenderFlag(const Entity& _e) {
	return GetPhysicsComponent(_e).renderFlag;
}

void Physics2DManager::SetPhysicsRenderFlag(const Entity& _e, const bool& _renderFlag) {
	GetPhysicsComponent(_e).renderFlag = _renderFlag;
}

void Physics2DManager::UpdateEntitiesAccumulatedForce(const Entity& _e) {
	for (auto it{ GetPhysicsComponent(_e).forceList.begin() }; it != GetPhysicsComponent(_e).forceList.end();) {
		if (it->age < it->lifetimeLimit)
			it->age += Physics2DManager::mFixedDT;
		else if (it->lifetimeLimit == 0.0) {
			// Assume infinite lifespan
			// Do nothing
		}
		else
			it->isActive = false;

		if (it->isActive) {
			switch (it->forceID) {
				// Linear
			case 0:
				SetAccumulatedForce(_e, GetAccumulatedForce(_e) + (it->linearForce.unitDirection * it->linearForce.magnitude));
				break;
				// Rotational
			case 1:
				SetAngularTorque(_e, GetAngularTorque(_e) + static_cast<float>(it->rotationalForce.torque));
				break;
				// Drag
			case 2:
				SetAccumulatedForce(_e, GetAccumulatedForce(_e) * it->dragForce.directionalDrag);
				SetAngularTorque(_e, GetAngularTorque(_e) * static_cast<float>(it->dragForce.rotationalDrag));
				break;
			}

			++it;
		}
		else {
			it = GetPhysicsComponent(_e).forceList.erase(it);
		}
	}
}

void Physics2DManager::AddLinearForceToList(const Entity& _e, const Math::Vec2& _unitDirection, const float& _magnitude,
	const double& _lifetimeLimit, const double& _age, const bool& _isActive) {
	Force tmpForce{};
	tmpForce.lifetimeLimit = _lifetimeLimit;
	tmpForce.age = _age;
	tmpForce.isActive = _isActive;
	tmpForce.forceID = 0;
	tmpForce.linearForce.unitDirection = _unitDirection;
	tmpForce.linearForce.magnitude = _magnitude;

	GetPhysicsComponent(_e).forceList.push_back(tmpForce);
}

void Physics2DManager::AddRotationForceToList(const Entity& _e, const float& _torque,
	const double& _lifetimeLimit, const double& _age, const bool& _isActive) {
	Force tmpForce{};
	tmpForce.lifetimeLimit = _lifetimeLimit;
	tmpForce.age = _age;
	tmpForce.isActive = _isActive;
	tmpForce.forceID = 1;
	tmpForce.rotationalForce.torque = _torque;

	GetPhysicsComponent(_e).forceList.push_back(tmpForce);
}
void Physics2DManager::AddDragForceToList(const Entity& _e, const float& _directionDrag, const float& _rotationDrag,
	const double& _lifetimeLimit, const double& _age, const bool& _isActive) {
	Force tmpForce{};
	tmpForce.lifetimeLimit = _lifetimeLimit;
	tmpForce.age = _age;
	tmpForce.isActive = _isActive;
	tmpForce.forceID = 2;
	tmpForce.dragForce.directionalDrag = _directionDrag;
	tmpForce.dragForce.rotationalDrag = _rotationDrag;

	GetPhysicsComponent(_e).forceList.push_back(tmpForce);
}

void Physics2DManager::ApplyImpulse(const Entity& _e, const Math::Vec2& _impulse, const Math::Vec2& _rotation) {
	if (!_e.HasComponent<Physics2D>())
		return;

	if (_e.ShouldRun()) {
		SetVelocity(_e, GetVelocity(_e) + (GetMass(_e) == 0.f ? 0.f : (1.f / GetMass(_e))) * _impulse);
		SetAngularVelocity(_e, GetAngularVelocity(_e) + (GetInertia(_e) == 0.f ? 0.f : (1.f / GetInertia(_e))) * Math::Cross(_impulse, _rotation));
	}
}