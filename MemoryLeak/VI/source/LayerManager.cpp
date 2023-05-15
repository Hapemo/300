/*!*****************************************************************************
\file	LayerManager.cpp
\author Lee Hsien Wei, Joachim
\par	DP email: l.hsienweijoachim@digipen.edu
\par	Course: GAM200
\par	Group: Memory Leak Studios
\date	14-02-2023
\brief  This file contains the definition of the Layer Manager Class and its
		member functions. It handles the updating of the layer values
		of each entity through the y-axis value comparisions with an offset
*******************************************************************************/

// -----------------------------
// Include files
// -----------------------------
#include "ECSManager.h"

/*!*****************************************************************************
\brief
Update function that calls the inner step function once enough time has passed
\return void
NULL
*******************************************************************************/
void LayerManager::Update(const double& _appDT) {
	mAccumulatedDT += _appDT;

	if (mAccumulatedDT > mAccumulatedDTCap)
		mAccumulatedDT = mAccumulatedDTCap;

	while (mAccumulatedDT >= mFixedDT) {
		Step();
		mAccumulatedDT -= mFixedDT;
	}
}

/*!*****************************************************************************
\brief
entityLayerCmp function which servers as the bool comparision function for sorting
purposes
\param const Entity&
A reference to read-only entity to compare with
\param const Entity&
A reference to read-only entity to compare against
\return bool
Comparision result
*******************************************************************************/
bool entityLayerCmp(const Entity& _e1, const Entity& _e2) {
	return (_e1.GetComponent<Transform>().translation.y + _e1.GetComponent<LayerCollider>().centerOffset.y) <= (_e2.GetComponent<Transform>().translation.y + _e2.GetComponent<LayerCollider>().centerOffset.y);
}

/*!*****************************************************************************
\brief
Step function that checks which entities layer collider collides and stores that
as a contact info for resolution of the layer values
\return void
NULL
*******************************************************************************/
void LayerManager::Step() {
	// Create sorted container using set custom compare
	std::set<Entity, decltype(&entityLayerCmp)> sortedEntities(&entityLayerCmp);
	for (auto& entity : mEntities)
		sortedEntities.emplace(entity);

	double index{ 0 };
	for (auto it{ sortedEntities.begin() }; it != sortedEntities.end(); ++it, ++index) {
		//if (it->GetComponent<General>().tag == TAG::PLAYER) {
		//	auto nextIt{ std::next(it, 1) };
		//	if (nextIt != sortedEntities.end())
		//		it->GetComponent<Sprite>().layer = nextIt->GetComponent<Sprite>().layer - 2;
		//	else
		//		it->GetComponent<Sprite>().layer = 255;
		//}
		it->GetComponent<Sprite>().layer = static_cast<int>(std::lerp(MAX_DYNAMIC_LAYER_VALUE, MIN_DYNAMIC_LAYER_VALUE, index / static_cast<double>(sortedEntities.size())));
	}
}