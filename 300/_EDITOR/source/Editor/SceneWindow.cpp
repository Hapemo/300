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
#include "imgui_internal.h"


typedef void    (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);

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
	mWinFlag |= ImGuiWindowFlags_NoScrollbar;

	//ConstrainedResize(nullptr);
	scene_m_Hovered = ImGui::IsWindowHovered();	

	
	ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetEditorAttachment(),ImGui::GetWindowSize());
	

	//winSize_X = ImGui::GetWindowSize().x;
	//winSize_Y = ImGui::GetWindowSize().y;
	

	winSize= { ImGui::GetWindowSize().x ,ImGui::GetWindowSize().y};
	//systemManager->mGraphicsSystem->SetCameraSize()
	//ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetEditorAttachment(),ImVec2(1920, 1080));
}

static void Square(ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = ImMin(data->DesiredSize.x, data->DesiredSize.y); }


void SceneWindow::ConstrainedResize(bool* p_open)
{
	//auto SquareFunc =[](ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = ImMax(data->DesiredSize.x, data->DesiredSize.y); };
	// Helper functions to demonstrate programmatic constraints


	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), Square);

}



