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
	mWinFlag |= ImGuiWindowFlags_NoScrollbar;

	const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)(intptr_t)systemManager->mGraphicsSystem->GetGameAttachment(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

	winSize = { ImGui::GetWindowSize().x ,ImGui::GetWindowSize().y };

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		systemManager->mGraphicsSystem->m_CameraControl = CAMERA_TYPE::CAMERA_TYPE_GAME;
	}

	systemManager->mGraphicsSystem->m_EnableScroll |= ImGui::IsWindowHovered();

	if (ImGui::IsWindowHovered()) {
		Input::mosposEditor = glm::vec2 { ImGui::GetMousePos().x ,ImGui::GetMousePos().y};
	}
}

