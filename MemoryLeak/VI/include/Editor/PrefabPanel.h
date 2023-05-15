/*!*****************************************************************************
\file PrefabPanel.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function declarations for a Hierarchy Panel Editor system 
that lists the entities and its components in the scene
*******************************************************************************/
#pragma once
#include "Panel.h"

/*!*****************************************************************************
\brief
	This class encapsulates the functions for the Level Editor
*******************************************************************************/
class PrefabPanel : public Panel
{
public:
	void LoadPrefab();
	void Init();
	void Update();
	void Free();

	
private:
	void newPrefab();
	void newPrefabee(PrefabPtr);
	void setSelectedPrefab(PrefabPtr p);
	void listComponents(PrefabPtr p, std::string _name);
};