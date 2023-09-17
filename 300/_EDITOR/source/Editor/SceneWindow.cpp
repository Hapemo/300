/*!*************************************************************************
****
\file SceneWindow.cpp
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


#include "Editor.h"
#include "SceneWindow.h"
#define _USE_MATH_DEFINES
#include <math.h>





//bool SceneWindow::follow = false;
//
//bool				SceneWindow::Move	={ true };
//bool				SceneWindow::Scale	={ true };
//bool				SceneWindow::Rotate	={ true };


void SceneWindow::init() 
{
	//empty
}

void SceneWindow::update()
{

	scene_m_Hovered = ImGui::IsWindowHovered();	

	ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetEditorAttachment(),ImGui::GetWindowSize());
	//ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetEditorAttachment(),ImVec2(1920, 1080));
}
