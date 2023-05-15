#pragma once

/*!*****************************************************************************
\brief Contact class that encapsulates information about a collision
*******************************************************************************/
class Contact {
public:
	/*!*****************************************************************************
	\brief
	DetermineRestitution function computes the restitution value between the two
	entities
	\param void
	NULL
	\return float
	The computed restitution
	*******************************************************************************/
	float DetermineRestitution() {
		if (!obj[0].HasComponent<Physics2D>() || !obj[1].HasComponent<Physics2D>()) {
			return obj[0].HasComponent<Physics2D>() ? obj[0].GetComponent<Physics2D>().restitution : obj[1].HasComponent<Physics2D>() ? obj[1].GetComponent<Physics2D>().restitution : 0.f;
		}

		return std::min(obj[0].GetComponent<Physics2D>().restitution, obj[1].GetComponent<Physics2D>().restitution);
	}

	/*!*****************************************************************************
	\brief
	DetermineFriction function computes the friction value between the two
	entities
	\param void
	NULL
	\return float
	The computed friction
	*******************************************************************************/
	float DetermineFriction() {
		if (!obj[0].HasComponent<Physics2D>() || !obj[1].HasComponent<Physics2D>()) {
			return obj[0].HasComponent<Physics2D>() ? std::sqrtf(obj[0].GetComponent<Physics2D>().friction) : obj[1].HasComponent<Physics2D>() ? std::sqrtf(obj[1].GetComponent<Physics2D>().friction) : 0.f;
		}

		return std::sqrtf(obj[0].GetComponent<Physics2D>().friction * obj[1].GetComponent<Physics2D>().friction);
	}

	/*!*****************************************************************************
	\brief
	DetermineSeperatingVelocity function computes the seperating velocity value
	between the two entities
	\param void
	NULL
	\return float
	The computed friction
	*******************************************************************************/
	float DetermineSeperatingVelocity() {
		Math::Vec2 relVel{};
		if (obj[0].HasComponent<Physics2D>() && obj[1].HasComponent<Physics2D>())
			relVel = obj[0].GetComponent<Physics2D>().velocity - obj[1].GetComponent<Physics2D>().velocity;
		else if (obj[0].HasComponent<Physics2D>())
			relVel = obj[0].GetComponent<Physics2D>().velocity;
		else if (obj[1].HasComponent<Physics2D>())
			relVel = obj[1].GetComponent<Physics2D>().velocity;

		return Math::Dot(relVel, normal);
	}

	/*!*****************************************************************************
	\brief
	Default constructor. Initializes its data members
	\param const Entity &
	A reference to a read-only entity
	\param const Entity &
	A reference to a read-only entity
	\param const int &
	A reference to a read-only value containing the collider type of the first entity
	\param const int &
	A reference to a read-only value containing the collider type of the second entity

	*******************************************************************************/
	Contact(const Entity& _obj1, const Entity& _obj2, const int& _obj1Type, const int& _obj2Type) {
		obj[0] = _obj1;
		obj[1] = _obj2;
		objType[0] = _obj1Type;
		objType[1] = _obj2Type;

		combinedRestitution = DetermineRestitution();
		combinedFriction = DetermineFriction();
		seperatingVelocity = DetermineSeperatingVelocity();
	}


	/*!*****************************************************************************
	Public Class variables
	*******************************************************************************/
	Entity obj[2]{};				// Array of entity
	int objType[2]{};				// Array of entity objType
	//double interTime{};				// Collision time
	Math::Vec2 normal{};			// Collision normal
	Math::Vec2 contacts{};			// Points of contact of the collision
	float penetration{};			// Collision penetration depths
	float combinedRestitution{};	// Combined restitution value
	float combinedFriction{};		// Combined friction value
	float seperatingVelocity{};		// Computed seperating velocity scalar value
	float contactImpulse{};			// Contact impulse
};