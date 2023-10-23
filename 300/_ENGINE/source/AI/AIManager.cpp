#include "AI/AIManager.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"

//--------------------------------------------------
// Public functions
//--------------------------------------------------
AIManager::AIManager() : mPlayerEntity(entt::null), mPlayerTransform(nullptr), mPlayerArrayIndex(0), mPlayerHistory(), mAILists() {
	mAILists[GROUND_ENEMY];
	mAILists[AIR_ENEMY];
}


glm::vec3 AIManager::GetDirection(Entity _e) {
	glm::vec3 dir{};
	switch (_e.GetComponent<AISetting>().mMovementType) {
	case E_MOVEMENT_TYPE::GROUND_DIRECT: dir = CalcGroundAIDir(_e);
		break;
	case E_MOVEMENT_TYPE::AIR_HOVER:
		break;
	}

	// Advance movement settings
	if (_e.GetComponent<AISetting>().mSpreadOut) SpreadOut(_e, dir);

	return dir;
}

void AIManager::TrackPlayerPosition() {
	if (mPlayerEntity == entt::null) return;
	
	mPlayerHistory[mPlayerArrayIndex] = mPlayerTransform->mTranslate;
	if (++mPlayerArrayIndex == MAX_DECISECOND_PLAYER_HISTORY) mPlayerArrayIndex = 0;
	if (mPlayerHistorySize != MAX_DECISECOND_PLAYER_HISTORY) ++mPlayerHistorySize;
}

void AIManager::SetPlayer(Entity _e) {
	mPlayerEntity = _e;
	mPlayerTransform = &_e.GetComponent<Transform>();
}

void AIManager::ResetPlayerTracking() {
	mPlayerEntity = entt::null;
	mPlayerTransform = nullptr;
	mPlayerHistorySize = 0;
}

void AIManager::SetPredictiveVelocity(Entity projectile, Entity target, float speed) {
	const glm::vec3 p1p0 = target.GetComponent<Transform>().mTranslate - projectile.GetComponent<Transform>().mTranslate;
	const glm::vec3 v0 = target.GetComponent<RigidBody>().mVelocity;
	const float s1 = glm::length(projectile.GetComponent<RigidBody>().mVelocity);

	projectile.GetComponent<RigidBody>().mVelocity = CalculatePredictiveVelocity(p1p0, v0, s1);

#if _DEBUG
	float actualSpeed = glm::length(projectile.GetComponent<RigidBody>().mVelocity);
	PASSERTMSG(!(actualSpeed > speed + cEpsilon) || (actualSpeed < speed + cEpsilon),
						 std::string("There is a calculation error with AIManager::PredictiveShootPlayer. Desired speed is " + std::to_string(speed) + " but actual speed is " + std::to_string(actualSpeed)).c_str());
#endif
}

void AIManager::PredictiveShootPlayer(Entity projectile, float speed, short deciseconds, unsigned weightage = 50) {
	int prevPosIndex{ mPlayerArrayIndex ? mPlayerArrayIndex - 1 : MAX_DECISECOND_PLAYER_HISTORY - 1 };
	glm::vec3* currPos{ &mPlayerEntity.GetComponent<Transform>().mTranslate };
	glm::vec3 accumulatedVector{};

	// Calculating the accumulated vector
	int i{};
	deciseconds = std::min(deciseconds, mPlayerHistorySize); // use the amount of history that exists in the container if it's lesser
	for (i = 0; i < deciseconds; ++i) {
		glm::vec3* prevPos = &mPlayerHistory[prevPosIndex];
		
		glm::vec3 difference = *currPos - *prevPos;
		accumulatedVector += difference;

		currPos = prevPos;
		if (--prevPosIndex == 0) prevPosIndex = MAX_DECISECOND_PLAYER_HISTORY - 1;
	}
	accumulatedVector /= i;

	PASSERTMSG(weightage <= 100, std::string("weightage is percentage, should be less than 100, but it is currently " + std::to_string(weightage)).c_str());

	// Accounting for the accumulated vector, calculate the predicted velocity
	const glm::vec3 p1p0 = mPlayerEntity.GetComponent<Transform>().mTranslate - projectile.GetComponent<Transform>().mTranslate;
	const glm::vec3 v0 = mPlayerEntity.GetComponent<RigidBody>().mVelocity/static_cast<float>(100-weightage) + accumulatedVector/static_cast<float>(weightage);
	const float s1 = glm::length(projectile.GetComponent<RigidBody>().mVelocity);

	projectile.GetComponent<RigidBody>().mVelocity = CalculatePredictiveVelocity(p1p0, v0, s1);

#if _DEBUG
	float actualSpeed = glm::length(projectile.GetComponent<RigidBody>().mVelocity);
	PASSERTMSG(!(actualSpeed > speed + cEpsilon) || (actualSpeed < speed + cEpsilon), 
							 std::string("There is a calculation error with AIManager::PredictiveShootPlayer. Desired speed is " + std::to_string(speed) + " but actual speed is " + std::to_string(actualSpeed)).c_str());
#endif
}

glm::vec3 AIManager::CalculatePredictiveVelocity(glm::vec3 const& p1p0, glm::vec3 const& v0, const float s1) {
	// Find time to travel
	const float a = glm::dot(v0, v0) - s1*s1;
	const float b = 2 * glm::dot(p1p0, v0);
	const float c = glm::dot(p1p0, p1p0);
	float t0 = (-b + sqrt(b*b - 4*a*c))/(2*a);
	const float t1 = (-b - sqrt(b*b - 4*a*c))/(2*a);

	if (t1 > 0) t0 = std::min(t0, t1);

	// Calculate and set vector
	return p1p0/t0 + v0;
}


//--------------------------------------------------
// Private functions
//--------------------------------------------------

glm::vec3 AIManager::CalcGroundAIDir(Entity _e) {
	// Calculate basic direction
	glm::vec3 tgtPos = Entity(_e.GetComponent<AISetting>().mTarget).GetComponent<Transform>().mTranslate;
	glm::vec3 dir = tgtPos - _e.GetComponent<Transform>().mTranslate;
	
	return dir;
}

void AIManager::SpreadOut(Entity _e, glm::vec3& dir) {
	AISetting setting{ _e.GetComponent<AISetting>() };
	// Get list of AIs from same catagory
	std::set<Entity>* aiList;
	switch (setting.mMovementType) {
	case E_MOVEMENT_TYPE::GROUND_DIRECT:
		aiList = &(mAILists[GROUND_ENEMY]);
		break;
	case E_MOVEMENT_TYPE::AIR_HOVER:
		aiList = &(mAILists[AIR_ENEMY]);
		break;
	}

	// Calculate position relative with all other AI of same catagory
	glm::vec3 allDeviatedDir{};
	glm::vec3 ePos{ _e.GetComponent<Transform>().mTranslate };
	for (Entity e : *aiList) {
		// Add all vector of e to _e
		glm::vec3 direction{ ePos - e.GetComponent<Transform>().mTranslate };
		allDeviatedDir += (direction / (glm::length(direction) * 10));
	}
	allDeviatedDir = glm::normalize(allDeviatedDir);

	// Add deviation to direction with respect to spreadout ratio
	dir = glm::normalize(dir + (allDeviatedDir * setting.mSpreadOut));
}



