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
#include "ColorAIManager.h"
#include <ECSManager.h>


void ColorAIManager::Init()
{
	//for (auto& e : mEntities) {
		//addTransform(e, e.GetComponent<Transform>());
	//}
}
void ColorAIManager::Update()
{
	for (auto& e : mEntities) {
		if (!e.ShouldRun())
			continue;

		if (e.GetComponent<ColorAI>().run)
		{
			if (e.GetComponent<ColorAI>().targetColors.size() <= 1)
			{
				e.GetComponent<ColorAI>().run = false;
				return;
			}
			if (e.GetComponent<ColorAI>().setStep != -1)
			{
				e.GetComponent<ColorAI>().nextStep = e.GetComponent<ColorAI>().setStep;
				e.GetComponent<ColorAI>().setStep = -1;
			}
			if (e.GetComponent<ColorAI>().nextStep < 0 || e.GetComponent<ColorAI>().nextStep >= e.GetComponent<ColorAI>().targetColors.size())
			{
				e.GetComponent<ColorAI>().run = false;
				return;
			}
			if (e.GetComponent<ColorAI>().state == 0)//initialise
			{
				e.GetComponent<ColorAI>().step = e.GetComponent<ColorAI>().nextStep;
				e.GetComponent<ColorAI>().currtime = e.GetComponent<ColorAI>().time[e.GetComponent<ColorAI>().step];
				e.GetComponent<ColorAI>().state= 1;
			}
			else if (e.GetComponent<ColorAI>().state == 1)//move to target
			{
				float r = (float)(e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].r);
				float g = (float)(e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].g);
				float b = (float)(e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].b);
				float a = (float)(e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].a);
				//if (((e.GetComponent<ColorAI>().spriteORtext == 1) || (e.GetComponent<ColorAI>().spriteORtext == 2)) && e.HasComponent<Text>())
				if ((e.GetComponent<ColorAI>().spriteORtext == 1)  && e.HasComponent<Text>())
				{
					if (e.GetComponent<ColorAI>().changeR)
						r -= e.GetComponent<Text>().color.r;
					if (e.GetComponent<ColorAI>().changeG)
						g -= e.GetComponent<Text>().color.g;
					if (e.GetComponent<ColorAI>().changeB)
						b -= e.GetComponent<Text>().color.b;
					if (e.GetComponent<ColorAI>().changeA)
						a -= e.GetComponent<Text>().color.a;
				}
				else //if (((e.GetComponent<ColorAI>().spriteORtext == 0) || (e.GetComponent<ColorAI>().spriteORtext == 2)) && e.HasComponent<Sprite>())
				{
					if (e.GetComponent<ColorAI>().changeR)
						r -= e.GetComponent<Sprite>().color.r;
					if (e.GetComponent<ColorAI>().changeG)
						g -= e.GetComponent<Sprite>().color.g;
					if (e.GetComponent<ColorAI>().changeB)
						b -= e.GetComponent<Sprite>().color.b;
					if (e.GetComponent<ColorAI>().changeA)
						a -= e.GetComponent<Sprite>().color.a;
				}
				float timediff = e.GetComponent<ColorAI>().currtime / (float)FPSManager::dt;

				if (timediff >= 1.f)
				{
					r /= timediff;
					g /= timediff;
					b /= timediff;
					a /= timediff;
					if (((e.GetComponent<ColorAI>().spriteORtext == 1)|| (e.GetComponent<ColorAI>().spriteORtext == 2)) && e.HasComponent<Text>())
					{
						if (e.GetComponent<ColorAI>().changeR)
							e.GetComponent<Text>().color.r += (GLubyte)r;
						if (e.GetComponent<ColorAI>().changeG)
							e.GetComponent<Text>().color.g += (GLubyte)g;
						if (e.GetComponent<ColorAI>().changeB)
							e.GetComponent<Text>().color.b += (GLubyte)b;
						if (e.GetComponent<ColorAI>().changeA)
							e.GetComponent<Text>().color.a += (GLubyte)a;
						
					}
					if (((e.GetComponent<ColorAI>().spriteORtext == 0) || (e.GetComponent<ColorAI>().spriteORtext == 2)) && e.HasComponent<Sprite>())
					{
						if (e.GetComponent<ColorAI>().changeR)
							e.GetComponent<Sprite>().color.r += (GLubyte)r;
						if (e.GetComponent<ColorAI>().changeG)
							e.GetComponent<Sprite>().color.g += (GLubyte)g;
						if (e.GetComponent<ColorAI>().changeB)
							e.GetComponent<Sprite>().color.b += (GLubyte)b;
						if (e.GetComponent<ColorAI>().changeA)
							e.GetComponent<Sprite>().color.a += (GLubyte)a;
					}
					e.GetComponent<ColorAI>().currtime -= (float)FPSManager::dt;
				}
				else
					e.GetComponent<ColorAI>().state = 2;
				if (e.GetComponent<ColorAI>().currtime < 0.f)
				{
					e.GetComponent<ColorAI>().state = 2;
				}
			}
			else if (e.GetComponent<ColorAI>().state == 2) //end
			{
				e.GetComponent<ColorAI>().state = 0;
				if ((e.GetComponent<ColorAI>().spriteORtext == 1) || (e.GetComponent<ColorAI>().spriteORtext == 2) && e.HasComponent<Text>())
				{
					e.GetComponent<Text>().color.r = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].r;
					e.GetComponent<Text>().color.g = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].g;
					e.GetComponent<Text>().color.b = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].b;
					e.GetComponent<Text>().color.a = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].a;
					if (e.GetComponent<ColorAI>().spriteORtext == 2)
					{
						e.GetComponent<Sprite>().color.r = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].r;
						e.GetComponent<Sprite>().color.g = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].g;
						e.GetComponent<Sprite>().color.b = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].b;
						e.GetComponent<Sprite>().color.a = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].a;
					}
				}
				else
				{
					e.GetComponent<Sprite>().color.r = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].r;
					e.GetComponent<Sprite>().color.g = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].g;
					e.GetComponent<Sprite>().color.b = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].b;
					e.GetComponent<Sprite>().color.a = e.GetComponent<ColorAI>().targetColors[e.GetComponent<ColorAI>().step].a;
				}
				
				if (!e.GetComponent<ColorAI>().next)//move back to origial if loop
				{
					e.GetComponent<ColorAI>().run = false;
				}
				if (e.GetComponent<ColorAI>().reverse)
				{
					e.GetComponent<ColorAI>().nextStep--;
				}
				else
				{
					e.GetComponent<ColorAI>().nextStep++;
				}
				if (e.GetComponent<ColorAI>().loop)
				{
					if (e.GetComponent<ColorAI>().nextStep < 0)
					{
						e.GetComponent<ColorAI>().reverse = false;
						e.GetComponent<ColorAI>().nextStep = 1;
					}
					else if (e.GetComponent<ColorAI>().nextStep >= e.GetComponent<ColorAI>().targetColors.size())
					{
						if (e.GetComponent<ColorAI>().cycle)
						{
							e.GetComponent<ColorAI>().nextStep = 0;
						}
						else
						{
							e.GetComponent<ColorAI>().reverse = true;
							e.GetComponent<ColorAI>().nextStep = (int)e.GetComponent<ColorAI>().targetColors.size() - 2;
						}
					}
				}
				else
				{
					if (e.GetComponent<ColorAI>().nextStep >= e.GetComponent<ColorAI>().targetColors.size())
					{
						e.GetComponent<ColorAI>().run = false;
						e.GetComponent<ColorAI>().nextStep = 1;
						return;
					}
				}
				if (e.GetComponent<ColorAI>().nextStep < 0)
				{
					e.GetComponent<ColorAI>().reverse = false;
					e.GetComponent<ColorAI>().nextStep = 1;
				}
			}
		}
	}
}
void ColorAIManager::AddColor(Entity e, Color c, float time)
{
	e.GetComponent<ColorAI>().targetColors.push_back(c);
	e.GetComponent<ColorAI>().time.push_back(time);
}
void ColorAIManager::AddColorAt(Entity e, Color c, float time, int index)
{
	if (index < 0 || index >= e.GetComponent<ColorAI>().targetColors.size())
		return;
	e.GetComponent<ColorAI>().targetColors.insert(e.GetComponent<ColorAI>().targetColors.begin()+index, c);
	e.GetComponent<ColorAI>().time.insert(e.GetComponent<ColorAI>().time.begin() + index, time);
}
void ColorAIManager::RemoveColorAt(Entity e, int index)
{
	if (index < 0 || index >= e.GetComponent<ColorAI>().targetColors.size())
		return;
	e.GetComponent<ColorAI>().targetColors.erase(e.GetComponent<ColorAI>().targetColors.begin() + index);
	e.GetComponent<ColorAI>().time.erase(e.GetComponent<ColorAI>().time.begin() + index);
}

bool ColorAIManager::SetNextStep(Entity e, int i)
{
	if (i >= 0 && i < e.GetComponent<ColorAI>().targetColors.size())
	{
		e.GetComponent<ColorAI>().setStep = i;
		return true;
	}
	else
	{
		return false;
	}
}
void ColorAIManager::StartAnimation(Entity e)
{
	e.GetComponent<ColorAI>().run = true;
}
void ColorAIManager::StopAfterThisAnimation(Entity e, bool next)
{
	e.GetComponent<ColorAI>().next = !next;
}
void ColorAIManager::StopAfterEndofAnimationLoop(Entity e, bool loop)
{
	e.GetComponent<ColorAI>().loop = !loop;
}
void ColorAIManager::ReverseOrderAfterNextAnimation(Entity e, bool reverse)
{
	e.GetComponent<ColorAI>().reverse = reverse;
}
void ColorAIManager::SetAnimationLoopToCycle(Entity e, bool cycle)
{
	e.GetComponent<ColorAI>().cycle = cycle;
}
