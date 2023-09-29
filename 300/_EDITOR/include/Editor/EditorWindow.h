/*!*************************************************************************
****
\file EditorWindow.h
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 28-09-2023
\brief
This file contains base function for an IMGUI window to render

The functions
- update
Pure Virtual update function 

- ~EditorWindow
Virtual Destructor

- init
Pure Virtual init function


****************************************************************************
***/
#pragma once
#include "imgui.h"
class Editor;

/*******************************/
/*!
	EditorWindow Class
*/
/*******************************/
class EditorWindow 
{
public:
	/*!*****************************************************************************
		Constructor for game Window
	*******************************************************************************/
	virtual ~EditorWindow() = default;
	/*!*****************************************************************************
		Virtual Initialize function
	*******************************************************************************/
	virtual void init() = 0;

	/*!*****************************************************************************
		Virtual Update function
	*******************************************************************************/
	virtual void update() = 0;
	
	
	ImGuiWindowFlags	mWinFlag {0};

	friend class Editor;


	//static Editor* Mainwindow;

private:

};