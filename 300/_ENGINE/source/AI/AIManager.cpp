#include "AI/AIManager.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"

//--------------------------------------------------
// Public functions
//--------------------------------------------------
AIManager::AIManager() : mAILists() {
	mAILists[GROUND_ENEMY];
	mAILists[AIR_ENEMY];
}


glm::vec3 AIManager::GetDirection(Entity _e) {
	switch (_e.GetComponent<AISetting>().mMovementType) {
	case E_MOVEMENT_TYPE::GROUND: return CalcGroundAIDir(_e);
	case E_MOVEMENT_TYPE::AIR:

		break;
	}
	return glm::vec3();
}

glm::vec3 AIManager::GetNextPosition(Entity _e) {
	glm::vec3 nextPos{};
	switch (_e.GetComponent<AISetting>().mMovementType) {
	case E_MOVEMENT_TYPE::GROUND:
		break;
	case E_MOVEMENT_TYPE::AIR:
		break;
	}
	
	return glm::vec3();
}



//--------------------------------------------------
// Private functions
//--------------------------------------------------

glm::vec3 AIManager::CalcGroundAIDir(Entity _e) {
	// Calculate basic direction
	AISetting setting = _e.GetComponent<AISetting>();
	glm::vec3 tgtPos = setting.mTarget.GetComponent<Transform>().mTranslate;
	glm::vec3 dir = tgtPos - _e.GetComponent<Transform>().mTranslate;
	
	// Advance direction changing
	if (setting.mSpreadOut) SpreadOut(_e, dir);
	
	return dir;
}

void AIManager::SpreadOut(Entity _e, glm::vec3& dir) {
	AISetting setting{ _e.GetComponent<AISetting>() };
	// Get list of AIs from same catagory
	std::set<Entity>* aiList;
	switch (setting.mMovementType) {
	case E_MOVEMENT_TYPE::GROUND:
		aiList = &(mAILists[GROUND_ENEMY]);
		break;
	case E_MOVEMENT_TYPE::AIR:
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



