/*!*****************************************************************************
\file	LayerManager.h
\author Lee Hsien Wei, Joachim
\par	DP email: l.hsienweijoachim@digipen.edu
\par	Course: GAM200
\par	Group: Memory Leak Studios
\date	14-02-2023
\brief  This file contains the declaration of the Layer Manager Class and its
		member functions. It handles the updating of the layer values
		of each entity through the y-axis value comparisions with an offset
*******************************************************************************/
#pragma once

// -----------------------------
// Include files
// -----------------------------
#include "ECS_systems.h"

/*!*****************************************************************************
\brief Layer Manager system class that handles the updating of layer values of 
	   entities.
*******************************************************************************/
class LayerManager : public System {
public:
	/*!*****************************************************************************
	\brief
	Update function that calls the inner step function once enough time has passed
	\return void
	NULL
	*******************************************************************************/
	void Update(const double& _appDT);
	
	/*!*****************************************************************************
	\brief
	Step function that checks which entities layer collider collides and stores that
	as a contact info for resolution of the layer values
	\return void
	NULL
	*******************************************************************************/
	void Step();
private:
	double mAccumulatedDT{ 0.0 };						
	const double mFixedDT{ 1.0 / 60.0 };
	double mAccumulatedDTCap{ 1.0 };

	const int MIN_DYNAMIC_LAYER_VALUE{ 400 };
	const int MAX_DYNAMIC_LAYER_VALUE{ 600 };
};