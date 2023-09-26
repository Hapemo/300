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
#include "Hierarchy.h"
#include "ImGuizmo.h"
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
	const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetEditorAttachment(), viewportPanelSize, ImVec2(0,1), ImVec2(1,0));
	
	
	
	
	
	
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		unsigned int getid = systemManager->mGraphicsSystem->GetEntityID(((ImGui::GetMousePos().x - ImGui::GetWindowPos().x)/winSize.x)*1600,
			((ImGui::GetMousePos().y - ImGui::GetWindowPos().y) / winSize.y)*900);

		std::cout << "Mouse pos " << ImGui::GetMousePos().x- ImGui::GetWindowPos().x << " " << ImGui::GetMousePos().y- ImGui::GetWindowPos().y << "\n";

		std::cout << getid << " i got something bitch\n";
		std::cout << (unsigned int)Hierarchy::selectedId <<"\n";
	}

	ImGui::SetItemAllowOverlap();
	//wevents = ImGui::IsItemHovered();  /// <-- This returns true if mouse is over the overlaped Test button


	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x-70);
	ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y+10);
	//ImGui::Checkbox("Debug",&systemManager->mGraphicsSystem->m_DebugDrawing);
	
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

	//static glm::mat3 transformtemp = glm::mat4(1.0f);
	//float transformtemp[16] {0};

	//Hierarchy::selectedId =;
	if (Hierarchy::selectionOn == true) {

		Transform& transform = Entity(Hierarchy::selectedId).GetComponent<Transform>();

		//glm::mat3 testMat{ 1.0 };

		glm::vec3 tempRot = /*{ transform->orientation.z,0,transform->orientation.x }*/{ 0,0,0 };

		ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transform.mTranslate), glm::value_ptr(transform.mRotate),
			glm::value_ptr(transform.mScale), glm::value_ptr(objectMatrix) /*glm::value_ptr(transformtemp)*/);

		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, winSize.x, winSize.y);

	//	//if (Move == true) {
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

		if (ImGuizmo::IsUsing()) {
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(objectMatrix), glm::value_ptr(transform.mTranslate),
				glm::value_ptr(tempRot), glm::value_ptr(transform.mScale));

	//		//transform->orientation.x += tempRot.z;
		}


	}
}

