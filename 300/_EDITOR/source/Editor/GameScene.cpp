/**-------------------------------------------------------------------------------------/
 * @file
 *  GeneralScene.hpp
 * @author
 *  Han Cheng Wen, c,han@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  General Scene function definition
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#include "GameScene.h"



 /***************************************************************************/
 /*!
 \brief
	 Init function for Imgui Window
 */
 /**************************************************************************/
void GameScene::init() 
{

}

/***************************************************************************/
/*!
\brief
	Update function for Imgui Window
*/
/**************************************************************************/
void GameScene::update() 
{
	if (ImGui::IsWindowHovered()) {
		//Input::mosposEditor = glm::vec2{ ImGui::GetMousePos().x ,ImGui::GetMousePos().y };
		systemManager->mGraphicsSystem->m_EditorSceneHovered = true;
	}
	else {
		systemManager->mGraphicsSystem->m_EditorSceneHovered = false;
	}

	systemManager->mGraphicsSystem->m_EditorWindowPos = { ImGui::GetWindowPos().x+10 +(ImGui::GetWindowWidth() + 10)/2
		, ImGui::GetWindowPos().y + 40+(ImGui::GetWindowHeight() + 40)/2};

	mWinFlag |= ImGuiWindowFlags_NoScrollbar;

	const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetGameAttachment(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

	winSize = { ImGui::GetWindowSize().x ,ImGui::GetWindowSize().y };

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		systemManager->mGraphicsSystem->m_CameraControl = CAMERA_TYPE::CAMERA_TYPE_GAME;
	}

	systemManager->mGraphicsSystem->m_EnableScroll |= ImGui::IsWindowHovered();


}

