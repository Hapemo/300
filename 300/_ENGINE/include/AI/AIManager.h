#pragma once
#include "pch.h"
#include "glm/glm.hpp"

// TODO
// - How to set up target
// - How to add ground entities and flying entities to container to keep track
// 
// 
// Testing needed
// - Test the spreadout
// 



struct Entity;

// All the label for names of container of AIs
#define GROUND_ENEMY "GroundEnemy"
#define AIR_ENEMY "AirEnemy"

enum class E_MOVEMENT_TYPE : char {
	GROUND,
	AIR
};

enum class E_ATTACK_TYPE : char {
	CONTACT,
	SHOOTING
};

class AIManager {
public:
	AIManager();

	/*!*****************************************************************************
	Get the direction of entity which movement is dictated by AI.

	\param _e
	- Entity to find direction of

	\return glm::vec3
	- Direction of movement dictated by AI
	*******************************************************************************/
	glm::vec3 GetDirection(Entity _e);

	/*!*****************************************************************************
	Get the next position the AI will be in. Moving with reference to target.

	\param _e
	- Entity to find direction of

	\return glm::vec3
	- Next position of the entity dictated by AI
	*******************************************************************************/
	glm::vec3 GetNextPosition(Entity _e);

private:

	/*!*****************************************************************************
	Calculate the ground AI next direction

	\param _e
	- Entity

	\return glm::vec3
	- Direction
	*******************************************************************************/
	glm::vec3 CalcGroundAIDir(Entity _e);

	/*!*****************************************************************************
	Change the input direction of an AI with respect to other AIs from it's 
	catagory. Degree of spread is determined by mSpreadOut ratio in AISetting.

	\param _e
	- Entity to adjust

	\param &dir
	- Direction of AI to change (Reference)
	*******************************************************************************/
	void SpreadOut(Entity _e, glm::vec3& dir);

private:
	
	std::unordered_map<std::string, std::set<Entity>> mAILists; // Contains the different list of AIs of different classification. string is the name of container





};










