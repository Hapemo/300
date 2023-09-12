/*!*************************************************************************
****
\file LeftPanel.h
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 22-9-2022
\brief
This header file contains the Leftpanel(Hierarchy) window and relative functions to run
the system logic, allowing user to select Entites, opening up Inspector window and modify
related variables.

The functions

- init
Initialize related data needed for Leftpanel window to draw

- update
Run Leftpanel logic that checks for Entity and render related data allows user
to select current Entity and activates inspector

****************************************************************************
***/
#pragma once

#include "Editor.h"
#include "EditorWindow.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"


extern bool CreateTexture; // test

/*******************************/
/*!
	LeftPanel Class
*/
/*******************************/
class Hierarchy : public EditorWindow {

public:
	static entt::entity selectedId;
	static bool selectionOn;


	bool mPopup {false};

/***************************************************************************/
/*!
\brief
Initialize related data needed for Leftpanel window to draw

*/
/**************************************************************************/
	void init();
/***************************************************************************/
/*!
\brief
Run Leftpanel logic that checks for Entity and render related data allows user
to select current Entity and activates inspector

*/
/**************************************************************************/
	void update();





	friend class Editor;

private:
	//Transform transform;
	//Texture texture;
	//bool isdeletedOnce = false;
};
