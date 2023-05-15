/*!*****************************************************************************
\file ViewportPanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function definations for a Viewport Panel Editor system that displays the scene
*******************************************************************************/
#include "ViewportPanel.h"
#include <ECSManager.h>
#include "GameStateManager.h"
int ViewportPanel::isSelected = 0;
//int ViewportPanel::newEntityCount = 1;
/*!*****************************************************************************
\brief
	This function set the view port to 16:9 aspect ratio regardless of window size

\return
None.
*******************************************************************************/
void ViewportPanel::SetViewportAspectRatio()
{
	viewportSize = { ImGui::GetWindowSize().x,ImGui::GetWindowSize().y - 70 };
	//viewportSize = { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y - 70 };
	if (viewportSize.x / viewportSize.y > ((float)* mWindowWidth) / ((float)*mWindowHeight)) //wide screen
	{
		viewportSize.x = viewportSize.y / ((float)*mWindowHeight) * ((float)*mWindowWidth);
	}
	else if (viewportSize.x / viewportSize.y < ((float)*mWindowWidth) / ((float)*mWindowHeight)) // tall screen
	{
		viewportSize.y = viewportSize.x / ((float)*mWindowWidth) * ((float)*mWindowHeight);
	}


	//if (viewportSize.x / viewportSize.y > 16 / 9.0f) //wide screen
	//{
	//	viewportSize.x = viewportSize.y / 9 * 16;
	//}
	//else if (viewportSize.x / viewportSize.y < 16 / 9.0f) // tall screen
	//{
	//	viewportSize.y = viewportSize.x / 16 * 9;
	//}

}
/*!*****************************************************************************
\brief
	This function Calculate the MousePos in world, screen and camera coordinates

\return
None.
*******************************************************************************/
void ViewportPanel::CalculateMousePos(E_CAMERA_TYPE _type)
{
	//to use matrix from graphics in the future
	viewportPos = { (ImGui::GetWindowWidth() - viewportSize.x) * 0.5f, buttonSize.y +35.f };
	//screenMousePos = Input::CursorPos() - Math::Vec2{ ImGui::GetWindowPos().x,ImGui::GetWindowPos().y } - viewportPos - viewportSize / 2;
	
	//viewportPos = { (ImGui::GetContentRegionAvail().x - viewportSize.x) * 0.5f, buttonSize.y + 35.f };
	Math::Vec2 cursorPos = {ImGui::GetMousePos().x, ImGui::GetMousePos().y};
	screenMousePos = cursorPos - Math::Vec2{ ImGui::GetWindowPos().x,ImGui::GetWindowPos().y } - viewportPos - viewportSize / 2;
	static int oldWinWidth = *mWindowWidth;
	static int oldWinHeight = *mWindowHeight;
	worldMousePos = screenMousePos;
	worldMousePos.y = -worldMousePos.y;
	//worldMousePos.x = worldMousePos.x / viewportSize.x * *mWindowWidth;
	//worldMousePos.y = worldMousePos.y / viewportSize.y * *mWindowHeight;
	worldMousePos.x = worldMousePos.x / viewportSize.x * oldWinWidth;
	worldMousePos.y = worldMousePos.y / viewportSize.y * oldWinHeight;
	if(_type == E_CAMERA_TYPE::GAME)
		camMousePos = worldMousePos * renderManager->GetGameCamera().GetZoom() + renderManager->GetGameCamera().GetPos();
	else if (_type == E_CAMERA_TYPE::WORLD)
		camMousePos = worldMousePos * renderManager->GetWorldCamera().GetZoom() + renderManager->GetWorldCamera().GetPos();
}
bool ViewportPanel::IsMouseInScreen()
{
	return (abs(screenMousePos.x) < viewportSize.x / 2 && abs(screenMousePos.y) < viewportSize.y / 2);
	
}
void ViewportPanel::renderUI()
{
	ImGui::SetWindowFontScale(1.5f);
	Math::Vec2 pos = { (ImGui::GetWindowWidth() / 2.f) - 110.f, 30.f };
	ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
	if (isViewportPaused)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 0, 0)));
	else
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 150, 0)));
	
	if (ImGui::Button("Play", buttonSize) || (Input::CheckKey(E_STATE::PRESS, E_KEY::P) && Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL)))
	{
		isViewportPaused = false;
		for (Scene& scene : GameStateManager::GetInstance()->mCurrentGameState->mScenes)
			for (const Entity& e : scene.mEntities)
				if (e.HasComponent<Script>())
					logicSystem->Alive(e);
		for (Scene& scene : GameStateManager::GetInstance()->mCurrentGameState->mScenes)
			for (const Entity& e : scene.mEntities)
				if (e.ShouldRun())
					if (e.HasComponent<Script>())
						logicSystem->Init(e);
		audioManager->ResumeAllChannels();
	}
	ImGui::PopStyleColor();

	if (isViewportPaused)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 150, 0)));
	else
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 0, 0)));
	ImGui::SameLine(0.f, 20.f);
	if (ImGui::Button("Pause", buttonSize)|| (Input::CheckKey(E_STATE::PRESS, E_KEY::K)&& Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL)))
	{
		isViewportPaused = true;
		audioManager->PauseAllChannels();
	}
	ImGui::PopStyleColor();
	ImGui::SetWindowFontScale(1.0f);

	/*ImGui::SetCursorPosX(ImGui::GetWindowWidth() - sendbuttonSize.x-30);
	ImGui::PushStyleColor(ImGuiCol_Button, unselectedCol);
	if (ImGui::Button("X", sendbuttonSize))
	{
		active = false;
		ImGui::PopStyleColor();
		return;
	}
	ImGui::PopStyleColor();*/
}

