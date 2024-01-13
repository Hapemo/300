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
#include <Hierarchy.h>
#include <Constants.h>
 /***************************************************************************/
 /*!
 \brief
	 Init function for Tab window
 */
 /***************************************************************************/
void TabWindow::init() {
}


std::string getMeshName(const std::string& input)
{
	return input.substr(input.find_last_of("/") + 1);
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

	if (ImGui::Button("Reset Camera Zoom"))
	{
		systemManager->mGraphicsSystem->m_EditorCamera.mFovDegree = GFX::CameraConstants::defaultFOV;
	}

	ImGui::Separator();
	ImGui::Text("Debug");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	bool ischecked = systemManager->mGraphicsSystem->m_DebugDrawing;
	if (ImGui::Checkbox("##Debug", &ischecked)) {
		systemManager->mGraphicsSystem->m_DebugDrawing = ischecked ? 1 : 0;
		systemManager->mGraphicsSystem->m_GlobalTint.w = ischecked ? 0.3f : 1.f;
	}


	// the threshold for bloom
	ImGui::Separator();
	ImGui::TextColored({ 0.f,0.5f, 1.f, 1.f }, "Global Bloom Variables");
	ImGui::DragFloat3("Global Bloom Threshold", (float*)&systemManager->mGraphicsSystem->mAmbientBloomThreshold, 0.01f, 0.f, 1.f);
	ImGui::DragFloat("Global Exposure", &systemManager->mGraphicsSystem->mAmbientBloomExposure, 0.01f, 0.f, 5.f, "%0.2f");

	if (ImGui::Checkbox("Enable Bloom", &systemManager->mGraphicsSystem->m_EnableBloom))
	{
		if (systemManager->mGraphicsSystem->m_EnableChromaticAbberation && !systemManager->mGraphicsSystem->m_EnableBloom)
		{
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Warning: CRT must be used with bloom!");
			systemManager->mGraphicsSystem->m_EnableBloom = true;
		}
	}

	if (systemManager->mGraphicsSystem->mBloomType == BloomType::GAUSSIANBLUR) {
		// Bloom type 1 specific variable
		ImGui::DragFloat("Texel Offset", &systemManager->mGraphicsSystem->mTexelOffset, 0.1f, 0.f, 5.f, "%0.2f");
		ImGui::DragFloat("Sampling Weight", &systemManager->mGraphicsSystem->mSamplingWeight, 0.001f, 0.f, 7.f, "%0.4f");
	}

	else if (systemManager->mGraphicsSystem->mBloomType == BloomType::GAUSSIANBLUR) {
		ImGui::DragFloat("Sampling Weight", &systemManager->mGraphicsSystem->mSamplingWeight, 0.001f, 0.f, 7.f, "%0.4f");
	}

	else if (systemManager->mGraphicsSystem->mBloomType == BloomType::PHYS_BASED_BLOOM) {
		ImGui::DragFloat("Filter Radius", &systemManager->mGraphicsSystem->mFilterRadius, 0.0001f, 0.f, 1.f, "%0.4f");
	}
	

	std::string bloomstr;

	switch (systemManager->mGraphicsSystem->mBloomType)
	{
		case BloomType::GAUSSIANBLUR:
			bloomstr = "Gaussian Blur";
			break;
		case BloomType::GAUSSIANBLUR_VER2:
			bloomstr = "Gaussian Blur Ver2";
			break;
		case BloomType::PHYS_BASED_BLOOM:
			bloomstr = "Physically Based Bloom";
			break;
		default:
			break;
	}

	if (ImGui::Button(bloomstr.c_str()))
	{
		// switch between the different bloom types
		systemManager->mGraphicsSystem->mBloomType = static_cast<BloomType>((static_cast<unsigned>(systemManager->mGraphicsSystem->mBloomType) + 1) % static_cast<unsigned>(BloomType::last_element));
	}

	if (ImGui::BeginCombo("EntityBloomThresholds", mBloomEntityStr.c_str()))
	{
		for (const auto& mapinst : mBloomEntityMap)
		{
			// Combo selector
			if (ImGui::Selectable(mapinst.second.c_str())) {
				mBloomEntityStr = mapinst.second;
			}
		}
		ImGui::EndCombo();
	}


	ImGui::Separator();
	ImGui::Checkbox("Enable Chromatic Abberation", &systemManager->mGraphicsSystem->m_EnableChromaticAbberation);
	ImGui::Checkbox("Enable CRT", &systemManager->mGraphicsSystem->m_EnableCRT);

	// Chromatic Abberation specific variables
	if (systemManager->mGraphicsSystem->m_EnableChromaticAbberation) 
	{
		ImGui::DragFloat("Chromatic Offset", &systemManager->mGraphicsSystem->mChromaticOffset, 0.001f, 0.0f, 1.f);
		ImGui::DragFloat("Chromatic Strength", &systemManager->mGraphicsSystem->mChromaticStrength, 0.01f, 0.0f, 1.f);
	}

	// CRT specific variables
	if (systemManager->mGraphicsSystem->m_EnableCRT)
	{
		ImGui::DragFloat("CRT Distortion", &PostProcessing::getInstance().mCRT_DistortionValue, 0.0001f, 0.0f, 0.1f, "%0.4f");
		ImGui::DragInt("CRT LineHeight", &PostProcessing::getInstance().mCRT_HeightOffset);
	}

	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();
	// this is to only show active meshrenderer entities
	for (Entity inst : meshRendererInstances)
	{
		// populate the map with the mesh name
		auto& meshinst = inst.GetComponent<MeshRenderer>();
		mBloomEntityMap[meshinst.mMeshPath] = getMeshName(meshinst.mMeshPath);
	}

	if (mBloomEntityStr.length())
	{
		Entity selectedMeshEntity;

		// select a mesh renderer entity
		for (Entity inst : meshRendererInstances)
		{
			if (getMeshName(inst.GetComponent<MeshRenderer>().mMeshPath) == mBloomEntityStr)
			{
				selectedMeshEntity = inst;
				break;
			}
		}

		// get the bloom threshold for the selected mesh
		ImGui::Text("Entity's Bloom Threshold");
		ImGui::DragFloat4("##EntityBloomThres", (float*)&selectedMeshEntity.GetComponent<MeshRenderer>().mBloomThreshold, 0.01f, 0.f, 1.f, "%0.2f");
		
		if (ImGui::Button("Set Bloom Threshold"))
		{
			// set the bloom threshold for all the entities with the same mesh
			for (Entity inst : meshRendererInstances)
			{
				if (getMeshName(inst.GetComponent<MeshRenderer>().mMeshPath) == mBloomEntityStr)
				{
					inst.GetComponent<MeshRenderer>().mBloomThreshold = selectedMeshEntity.GetComponent<MeshRenderer>().mBloomThreshold;
				}
			}
		}
	}


	ImGui::Separator();

	ImGui::ColorPicker4("Global Tint", (float*)&systemManager->mGraphicsSystem->m_GlobalTint);
}