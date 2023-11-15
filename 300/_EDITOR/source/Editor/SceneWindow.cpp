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
bool				SceneWindow::Move	={ true };
bool				SceneWindow::Scale	={ true };
bool				SceneWindow::Rotate	={ true };

bool				SceneWindow::buttonHovered = { false };
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

	if (ImGui::IsWindowHovered()) {
		Input::m_EditorSceneHovered = true;
	}
	else {
		Input::m_EditorSceneHovered = false;
	}
	

	mWinFlag |= ImGuiWindowFlags_NoScrollbar;

	scene_m_Hovered = ImGui::IsWindowHovered();	
	const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();	

	static bool meh{ false };
	ImGui::Checkbox("change", &meh);
	if (meh)
	{
		static int local = 0;
		ImGui::InputInt("mipss", & local);
		ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->m_PhysBloomRenderer.mBloomFBO.mMipChain[local].mTexture, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
	}
	else {
		ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetEditorAttachment(), viewportPanelSize, ImVec2(0,1), ImVec2(1,0));
	}
	
	//--------------------------------------------------------------------------------------------------// overlapping objects
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && buttonHovered == false) {
		if (objPicking) {
			unsigned int getid = systemManager->mGraphicsSystem->GetEntityID(((ImGui::GetMousePos().x - ImGui::GetWindowPos().x) / winSize.x),
				((ImGui::GetMousePos().y - ImGui::GetWindowPos().y) / winSize.y));

			systemManager->mGraphicsSystem->m_CameraControl = CAMERA_TYPE::CAMERA_TYPE_EDITOR;

			if (getid != 0) {
				Hierarchy::selectedId = static_cast<entt::entity>(getid);
				Hierarchy::selectionOn = true;
			}
		}
	}
	
	ImVec4 ve4scale = (Scale == false) ? ImVec4(0, 0, 0, 0.2f) : ImVec4(1, 1, 1, 0.6f);
	ImVec4 vec4rotate = (Rotate == false) ? ImVec4(0, 0, 0, 0.2f) : ImVec4(1, 1, 1, 0.6f);
	ImVec4 vec4move = (Move == false) ? ImVec4(0, 0, 0, 0.2f) : ImVec4(1, 1, 1, 0.6f);

	buttonHovered = false;

	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 120, 40));
	ImGui::PushStyleColor(ImGuiCol_Button, vec4move);
	if (ImGui::Button(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT)) {
		Move = (Move == false ) ? true : false;
	}

	if (ImGui::IsItemHovered())
		buttonHovered = true;

	ImGui::PopStyleColor();
	ImGui::SetItemAllowOverlap();

	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 90, 40));
	ImGui::PushStyleColor(ImGuiCol_Button, vec4rotate);
	if (ImGui::Button(ICON_FA_CIRCLE_NOTCH)) {
		Rotate = (Rotate == false) ? true : false;
	}

	if (ImGui::IsItemHovered())
		buttonHovered = true;

	ImGui::PopStyleColor();
	ImGui::SetItemAllowOverlap();

	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 60, 40));
	ImGui::PushStyleColor(ImGuiCol_Button, ve4scale);
	if (ImGui::Button(ICON_FA_SQUARE)) {
		Scale = (Scale == false) ? true : false;
	}

	if (ImGui::IsItemHovered())
		buttonHovered = true;

	ImGui::PopStyleColor();
	ImGui::SetItemAllowOverlap();

	//--------------------------------------------------------------------------------------------------// overlapping objects End


	//wevents = ImGui::IsItemHovered();  /// <-- This returns true if mouse is over the overlaped Test button

	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x-70);
	ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y+10);
	
	ImGui::SetItemAllowOverlap();

	winSize= { ImGui::GetWindowSize().x ,ImGui::GetWindowSize().y};

	if (ImGui::IsWindowHovered()) {
		systemManager->mGraphicsSystem->m_EnableScroll = true;
	}
	else {
		systemManager->mGraphicsSystem->m_EnableScroll = false;
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

		if (Move == true) {
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(objectMatrix) /*glm::value_ptr(transformtemp)*/);

		}
		if (Rotate == true) {
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL, glm::value_ptr(objectMatrix) /*glm::value_ptr(transformtemp)*/);
		}
		if (Scale == true) {
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION::SCALE, ImGuizmo::WORLD, glm::value_ptr(objectMatrix));
		}

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

