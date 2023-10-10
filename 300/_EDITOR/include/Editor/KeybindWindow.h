/*!*************************************************************************
****
\file KeybindWindow.h
\author Han ChengWen, Lor Xaun Yun Michelle
\par DP email: c.han@digipen.edu, xaunyunmichelle.lor@digipen.edu
\date 28-09-2023
\brief
This file contains Performance window functions that display
the Frames of different system etc Graphics,physics.. and also total frame for
whole execution.

The functions
- update
Contains main loop for the logic of Performance window, drawing Histograph,
Line graph as display setting.

- m_hovered
Return true if mouse is within window
****************************************************************************
***/
#pragma once

#include "EditorWindow.h"

/*******************************/
/*!
	Performance Class
*/
/*******************************/
class KeybindWindow : public EditorWindow {
public:
	/***************************************************************************/
	/*!
	\brief
	Init
	*/
	/**************************************************************************/
	void init();

	/***************************************************************************/
	/*!
	\brief
	Contains main loop for the logic of Performance window, drawing Histograph,
	Line graph as display setting.
	*/
	/**************************************************************************/
	void update();

///***************************************************************************/
///*!
//\brief
//Return true if mouse is within window
//
//\param
//    none
//
//\return bool
//Returns true if mouse is within window
//*/
///**************************************************************************/
//	bool m_Hovered();
//
//static float values[INTERVALS];
//static float graphicsValues[INTERVALS];
//static float physicsValues[INTERVALS];
//static float scriptingValues[INTERVALS];
//static float resourceValues[INTERVALS];
//static float gameStateValues[INTERVALS];
//static float audioValues[INTERVALS];
//static float loggerValues[INTERVALS];
//static float editorValues[INTERVALS];
////static float audioValues[INTERVALS];
//static float min;
//static int fpsCount;
//static int sysCount;


private:
	//bool scene_m_Hovered{};
	//float gDisplayTemp{}, pDisplayTemp{}, sDisplayTemp{},
	//	rDisplayTemp{}, gsDisplayTemp{}, aDisplayTemp{}, lDisplayTemp{}, eDisplayTemp{};
};
