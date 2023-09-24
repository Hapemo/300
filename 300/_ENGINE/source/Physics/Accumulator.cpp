#include "Physics/Accumulator.h"

float Accumulator::mAccumulatedTime = 0.f;
const float Accumulator::mFixedDT = 1 / 60.f;
unsigned int Accumulator::mSteps = 0;

void Accumulator::Update(float dt)
{
	mSteps = 0;
	mAccumulatedTime += dt;
	while (mAccumulatedTime >= mFixedDT)
	{
		mAccumulatedTime -= mFixedDT;
		++mSteps;
	}
}
