/*!*****************************************************************************
\file Accumulator.h
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Pepe Production
\date 15/08/2023
\brief
Accumulator class for fixed time step update. For systems where accuracy is important.
*******************************************************************************/
#pragma once

struct Accumulator
{
	static float mAccumulatedTime;
	static unsigned int mSteps;
	static const float mFixedDT;
	static void Update(float dt);
};