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

	vec3 editorcampos = editorcam.mPosition;
	ImGui::TextColored({ 0.8f, 0.9f, 0.9f, 1.f }, "Editor Camera Position: %0.2f, %0.2f, %0.2f", editorcampos.x, editorcampos.y, editorcampos.z);

	ImGui::Separator();
	
	vec3 editorcamdir = editorcam.direction();
	ImGui::TextColored({ 0.8f, 0.9f, 0.9f, 1.f }, "Editor Camera Direction: %0.2f, %0.2f, %0.2f", editorcamdir.x, editorcamdir.y, editorcamdir.z);

	ImGui::Separator();
	ImGui::Text("Camera Speed");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	ImGui::DragFloat("##CS", (float*)&systemManager->mGraphicsSystem->m_EditorCamera.mCameraSpeed);

	ImGui::Separator();
	ImGui::Text("Debug");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	static bool ischecked = systemManager->mGraphicsSystem->m_DebugDrawing;
	if (ImGui::Checkbox("##Debug", &ischecked)) {
		systemManager->mGraphicsSystem->m_DebugDrawing = ischecked ? 1 : 0;

		if (ischecked) {
			systemManager->mGraphicsSystem->m_GlobalTint.w = 0.3f;
		}
		else {
			systemManager->mGraphicsSystem->m_GlobalTint.w = 1.f;
		}

	}

	// the threshold for bloom
	ImGui::Separator();
	ImGui::TextColored({ 0.f,0.5f, 1.f, 1.f }, "Global Bloom Variables");
	ImGui::DragFloat3("Global Bloom Threshold", (float*)&systemManager->mGraphicsSystem->mAmbientBloomThreshold, 0.01f, 0.f, 1.f);
	ImGui::DragFloat("Global Exposure", &systemManager->mGraphicsSystem->mAmbientBloomExposure, 0.01f, 0.f, 5.f, "%0.2f");
	ImGui::DragFloat("Texel Offset", &systemManager->mGraphicsSystem->mTexelOffset, 0.01f, 0.f, 5.f, "%0.2f");
	ImGui::DragFloat("Sampling Weight", &systemManager->mGraphicsSystem->mSamplingWeight, 0.001f, 0.f, 5.f, "%0.4f");

	ImGui::InputInt("Gaussian Blur Amount", (int*)&systemManager->mGraphicsSystem->m_PingPongFbo.mblurAmount);
	if(systemManager->mGraphicsSystem->m_PingPongFbo.mblurAmount < 1)
		systemManager->mGraphicsSystem->m_PingPongFbo.mblurAmount = 1;

	ImGui::DragFloat("Chromatic Abberation Strength", &systemManager->mGraphicsSystem->mChromaticStrength, 0.001f, 0.0f, 1.f);

	ImGui::Checkbox("Enable Bloom", &systemManager->mGraphicsSystem->m_EnableBloom); 
	ImGui::Checkbox("Enable Chromatic Abberation", &systemManager->mGraphicsSystem->m_EnableChromaticAbberation);

	ImGui::DragFloat("Crosshair Thickness", (float*)&systemManager->mGraphicsSystem->m_CrosshairThickness);
	ImGui::DragFloat("Crosshair Inner", (float*)&systemManager->mGraphicsSystem->m_CrosshairInner);
	ImGui::DragFloat("Crosshair Outer", (float*)&systemManager->mGraphicsSystem->m_CrosshairOuter);

	ImGui::ColorPicker4("Global Tint", (float*)&systemManager->mGraphicsSystem->m_GlobalTint);
}