/*!*************************************************************************
****
\file SceneWindow.h
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 28-09-2023
\brief
This file contains SceneWindow window functions provides the drawing of FrameBuffer
and display the objects with its respective camera system

The functions
- update
Contains main loop for the Scene window that draws framebuffer

- m_Hovered
Return true if mouse is within window

- Framebuffer_spec
Setting up specification for frame buffer rendering

****************************************************************************
***/
#pragma once
#include "Graphics/GraphicsSystem.h"
#include "EditorWindow.h"
#include <GLFW/glfw3.h>


/*******************************/
/*!
	SceneWindow Class
*/
/*******************************/
class SceneWindow : public EditorWindow 
{
public:

	//static bool				Move;
	//static bool				Scale;
	//static bool				Rotate;


	//ImGuiWindowFlags	mWinFlag;

	//float winSize_X;
	//float winSize_Y;

	glm::ivec2 winSize;
/***************************************************************************/
/*!
\brief
Initialization

*/
/**************************************************************************/
	void init();

/***************************************************************************/
/*!
\brief
Contains main loop for the Scene window that draws framebuffer

*/
/***************************************************************************/
	void update();

/***************************************************************************/
/*!
\brief
Setting up specification for frame buffer rendering

\param fb_reference
frame buffer reference for drawimg

\return
	none
*/
/***************************************************************************/
	void Framebuffer_spec(GLuint&);

/***************************************************************************/
/*!
\brief
Return true if mouse is within window

*/
/***************************************************************************/
	bool m_Hovered();



/***************************************************************************/
/*!
\brief
allowing dragging of object on scene window

*/
/***************************************************************************/
	void ClickDrop();


/***************************************************************************/
/*!
\brief
Render IMGUIZMO integration

*/
/***************************************************************************/
	void RenderGuizmo();




	void ConstrainedResize(bool* p_open);

	static bool			follow;
	bool				objPicking{ true };

private:

	bool			    scene_m_Hovered     {0};
	GLuint				Framebf				{0};
};
