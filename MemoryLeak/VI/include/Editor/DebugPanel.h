/*!*****************************************************************************
\file DebugPanel.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function declarations for a Debug Panel Editor system that displays debug infomation from logger
*******************************************************************************/
#pragma once
#include "Panel.h"


/*!*****************************************************************************
\brief
	This class encapsulates the functions for the DebugPanel
*******************************************************************************/
class DebugPanel : public Panel
{
public:
	void Init();
	void Update();
	void Free();
	
	
private:
	std::vector<std::pair<Logger::E_LOGLEVEL, std::string>> loggerStr;
	
};