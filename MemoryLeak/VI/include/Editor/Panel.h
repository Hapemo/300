/*!*****************************************************************************
\file LevelEditor.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for a Level Editor system that modifies
Entities and its Components.
*******************************************************************************/
#pragma once
#include "EditorManager.h"

/*!*****************************************************************************
\brief
	This class encapsulates the functions for the Level Editor
*******************************************************************************/
class Panel : public EditorManager
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Free() = 0;
	bool isActive() { return active; }
	void setIsActive(bool _active) { active = _active; }
protected:
	bool active = true;
private:
	
};