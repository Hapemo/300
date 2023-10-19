#pragma once
#include "pch.h"
#include "glm/glm.hpp"

struct Entity;

enum class E_MOVEMENT_TYPE {
	GROUND,
	AIR
};

enum class E_ATTACK_TYPE {
	CONTACT,
	SHOOTING
};

struct EnemySetting {
	E_MOVEMENT_TYPE mMovementType;	// Enemy's movement type
	E_ATTACK_TYPE mAttackType;			// Enemy's attack type
	float mSpreadOut;								// Degree of spreading out
};

class AIManager {
public:
	/*!*****************************************************************************
	Get the direction of entity which movement is dictated by AI.

	\param _e
	- Entity to find direction of

	\return glm::vec3
	- Direction of movement dictated by AI
	*******************************************************************************/
	glm::vec3 GetDirection(Entity _e);

	/*!*****************************************************************************
	Get the next position the AI will be in.

	\param _e
	- Entity to find direction of

	\return glm::vec3
	- Next position of the entity dictated by AI
	*******************************************************************************/
	glm::vec3 GetNextPosition(Entity _e);

private:
	






};










