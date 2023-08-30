/*!*************************************************************************
****
\file Editor.h
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 22-9-2022
\brief
This header file contains main loop logic for drawing all related IMGUI window
classes declared such as Inspect, ContentBrowser.. etc, setting up / creation
of all classes, deletion of classes and execution of IMGUI related component.

The functions
- UIinit
Initialize IMGUI related data, prepare for rendering UI necessary resosures
- WindowUpdate
Run swapping of window buffer/polling event
- UIupdate
Contains main loop for rendering all window of inherited class of EditorWindow
which are sections of UI respectively for their own purposes
- UIend
End call function for IMGUI
-UIdraw
Draw setup/calling for IMGUI related calls for drawing UI
- Getwindow
Returns main window for docking

****************************************************************************
***/
#pragma once

#include <GLFW/glfw3.h>
#include "../Imgui/imgui.h"
#include "EditorWindow.h"
#include <map>
#include <string>

class EditorWindow;
class MenuPanel;

/*******************************/
/*!
	Editor Class
*/
/*******************************/
class Editor
{
public:
	//static bool show_Inspector; // if inspector should be up
	static int entity;
	static bool Entity_Selected; // if entity selected 

	ImFont* bold{};
	ImFont* regular{};
	GLFWwindow* _Editorwindow{};
	MenuPanel* topwindow{};

	std::map<std::string, EditorWindow*> windowlist{};
	/***************************************************************************/
	/*!
	\brief
	Initialize IMGUI related data, prepare for rendering UI necessary resosures

	*/
	/**************************************************************************/
	void UIinit();
	/***************************************************************************/
	/*!
	\brief
	Run swapping of window buffer/polling event

	*/
	/**************************************************************************/
	void UIupdate();
	/***************************************************************************/
	/*!
	\brief
	Contains main loop for rendering all window of inherited class of EditorWindow
	which are sections of UI respectively for their own purposes

	*/
	/**************************************************************************/
	void UIend();

	/***************************************************************************/
	/*!
	\brief
	End call function for IMGUI

	*/
	/**************************************************************************/
	void WindowUpdate();
	/***************************************************************************/
	/*!
	\brief
	Draw setup/calling for IMGUI related function


	*/
	/**************************************************************************/
	void UIdraw(GLFWwindow*);
	/***************************************************************************/
	/*!
	\brief
	Returns main window for docking

	\param
		std::string

	\return
		EditorWindow*
	*/
	/**************************************************************************/
	EditorWindow* Getwindow(std::string id);




private:


};