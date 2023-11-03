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
//#include <Windows.h>
//#include <WinUser.h>

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
		Input::mosposEditor = glm::vec2{ ImGui::GetMousePos().x ,ImGui::GetMousePos().y };

		systemManager->mGraphicsSystem->m_EditorSceneHovered = true;
	}
	else {
		systemManager->mGraphicsSystem->m_EditorSceneHovered = false;
	}

	//Input::m_EditorWindowPos = { ImGui::GetWindowPos().x+10 +(ImGui::GetWindowWidth() + 10)/2
	//	, ImGui::GetWindowPos().y + 40+(ImGui::GetWindowHeight() + 40)/2};

	static int xpos, ypos;
	glfwGetWindowPos(systemManager->GetWindow()->GetHandle(), & xpos, & ypos);

	static double xxpos, yypos;
	glfwGetCursorPos(systemManager->GetWindow()->GetHandle(), &xxpos, &yypos);

	Input::m_EditorWindowPos = { int(xpos + ImGui::GetWindowPos().x +(ImGui::GetWindowSize().x/2))
	, int(ypos  + ImGui::GetWindowPos().y+ (ImGui::GetContentRegionMax().y / 2)) };
	Input::m_EditorWindowPosUnadjusted = { int(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2) ),
		int(ImGui::GetWindowPos().y+ ImGui::GetContentRegionMax().y / 2) };

	//if (ImGui::IsMouseClicked(1)) {
	//	std::cout << xpos << " glfwX\n" << ypos << " glfwY\n";
	//	std::cout << ImGui::GetWindowPos().x << " WimguiX\n" << ImGui::GetWindowPos().y << " WimguiY\n";

	//}
	//if (ImGui::IsMouseClicked(0)) {
	//	SetCursorPos(Input::m_EditorWindowPos.x, Input::m_EditorWindowPos.y);
	//	std::cout << ImGui::GetMousePos().x << " MouseX\n" << ImGui::GetMousePos().y << " MouseY\n";
	//	std::cout << Input::m_EditorWindowPosUnadjusted.x << " EditorX\n" << Input::m_EditorWindowPosUnadjusted.y << " EditorY\n";
	//	std::cout << xxpos << " MouseGLFWX\n" << yypos << " MouseGLFWY\n";

	//}
	
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

