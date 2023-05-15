/*!*****************************************************************************
\file	Force.h
\author Lee Hsien Wei, Joachim
\par	DP email: l.hsienweijoachim@digipen.edu
\par	Course: GAM200
\par	Group: Memory Leak Studios
\date	27-09-2022
\brief  This file contains the declaration of the Force class, its member
		functions and its data members which encapsulates information about
		a force that needs to be acted on an entity
*******************************************************************************/
#pragma once

// -----------------------------
// Include files
// -----------------------------
#include "pch.h"

/*!*****************************************************************************
\brief Force class that encapsulates information about the Force and its
		different types
*******************************************************************************/
class Force {
public:
	/*!*****************************************************************************
	\brief
	Default constructor. Required to nullify errors
	\return void
	NULL
	*******************************************************************************/
	Force() {}

	/*!*****************************************************************************
	\brief
	Default copy constructor. Required to nullify errors. Calls assignment operator
	\param const Force &
	A reference to a read-only force to copy from
	*******************************************************************************/
	Force(const Force& _rhs) {
		*this = _rhs;
	}

	/*!*****************************************************************************
	\brief
	Overloaded constructor. Creates a force that encapsulates a linear force
	\param const Math::Vec2 &
	A reference to a read-only variable containing the direction of the force
	\param const float &
	A reference to a read-only variable containing the magnitude of the force
	\param const double &
	A reference to a read-only variable containing the lifespan of the force.
	Omission of this param assumes a lifespan of 0 which is assumed to be infinite
	lifespan
	\param const double &
	A reference to a read-only variable containing the starting age of the force.
	Omission of this param assumes it starts from 0
	\param const bool &
	A reference to a read-only variable containing the active status of the force.
	Omission of this param assumes it to be active
	*******************************************************************************/
	Force(const Math::Vec2& _unitDirection, const float& _magnitude, const double& _lifetimeLimit = 0.0, const double& _age = 0.0, const bool& _isActive = true) {
		this->lifetimeLimit = _lifetimeLimit;
		this->age = _age;
		this->isActive = _isActive;
		this->forceID = 0;
		this->linearForce.unitDirection = _unitDirection;
		this->linearForce.magnitude = _magnitude;
	}

	/*!*****************************************************************************
	\brief
	Overloaded constructor. Creates a force that encapsulates a rotation force
	\param const float &
	A reference to a read-only variable containing the torque
	\param const double &
	A reference to a read-only variable containing the lifespan of the force.
	Omission of this param assumes a lifespan of 0 which is assumed to be infinite
	lifespan
	\param const double &
	A reference to a read-only variable containing the starting age of the force.
	Omission of this param assumes it starts from 0
	\param const bool &
	A reference to a read-only variable containing the active status of the force.
	Omission of this param assumes it to be active
	*******************************************************************************/
	Force(const float& _torque, const double& _lifetimeLimit = 0.0, const double& _age = 0.0, const bool& _isActive = true) {
		this->lifetimeLimit = _lifetimeLimit;
		this->age = _age;
		this->isActive = _isActive;
		this->forceID = 1;
		this->rotationalForce.torque = _torque;
	}

	/*!*****************************************************************************
	\brief
	Overloaded constructor. Creates a force that encapsulates a rotation force
	\param const float &
	A reference to a read-only variable containing the directional drag force. It has
	a default value of 1
	\param const float &
	A reference to a read-only variable containing the rotational drag force. It has
	a default value of 1
	\param const double &
	A reference to a read-only variable containing the lifespan of the force.
	Omission of this param assumes a lifespan of 0 which is assumed to be infinite
	lifespan
	\param const double &
	A reference to a read-only variable containing the starting age of the force.
	Omission of this param assumes it starts from 0
	\param const bool &
	A reference to a read-only variable containing the active status of the force.
	Omission of this param assumes it to be active
	*******************************************************************************/
	Force(const float& _directionalDrag, const float& _rotationalDrag, const double& _lifetimeLimit = 0.0, const double& _age = 0.0, const bool& _isActive = true) {
		this->lifetimeLimit = _lifetimeLimit;
		this->age = _age;
		this->isActive = _isActive;
		this->forceID = 2;
		this->dragForce.directionalDrag = _directionalDrag;
		this->dragForce.rotationalDrag = _rotationalDrag;
	}

	/*!*****************************************************************************
	\brief
	Default destructor. Required to nullify errors
	*******************************************************************************/
	~Force() {}

	/*!*****************************************************************************
	\brief
	Assignment operator that assigns values to be the same as the given Force
	\param const Force &
	A reference to a read-only force to reference from
	\return Force &
	Returns itself
	*******************************************************************************/
	Force& operator=(const Force& _rhs) {
		// Copy data members
		this->lifetimeLimit = _rhs.lifetimeLimit;
		this->age = _rhs.age;
		this->isActive = _rhs.isActive;
		this->forceID = _rhs.forceID;
		switch (this->forceID) {
		case 0:
			this->linearForce = _rhs.linearForce;
			break;
		case 1:
			this->rotationalForce = _rhs.rotationalForce;
			break;
		case 2:
			this->dragForce = _rhs.dragForce;
			break;
		}

		// Return itself
		return *this;
	}

	/*!*****************************************************************************
	Public Class variables
	*******************************************************************************/
	double lifetimeLimit{ 0.f };	// Maximum lifetime
	double age{ 0.f };			// Current lifespan
	bool isActive{ true };		// Is it an active force
	int forceID;				// Force ID to identify how to access the union
	union {
		// Linear force
		struct LinearForce {
			Math::Vec2 unitDirection{};	// Direction of the force
			float magnitude{};			// Magnitude of the force
		} linearForce;
		// Rotation force
		struct RotationalForce {
			// Math::Vec2 centerOffset	// Position where the force is applied relative to the entity
			float torque{};				// Angular torque of the force
		} rotationalForce;
		// Drag force
		struct DragForce {
			float	directionalDrag{};	// Scalar representing direction(air) drag
			float	rotationalDrag{};	// Scalar representing rotation drag
		} dragForce;
	};
};