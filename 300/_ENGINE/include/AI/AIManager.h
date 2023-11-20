#pragma once
#include "pch.h"
#include "glm/glm.hpp"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
// TODO
// 
// 
// Testing needed
// - Test the spreadout
// - Test the shot prediction
// - Test flying enemy movement
// 

#define ADDITEMARR_E_MOVEMENT_TYPE(x) arr[static_cast<int>(E_MOVEMENT_TYPE::x)] = #x;

#define MAX_DECISECOND_PLAYER_HISTORY 30
#define PLAYER_NAME "Player"

enum class E_MOVEMENT_TYPE : char {
	BEGIN,
	GROUND_DIRECT,		// Directly moving to target on ground
	AIR_HOVER,						// Hover around the target
	SIZE

};
E_MOVEMENT_TYPE& operator++(E_MOVEMENT_TYPE& _enum);

//enum class E_ATTACK_TYPE : char {
//	CONTACT = 1,
//	SHOOTING
//};

class AIManager {
public:
	AIManager();

	/*!*****************************************************************************
	Does the following tasks:
	- track player position
	*******************************************************************************/
	void Update(float _dt);

	/*!*****************************************************************************
	Takes the position of player every 0.1s and keep a history of it. Will not run
	if mPlayer is entt::null
	* This function has to be ran every loop. *
	*******************************************************************************/
	void TrackPlayerPosition(float _dt);

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

	/*!*****************************************************************************
	Initialise the AI data of all the AI in active gamestate.
	Initialise player if it exists.
	* Player name has to be same as macro PLAYER_NAME *
	* Has to be called at start of every game state with AI component*
	*******************************************************************************/
	void InitAIs();

	/*!*****************************************************************************
	Wipe out all the data of entities in AI Manager
	* Has to be called at end of every game state with AI component*
	*******************************************************************************/
	void ClearAIs();

	void RemoveAIFromEntity(Entity _e);

	std::array<std::string, static_cast<size_t>(E_MOVEMENT_TYPE::SIZE)> const& GetMovementTypeArray() { return mMovementTypeArray; }

public:
	// Script Functions

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
	void PredictiveShootPlayer(Entity projectile, float speed, int deciseconds, unsigned weightage);

	/*!*****************************************************************************
	Get the direction of entity which movement is dictated by AI.

	\param _e
	- Entity to find direction of

	\return glm::vec3
	- Direction of movement dictated by AI
	*******************************************************************************/
	glm::vec3 GetDirection(Entity _e);



	/*!*****************************************************************************
	Test if one entity is in the line of sight of another entity. Check if target 
	is within view of horizontal angle from the center line of sight

	\param _eye
	- Entity looking from

	\param _tgt
	- Entity looking at

	\param _horizontalAngle
	- horizontal angle from the direct line of sight

	\param _verticalLimit
	- vertical height limit where the entity can see

	\return bool
	- True if able to see target, otherwise false
	*******************************************************************************/
	bool ConeOfSight(Entity _eye, Entity _tgt, float _horizontalAngle, float _verticalLimit);

	bool LineOfSight(Entity _eye, Entity _tgt);

	/*!*****************************************************************************
	Does the following procedures to initialise the AI
	- Adds the AI to AI Manager to keep track
	- Initialises the Target entity ID with the target name
	* Must be called when AI Setting component is added to an entity *
	*******************************************************************************/
	void InitialiseAI(Entity _e);
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

	glm::vec3 GetAStarDir(Entity _e, AISetting const& _setting);

	bool CheckUseAStar(Entity _e, AISetting const& _setting);

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
	Calculate the predictive velocity to hit target
	index 0 being target, index 1 being projectile
	\param p1p0
	- Vector of projectile position to target position

	\param v0
	- Velocity of target

	\param s1
	- Speed of projectile

	\return glm::vec3
	- Predictive velocity of projectile
	*******************************************************************************/
	glm::vec3 CalculatePredictiveVelocity(glm::vec3 const& p1p0, glm::vec3 const& v0, const float s1);

	/*!*****************************************************************************
	Initialise mMovementTypeArray
	*******************************************************************************/
	static std::array<std::string, static_cast<size_t>(E_MOVEMENT_TYPE::SIZE)> MovementTypeArrayInit() {
		std::array<std::string, static_cast<size_t>(E_MOVEMENT_TYPE::SIZE)> arr;

		ADDITEMARR_E_MOVEMENT_TYPE(GROUND_DIRECT);
		ADDITEMARR_E_MOVEMENT_TYPE(AIR_HOVER);

		return arr;
	}

private:
	Entity mPlayerEntity;
	Transform* mPlayerTransform;
	int mPlayerHistorySize;																											// Counting the size of mPlayerHistory container
	int mPlayerArrayIndex;																											// Indicating the latest index in the array to replace
	std::array<glm::vec3, MAX_DECISECOND_PLAYER_HISTORY> mPlayerHistory;					// Contains the player's position for the past 3 seconds, storing every deciseconds
	std::unordered_map<std::string, std::set<Entity>> mAILists;										// Contains the different list of AIs of different classification. string is the name of container

	static const std::array<std::string, static_cast<size_t>(E_MOVEMENT_TYPE::SIZE)> mMovementTypeArray; // Contains all the name of E_MOVEMENT_TYPE

};










