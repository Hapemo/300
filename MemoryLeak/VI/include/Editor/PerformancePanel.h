/*!*****************************************************************************
\file PerformancePanel.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function declarations for a Performance Panel Editor system that displays Performance infomation 
*******************************************************************************/
#pragma once
#include "Panel.h"


/*!*****************************************************************************
\brief
	This class encapsulates the functions for the DebugPanel
*******************************************************************************/
class PerformancePanel : public Panel
{
public:
	void Init();
	void Update();
	void Free();
	
	
private:
	int updateTime;
	std::vector<std::deque<float>> system;
	std::vector < std::pair < std::string, double >> performance;
};