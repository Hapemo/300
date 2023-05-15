/*!*****************************************************************************
\file AI.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definations for a AI system that modifies
Entities and its Components.
*******************************************************************************/
#include "MovementAIManager.h"
#include <ECSManager.h>


void MovementAIManager::Init()
{
	//for (auto& e : mEntities) {
		//addTransform(e, e.GetComponent<Transform>());
	//}
}
void MovementAIManager::Update()
{
	for (auto& e : mEntities) {
		if (!e.ShouldRun()) continue;
		if (e.GetComponent<MovementAI>().run)
		{
			if (e.GetComponent<MovementAI>().targetTransforms.size() <= 1)
			{
				e.GetComponent<MovementAI>().run = false;
				return;
			}
			if (e.GetComponent<MovementAI>().setStep != -1)
			{
				e.GetComponent<MovementAI>().nextStep = e.GetComponent<MovementAI>().setStep;
				e.GetComponent<MovementAI>().setStep = -1;
			}
			if (e.GetComponent<MovementAI>().nextStep < 0 || e.GetComponent<MovementAI>().nextStep >= e.GetComponent<MovementAI>().targetTransforms.size())
			{
				e.GetComponent<MovementAI>().run = false;
				return;
			}
			if (e.GetComponent<MovementAI>().state == 0)//initialise
			{
				e.GetComponent<MovementAI>().step = e.GetComponent<MovementAI>().nextStep;
				e.GetComponent<MovementAI>().currtime = e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step];
				e.GetComponent<MovementAI>().state= 1;
			}
			else if (e.GetComponent<MovementAI>().state == 1)//move to target
			{
				float timediff = e.GetComponent<MovementAI>().currtime / (float)FPSManager::dt;

				if (timediff >= 1.f)
				{
					if (e.GetComponent<MovementAI>().moveScale)
					{
					Math::Vec2 scalediff = e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].scale - e.GetComponent<Transform>().scale;
						scalediff /= timediff;
						e.GetComponent<Transform>().scale += scalediff;
					}
					if (e.GetComponent<MovementAI>().moveRotate)
					{
						float rotationdiff = e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].rotation - e.GetComponent<Transform>().rotation;
						rotationdiff /= timediff;
						e.GetComponent<Transform>().rotation += rotationdiff;
					}
					if (e.GetComponent<MovementAI>().moveTranslation)
					{
						Math::Vec2 translationdiff = e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].translation - e.GetComponent<Transform>().translation;
						translationdiff /= timediff;
						e.GetComponent<Transform>().translation += translationdiff;
					}
					e.GetComponent<MovementAI>().currtime -= (float)FPSManager::dt;
				}
				else
					e.GetComponent<MovementAI>().state = 2;
				if (e.GetComponent<MovementAI>().currtime < 0.f)
				{
					e.GetComponent<MovementAI>().state = 2;
				}
			}
			else if (e.GetComponent<MovementAI>().state == 2) //end
			{
				e.GetComponent<MovementAI>().state = 0;
				e.GetComponent<Transform>().scale = e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].scale;
				e.GetComponent<Transform>().rotation = e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].rotation;
				e.GetComponent<Transform>().translation = e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].translation;
				if (!e.GetComponent<MovementAI>().next)//move back to origial if loop
				{
					e.GetComponent<MovementAI>().run = false;
				}
				if (e.GetComponent<MovementAI>().reverse)
				{
					e.GetComponent<MovementAI>().nextStep--;
				}
				else
				{
					e.GetComponent<MovementAI>().nextStep++;
				}
				if (e.GetComponent<MovementAI>().loop)
				{
					if (e.GetComponent<MovementAI>().nextStep < 0)
					{
						e.GetComponent<MovementAI>().reverse = false;
						e.GetComponent<MovementAI>().nextStep = 1;
					}
					else if (e.GetComponent<MovementAI>().nextStep >= e.GetComponent<MovementAI>().targetTransforms.size())
					{
						if (e.GetComponent<MovementAI>().cycle)
						{
							e.GetComponent<MovementAI>().nextStep = 0;
						}
						else
						{
							e.GetComponent<MovementAI>().reverse = true;
							e.GetComponent<MovementAI>().nextStep = (int)e.GetComponent<MovementAI>().targetTransforms.size() - 2;
						}
					}
				}
				else
				{
					if (e.GetComponent<MovementAI>().nextStep >= e.GetComponent<MovementAI>().targetTransforms.size())
					{
						e.GetComponent<MovementAI>().run = false;
						e.GetComponent<MovementAI>().nextStep = 1;
						return;
					}
				}
				if (e.GetComponent<MovementAI>().nextStep < 0)
				{
					e.GetComponent<MovementAI>().reverse = false;
					e.GetComponent<MovementAI>().nextStep = 1;
				}
			}
		}
	}
}
void MovementAIManager::AddTransform(Entity e, Transform t, float time)
{
	e.GetComponent<MovementAI>().targetTransforms.push_back(t);
	e.GetComponent<MovementAI>().time.push_back(time);
}
void MovementAIManager::AddTransformAt(Entity e, Transform t, float time, int index)
{
	if (index < 0 || index >= e.GetComponent<MovementAI>().targetTransforms.size())
		return;
	e.GetComponent<MovementAI>().targetTransforms.insert(e.GetComponent<MovementAI>().targetTransforms.begin()+index, t);
	e.GetComponent<MovementAI>().time.insert(e.GetComponent<MovementAI>().time.begin() + index, time);
}
void MovementAIManager::RemoveTransformAt(Entity e, int index)
{
	if (index < 0 || index >= e.GetComponent<MovementAI>().targetTransforms.size())
		return;
	e.GetComponent<MovementAI>().targetTransforms.erase(e.GetComponent<MovementAI>().targetTransforms.begin() + index);
	e.GetComponent<MovementAI>().time.erase(e.GetComponent<MovementAI>().time.begin() + index);
}
void MovementAIManager::AddTransformDifference(Entity e, Transform d, float time)
{
	int i = (int)e.GetComponent<MovementAI>().targetTransforms.size() - 1;
	d.scale += e.GetComponent<MovementAI>().targetTransforms[i].scale;
	d.rotation += e.GetComponent<MovementAI>().targetTransforms[i].rotation;
	d.translation += e.GetComponent<MovementAI>().targetTransforms[i].translation;
	e.GetComponent<MovementAI>().targetTransforms.push_back(d);
	e.GetComponent<MovementAI>().time.push_back(time);
}
bool MovementAIManager::SetNextStep(Entity e, int i)
{
	if (i >= 0 && i < e.GetComponent<MovementAI>().targetTransforms.size())
	{
		e.GetComponent<MovementAI>().setStep = i;
		return true;
	}
	else
	{
		return false;
	}
}
void MovementAIManager::StartAnimation(Entity e)
{
	e.GetComponent<MovementAI>().run = true;
}
void MovementAIManager::ForceStopAnimation(Entity e)
{
	e.GetComponent<MovementAI>().run = false;
	//e.GetComponent<MovementAI>().state = 2;
}
void MovementAIManager::StopAfterThisAnimation(Entity e, bool next)
{
	e.GetComponent<MovementAI>().next = !next;
}
void MovementAIManager::StopAfterEndofAnimationLoop(Entity e, bool loop)
{
	e.GetComponent<MovementAI>().loop = !loop;
}
void MovementAIManager::ReverseOrderAfterNextAnimation(Entity e, bool reverse)
{
	e.GetComponent<MovementAI>().reverse = reverse;
}
void MovementAIManager::SetAnimationLoopToCycle(Entity e, bool cycle)
{
	e.GetComponent<MovementAI>().cycle = cycle;
}
void MovementAIManager::SetCalculatedTimeFromPosition(Entity e, Math::Vec2 pos, int step)
{
	if (step != -1)
	{
		SetNextStep(e, step);
	}
	Math::Vec2 diff = e.GetComponent<Transform>().translation - e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].translation;
	float time = std::max( diff.x / pos.x, diff.y / pos.y);
	e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step] = time;
}
void MovementAIManager::SetCalculatedTimeFromRotation(Entity e, float rot, int step)
{
	if (step != -1)
	{
		SetNextStep(e, step);
	}
	float diff = e.GetComponent<Transform>().rotation - e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].rotation;
	float time = diff/rot;
	e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step] = time;
}
void MovementAIManager::SetCalculatedTimeFromScale(Entity e, Math::Vec2 scale, int step)
{
	if (step != -1)
	{
		SetNextStep(e, step);
	}
	Math::Vec2 diff = e.GetComponent<Transform>().scale - e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].scale;
	float time = std::max(diff.x / scale.x, diff.y / scale.y);
	e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step] = time;
}