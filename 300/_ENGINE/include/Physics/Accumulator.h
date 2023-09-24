#pragma once

struct Accumulator
{
	static float mAccumulatedTime;
	static unsigned int mSteps;
	static const float mFixedDT;
	static void Update(float dt);
};