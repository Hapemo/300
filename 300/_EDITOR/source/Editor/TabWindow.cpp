/**-------------------------------------------------------------------------------------/
 * @file
 * TabWindow.cpp
 * @author
 *  Han Cheng Wen, c,han@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Setting Scene function definition
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#include "TabWindow.h"
#include "Graphics/GraphicsSystem.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Systems.h"

#include <Graphics/Camera_Input.h>
 /***************************************************************************/
 /*!
 \brief
	 Init function for Tab window
 */
 /***************************************************************************/
void TabWindow::init() {
}
/***************************************************************************/
/*!
\brief
	Update function for tan window
*/
/***************************************************************************/
void TabWindow::update() 
{

	auto& editorcam = systemManager->mGraphicsSystem->m_EditorCamera;

	ImGui::Text("Editor Camera");
	//ImGui::SameLine();
	//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
	//	- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	ImGui::DragFloat3("Position", (float*)&editorcam.mPosition);

	ImGui::Separator();
	//ImGui::Text("Target");
	//ImGui::SameLine();
	//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
	//	- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	ImGui::DragFloat3("Target", (float*)&editorcam.mTarget);

	ImGui::Separator();
	ImGui::Text("Camera Speed");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	ImGui::DragFloat("##CS", (float*)&Camera_Input::getInstance().mCameraSpeed);

	//const auto& editorcam = systemManager->mGraphicsSystem->m_EditorCamera;
	//ImGui::Text("Editor Camera Position: %0.2f, %0.2f, %0.2f", editorcam.mPosition.x, editorcam.mPosition.y, editorcam.mPosition.z);
	//ImGui::Text("Editor Camera Target: %0.2f, %0.2f, %0.2f", editorcam.mTarget.x, editorcam.mTarget.y, editorcam.mTarget.z);

	ImGui::Separator();
	ImGui::Text("Debug");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	static bool ischecked = systemManager->mGraphicsSystem->m_DebugDrawing;
	if (ImGui::Checkbox("##Debug", &ischecked)) {
		systemManager->mGraphicsSystem->m_DebugDrawing = ischecked ? 1 : 0;
	}

	// the threshold for bloom
	ImGui::Separator();
	ImGui::TextColored({ 0.f,0.5f, 1.f, 1.f }, "Bloom Global Values");
	ImGui::DragFloat3("Bloom Threshold", (float*)&systemManager->mGraphicsSystem->mBloomThreshold, 0.01f, 0.f, 1.f);
	ImGui::InputFloat("Exposure", &systemManager->mGraphicsSystem->mBloomExposure, 0.01f, 0.1f, "%.2f");
}