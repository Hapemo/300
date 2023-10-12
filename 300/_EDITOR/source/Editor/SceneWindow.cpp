/*!*************************************************************************
****
\file SceneWindow.cpp
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


#include "Editor.h"
#include "SceneWindow.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "imgui_internal.h"
#include "Hierarchy.h"
#include "ImGuizmo.h"
#include "GameState/GameStateManager.h"

typedef void    (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);

//bool SceneWindow::follow = false;
//
//bool				SceneWindow::Move	={ true };
//bool				SceneWindow::Scale	={ true };
//bool				SceneWindow::Rotate	={ true };

/***************************************************************************/
/*!
\brief
	Init function for Menupanel window
*/
/***************************************************************************/
void SceneWindow::init() 
{
	//empty
}
/***************************************************************************/
/*!
\brief
	Update function for Menupanel window
*/
/***************************************************************************/
void SceneWindow::update()
{
	mWinFlag |= ImGuiWindowFlags_NoScrollbar;

	//ConstrainedResize(nullptr);
	scene_m_Hovered = ImGui::IsWindowHovered();	
	const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetEditorAttachment(), viewportPanelSize, ImVec2(0,1), ImVec2(1,0));
		
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		if (objPicking) {
			unsigned int getid = systemManager->mGraphicsSystem->GetEntityID(((ImGui::GetMousePos().x - ImGui::GetWindowPos().x) / winSize.x),
				((ImGui::GetMousePos().y - ImGui::GetWindowPos().y) / winSize.y));

		//std::cout << getid << ": ID \n";
		systemManager->mGraphicsSystem->m_CameraControl = CAMERA_TYPE::CAMERA_TYPE_EDITOR;

			if (getid != 0) {
				Hierarchy::selectedId = static_cast<entt::entity>(getid);
				Hierarchy::selectionOn = true;
			}
		}
		//std::cout << getid << ": ID \n";
	}
	
	ImGui::SetItemAllowOverlap();
	//wevents = ImGui::IsItemHovered();  /// <-- This returns true if mouse is over the overlaped Test button

	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x-70);
	ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y+10);
	
	ImGui::SetItemAllowOverlap();

	winSize= { ImGui::GetWindowSize().x ,ImGui::GetWindowSize().y};

	if (ImGui::IsWindowHovered()) {
		systemManager->mGraphicsSystem->m_EnableScroll == true;
	}
	else {
		systemManager->mGraphicsSystem->m_EnableScroll == false;
	}
}

static void Square(ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = ImMin(data->DesiredSize.x, data->DesiredSize.y); }


void SceneWindow::ConstrainedResize([[maybe_unused]]bool* p_open)
{
	//auto SquareFunc =[](ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = ImMax(data->DesiredSize.x, data->DesiredSize.y); };
	// Helper functions to demonstrate programmatic constraints

	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), Square);

}

void SceneWindow::RenderGuizmo()
{
	glm::mat4 objectMatrix =
	{ 1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f };

	glm::mat4 cameraProjection = glm::mat4(systemManager->mGraphicsSystem->m_EditorCamera.mProjection);

	glm::vec3 rotation = glm::vec3(0, 0, 0);
	
	glm::mat4 cameraView = systemManager->mGraphicsSystem->m_EditorCamera.mView;

	glm::mat4 inverseCamera = cameraView;


	glm::vec3 trans(0, 0, 0);
	glm::vec3 rotate(0, 0, 0);
	glm::vec3 scale(0, 0, 0);

	if (Hierarchy::selectionOn == true) {

		Transform& transform = Entity(Hierarchy::selectedId).GetComponent<Transform>();


		glm::vec3 tempRot = /*{ transform->orientation.z,0,transform->orientation.x }*/{ 0,0,0 };

		ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transform.mTranslate), glm::value_ptr(transform.mRotate),
			glm::value_ptr(transform.mScale), glm::value_ptr(objectMatrix) /*glm::value_ptr(transformtemp)*/);

		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float)winSize.x, (float)winSize.y);

		//if (Move == true) {
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(objectMatrix) /*glm::value_ptr(transformtemp)*/);

	//	//}
	////	if (Rotate == true) {
	//		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
	//			ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL, glm::value_ptr(objectMatrix) /*glm::value_ptr(transformtemp)*/);
	////	}
	////	if (Scale == true) {
	//		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
	//			ImGuizmo::OPERATION::SCALE, ImGuizmo::WORLD, glm::value_ptr(objectMatrix));
	////	}

		if (ImGuizmo::IsOver()) {
			objPicking = false;
		}
		else {
			objPicking = true;
		}

		if (ImGuizmo::IsUsing()) {
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(objectMatrix), glm::value_ptr(transform.mTranslate),
				glm::value_ptr(tempRot), glm::value_ptr(transform.mScale));
			
	//		//transform->orientation.x += tempRot.z;
		}


	}
}

