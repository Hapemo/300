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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <map>
#include <string>
#include "MenuPanel.h"

class EditorWindow;
//class MenuPanel;

/*******************************/
/*!
	Editor Class
*/
/*******************************/
class Editor 
{
public:
	//static bool show_Inspector; // if inspector should be up
	//static int entity;
	//static bool Entity_Selected; // if entity selected 

	//ImFont* bold{};
	//ImFont* regular{};
	//GLFWwindow* window{};

	std::map<std::string, EditorWindow*> mWindowlist{};
/***************************************************************************/
/*!
\brief
Initialize IMGUI related data, prepare for rendering UI necessary resosures

*/
/**************************************************************************/
	void UIinit(GLFWwindow* window);
/***************************************************************************/
/*!
\brief
Run swapping of window buffer/polling event

*/
/**************************************************************************/
	void UIupdate(GLFWwindow* window);
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
	void WindowUpdate(GLFWwindow* window);
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

	MenuPanel* mMenuwindow{};
};

