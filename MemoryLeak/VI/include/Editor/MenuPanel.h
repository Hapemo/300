/*!*****************************************************************************
\file MenuPanel.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function declarations for a Menu Panel Editor system
*******************************************************************************/
#pragma once
#include "Panel.h"

/*!*****************************************************************************
\brief
	This class encapsulates the functions for the Level Editor Main menu
*******************************************************************************/
class MenuPanel : public Panel
{
public:
	void Init();
	void Update();
	void Free();


private:
	std::string saveScene_filename;
	std::string openScene_filename;
	std::string saveDialog_filename;
	std::string openDialog_filename;
};
