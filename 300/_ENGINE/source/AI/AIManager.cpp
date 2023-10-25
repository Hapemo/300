#include "AI/AIManager.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "FPSManager.h"
#include "GameState/GameStateManager.h"

const std::array<std::string, static_cast<size_t>(E_MOVEMENT_TYPE::SIZE)> AIManager::mMovementTypeArray{ MovementTypeArrayInit() };

E_MOVEMENT_TYPE& operator++(E_MOVEMENT_TYPE& _enum) {
	_enum = static_cast<E_MOVEMENT_TYPE>(static_cast<int>(_enum) + 1);
	return _enum;
}

//--------------------------------------------------
// Public functions
//--------------------------------------------------
AIManager::AIManager() : mPlayerEntity(), mPlayerTransform(nullptr), mPlayerArrayIndex(0), mPlayerHistory(), mAILists() {
	E_MOVEMENT_TYPE i{ E_MOVEMENT_TYPE::BEGIN };
	while (++i != E_MOVEMENT_TYPE::SIZE)
		mAILists[mMovementTypeArray[static_cast<int>(i)]];

}

void AIManager::Update(float _dt) {
	TrackPlayerPosition(_dt);
}


glm::vec3 AIManager::GetDirection(Entity _e) {
	glm::vec3 dir{};
	switch (_e.GetComponent<AISetting>().mMovementType) {
	case E_MOVEMENT_TYPE::GROUND_DIRECT: dir = CalcGroundAIDir(_e);
		break;
	case E_MOVEMENT_TYPE::AIR_HOVER: dir = CalcAirAIDir(_e);
		break;
	}

	// Advance movement settings
	if (_e.GetComponent<AISetting>().mSpreadOut) SpreadOut(_e, dir);

	return dir;
}

void AIManager::TrackPlayerPosition(float _dt) {
	if (mPlayerEntity.id == entt::null) return;
	
	static float timePassed{};
	timePassed += _dt;
	if (timePassed >= 0.1) timePassed = 0;
	else return;

	mPlayerHistory[mPlayerArrayIndex] = mPlayerTransform->mTranslate;
	if (++mPlayerArrayIndex == MAX_DECISECOND_PLAYER_HISTORY) mPlayerArrayIndex = 0;
	if (mPlayerHistorySize != MAX_DECISECOND_PLAYER_HISTORY) ++mPlayerHistorySize;
}

void AIManager::SetPlayer(Entity _e) {
	if (_e.id == entt::null) return;
	mPlayerEntity = _e;
	mPlayerTransform = &_e.GetComponent<Transform>();
}

void AIManager::ResetPlayerTracking() {
	mPlayerEntity.id = entt::null;
	mPlayerTransform = nullptr;
	mPlayerHistorySize = mPlayerArrayIndex = 0;
	const glm::vec3 empty{ 0,0,0 };
	mPlayerHistory.fill(empty);
}

void AIManager::InitialiseAI(Entity _e) {
#if _DEBUG
	if (!_e.HasComponent<AISetting>()) {
		PWARNING("Attempted to initialise AI in entity with no AI Setting component. Entity name: %s", _e.GetComponent<General>().name);
		return;
	}
#endif
	AISetting& setting{ _e.GetComponent<AISetting>() };
	mAILists[mMovementTypeArray[static_cast<int>(setting.mMovementType)]].insert(_e);
	setting.SetTarget(systemManager->mGameStateSystem->GetEntity(setting.mTargetName));
}

void AIManager::InitAIs() {
	GameState* gs = systemManager->GetGameStateSystem()->GetCurrentGameState();
	auto const& entities = systemManager->ecs->GetEntitiesWith<AISetting>();
	for (auto entity : entities)
		InitialiseAI(Entity(entity));
	
	Entity player = systemManager->GetGameStateSystem()->GetEntity(PLAYER_NAME);
	if (player.id != entt::null) SetPlayer(player);
	else PWARNING("Attempted to set player in AI Manager, but was unable to find player");
}

void AIManager::ClearAIs() {
	ResetPlayerTracking();
	for (auto& EntityList : mAILists)
		EntityList.second.clear();
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
	glm::vec3 tgtPos = Entity(_e.GetComponent<AISetting>().GetTarget()).GetComponent<Transform>().mTranslate;
	glm::vec3 dir = tgtPos - _e.GetComponent<Transform>().mTranslate;
	
	return dir;
}

glm::vec3 AIManager::CalcAirAIDir(Entity _e) {
	AISetting const& _eSetting = _e.GetComponent<AISetting>();
	Transform const& _eTrans = _e.GetComponent<Transform>();
	Transform const& _tgtTrans = _eSetting.GetTarget().GetComponent<Transform>();

	// if AI is not above the target yet, gain height first, fly to mStayAway distance away.
	if (_eTrans.mTranslate.y <= _eSetting.mStayAway + _tgtTrans.mTranslate.y) return glm::vec3(0, 1, 0);

	float _eEdgeDistance = glm::length(_eTrans.mScale/2.f);
	float _tgtEdgeDistance = glm::length(_tgtTrans.mScale/2.f);
	float DistanceInBetween = abs(glm::length(_eTrans.mTranslate - _tgtTrans.mTranslate)) - (_eEdgeDistance + _tgtEdgeDistance);
	
	// When near the target's head, like 1.5x the stay away distance, move directly to the target until stay away distance
	if (DistanceInBetween > (_eSetting.mStayAway * 1.5f)) {
		// Move directly to the target's top
		glm::vec3 aboveTgtPos = _tgtTrans.mTranslate + glm::vec3(0, _eSetting.mStayAway, 0);

		return glm::normalize(aboveTgtPos - _eTrans.mTranslate);
	}

	// Move directly to the target if too far from player
	if (DistanceInBetween > _eSetting.mStayAway)
		return glm::normalize(_tgtTrans.mTranslate - _eTrans.mTranslate);

	return glm::vec3();
}

void AIManager::SpreadOut(Entity _e, glm::vec3& dir) {
	AISetting setting{ _e.GetComponent<AISetting>() };
	// Get list of AIs from same catagory
	std::set<Entity>& aiList = mAILists[mMovementTypeArray[static_cast<int>(setting.mMovementType)]];

	// Calculate position relative with all other AI of same catagory
	glm::vec3 allDeviatedDir{};
	glm::vec3 ePos{ _e.GetComponent<Transform>().mTranslate };
	for (Entity e : aiList) {
		// Add all vector of e to _e
		glm::vec3 direction{ ePos - e.GetComponent<Transform>().mTranslate };
		allDeviatedDir += (direction / (glm::length(direction) * 10));
	}
	allDeviatedDir = glm::normalize(allDeviatedDir);

	// Add deviation to direction with respect to spreadout ratio
	dir = glm::normalize(dir + (allDeviatedDir * setting.mSpreadOut));
}



