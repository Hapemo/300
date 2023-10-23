#pragma once
#include "pch.h"
#include "glm/glm.hpp"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
// TODO
// - How to set up target
// - How to add ground entities and flying entities to container to keep track
// 
// 
// Testing needed
// - Test the spreadout
// 

// All the label for names of container of AIs
#define GROUND_ENEMY "GroundEnemy"
#define AIR_ENEMY "AirEnemy"
#define MAX_DECISECOND_PLAYER_HISTORY 30

enum class E_MOVEMENT_TYPE : char {
	GROUND_DIRECT = 1,		// Directly moving to target on ground
	AIR_HOVER							// Hover around the target
};

//enum class E_ATTACK_TYPE : char {
//	CONTACT = 1,
//	SHOOTING
//};

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
	Takes the position of player every 0.1s and keep a history of it. Will not run
	if mPlayer is entt::null
	* This function has to be ran every loop. *

	\param _e
	- Entity to find direction of

	\return glm::vec3
	- Next position of the entity dictated by AI
	*******************************************************************************/
	void TrackPlayerPosition();

	/*!*****************************************************************************
	Set the player entity in AIManager

	\param _e
	- Player Entity
	*******************************************************************************/
	void SetPlayer(Entity _e);

	/*!*****************************************************************************
	Resets the player pointer in AIManager to null. 
	* Must be called when player entity gets dereferenced. *
	*******************************************************************************/
	void ResetPlayerTracking();

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
	Calculate the ground AI next direction

	\param _e
	- Entity

	\return glm::vec3
	- Direction
	*******************************************************************************/
	glm::vec3 CalcAirAIDir(Entity _e);

	/*!*****************************************************************************
	Change the input direction of an AI with respect to other AIs from it's 
	catagory. Degree of spread is determined by mSpreadOut ratio in AISetting.

	\param _e
	- Entity to adjust

	\param &dir
	- Direction of AI to change (Reference)
	*******************************************************************************/
	void SpreadOut(Entity _e, glm::vec3& dir);

	/*!*****************************************************************************
	Predict target's final position when the projectile is launched, based on
	target's current position and velocity. Automatically update projectile's
	velocity

	\param projectile
	- Entity id of projectile

	\param target
	- Entity id of target

	\param speed
	- Speed of projectile
	*******************************************************************************/
	void SetPredictiveVelocity(Entity projectile, Entity target, float speed);

	/*!*****************************************************************************
	Predict player's final position when the projectile is launched, based on
	player's past few positions. Automatically update projectile's velocity.
	Player's current direction takes 50% weightage, while history takes the other 50.

	*Require

	\param projectile
	- Entity id of projectile

	\param speed
	- Speed of projectile

	\param deciseconds
	- Amount of time to take account of player's past position (in scale of 1/10)
	*******************************************************************************/
	void PredictiveShootPlayer(Entity projectile, float speed, short deciseconds, unsigned weightage);

	glm::vec3 CalculatePredictiveVelocity(glm::vec3 const& p1p0, glm::vec3 const& v0, const float s1);

private:
	Entity mPlayerEntity;
	Transform* mPlayerTransform;
	short mPlayerArrayFirstCounter;
	short mPlayerHistorySize;																											// Counting the size of mPlayerHistory container
	short mPlayerArrayIndex;																											// Indicating the latest index in the array to replace
	std::array<glm::vec3, MAX_DECISECOND_PLAYER_HISTORY> mPlayerHistory;					// Contains the player's position for the past 3 seconds, storing every deciseconds
	std::unordered_map<std::string, std::set<Entity>> mAILists;										// Contains the different list of AIs of different classification. string is the name of container
};










