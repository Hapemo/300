/*!*************************************************************************
****
\file SceneWindow.h
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 22-9-2022
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
class GameScene : public EditorWindow
{
public:


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
	Return true if mouse is within window

	*/
	/***************************************************************************/
	bool m_Hovered();





private:
};
