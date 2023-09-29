/*!*************************************************************************
****
\file EditorLogger.h
\author Han ChengWen, Lor Xaun Yun Michelle
\par DP email: c.han@digipen.edu, xaunyunmichelle.lor@digipen.edu
\date 28-09-2023
\brief
This file contains logger UI system that display logging information in an
IMGUI window, with function such as filtering/ scrolling to find necessary
DEBUG information from the DEBUGGER SYSTEM in ENGINE.

The functions
- update
Contains main loop for the logic of logger UI system functionality
****************************************************************************
***/
#pragma once
#include "EditorWindow.h"
#include "imgui.h"

/*******************************/
/*!
	LoggerWindow Class
*/
/*******************************/
class EditorLogger : public EditorWindow
{
	struct AppLog {
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool                AutoScroll;  // Keep scrolling if already at the bottom.
/***************************************************************************/
/*!
\brief
AppLog constructor
*/
/***************************************************************************/
	AppLog();
	/***************************************************************************/
	/*!
	\brief
	Clear log buffer
	*/
	/***************************************************************************/
	void Clear();
	/***************************************************************************/
	/*!
	\brief
	Contains function that allows DEBUGGER to add logging information into display
	IMGUI clipper
	*/
	/***************************************************************************/
	void AddLog(const char*, ...);
	/***************************************************************************/
	/*!
	\brief
	Draw logger with apparent information to be displayed.
	*/
	/***************************************************************************/
	void Draw(const char*);
	};

public:

/***************************************************************************/
/*!
\brief
Empty for virtual init
*/
/***************************************************************************/
void init();
/***************************************************************************/
/*!
\brief
Contains main loop for the logic of logger UI system functionality
*/
/***************************************************************************/
void update();
};
