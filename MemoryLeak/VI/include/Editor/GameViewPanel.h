/*!*****************************************************************************
\file GameViewPanel.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for a GameView Panel Editor system that displays the game scene
*******************************************************************************/
#pragma once
#include "ViewportPanel.h"

/*!*****************************************************************************
\brief
	This class encapsulates the functions for the Level Editor
*******************************************************************************/
class GameViewPanel : public ViewportPanel
{
public:
	void Init();
	void Update();
	void Free();

	
private:
	void ArrowKeyMoveCam();
	void MouseClickMoveCam();
	void ScrollMoveCam();
	void ButtonClick();
	
};