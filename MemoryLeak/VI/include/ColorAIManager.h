/*!*****************************************************************************
\file AI.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definations for a AI system that modifies
Entities and its Components.
*******************************************************************************/
#pragma once
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"

#define _USE_MATH_DEFINES
#include <math.h>
/*!*****************************************************************************
DEFINES
*******************************************************************************/

/*!*****************************************************************************
\brief
	This class encapsulates the functions for AI manager
*******************************************************************************/
class ColorAIManager : public System
{
public:
	void Init();
	void Update();

	//Addtoscripting
	void StartAnimation(Entity e);
	void AddColor(Entity e, Color t, float time = 1.f);
	void AddColorAt(Entity e, Color t, float time = 1.f, int index=0);
	void RemoveColorAt(Entity e, int index);
	bool SetNextStep(Entity e, int i); // return ture if sucessful (withing 0 to the vector MAX)
	void StopAfterThisAnimation(Entity e, bool next = true);
	void StopAfterEndofAnimationLoop(Entity e, bool loop = true);
	void ReverseOrderAfterNextAnimation(Entity e, bool reverse = true);
	void SetAnimationLoopToCycle(Entity e, bool cycle = true);
	/*void SetCalculatedTimeFromPosition(Entity e, Math::Vec2 pos, int step =-1);
	void SetCalculatedTimeFromRotation(Entity e, float rot, int step =-1);
	void SetCalculatedTimeFromScale(Entity e, Math::Vec2 scale, int step =-1);*/
	//Addtoscripting
private:

};
