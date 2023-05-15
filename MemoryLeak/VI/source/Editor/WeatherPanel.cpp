/*!*****************************************************************************
/*!*****************************************************************************
\file WeatherPanel.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 30-10-2022
\brief
This file contains function declarations for a WeatherPanel Editor system that modifies
and Weather tile map
*******************************************************************************/
#include "WeatherPanel.h"
#include <ECSManager.h>
#define weatherIocnSize ImVec2(18,18)
/*!*****************************************************************************
\brief
	Initializes the Weather Panel editor

\return
None.
*******************************************************************************/
void WeatherPanel::Init()
{
	std::srand((unsigned int)std::time(nullptr));
	aiManager->weatherAIinit();
	viewportSize = { 0,0 };
	isViewportPaused = true;
	sunIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\sunIcon.png");
	rainIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\rainIcon.png");
	fogIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\fogIcon.png");
	windIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\windIcon.png");
	rainwindfogIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\rainwindfogIcon.png");
	rainwindIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\rainwindIcon.png");
	windfogIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\windfogIcon.png");
	rainfogIcon = (void*)(intptr_t)spriteManager->GetTextureID("Textures\\Icons\\rainfogIcon.png");
}
/*!*****************************************************************************
\brief
	Updates the Weather Panel editor

\return
None.
*******************************************************************************/
void WeatherPanel::Update()
{
	/*if (!active)
		return;*/
	

	if (ImGui::Begin("Weather Editor"))
	{
		SetViewportAspectRatio();
		renderUI();
		/*Math::Vec2 pos = { (ImGui::GetWindowWidth() / 2.f) - 110.f, 30.f };
		ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		if (isWeatherPaused)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 0, 0)));
		else
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 150, 0)));
		if (ImGui::Button("Play", buttonSize))
		{
			isWeatherPaused = false;
		}
		ImGui::PopStyleColor();

		if (isWeatherPaused)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 150, 0)));
		else
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 0, 0)));
		ImGui::SameLine(0.f, 20.f);
		if (ImGui::Button("Pause", buttonSize))
		{
			isWeatherPaused = true;
		}
		ImGui::PopStyleColor();*/
		for (int w = 0; w < aiManager->getWeatherMap()[0].size(); w++)
		{
			for (int h = 0; h < aiManager->getWeatherMap().size(); h++)
			{
				if ((aiManager->getWeatherMap()[h][w]) == SUNNY)
				{
					//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 203, 81)));
					//ImGui::Button("S", weatherIocnSize);
					//ImGui::Image(sunIcon, weatherIocnSize, ImVec2(0, 1), ImVec2(1, 0));
					//ImGui::PopStyleColor();
					selectedIcon = sunIcon;
				}
				else if ((aiManager->getWeatherMap()[h][w]) == ALL)
				{
					//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 203, 81)));
					//ImGui::Button("S", weatherIocnSize);
					//ImGui::Image(sunIcon, weatherIocnSize, ImVec2(0, 1), ImVec2(1, 0));
					//ImGui::PopStyleColor();
					selectedIcon = rainwindfogIcon;
				}
				else
				{
					if ((aiManager->getWeatherMap()[h][w] &= RAIN))
					{
						//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 76, 153)));
						//ImGui::Button("R", weatherIocnSize);
						//ImGui::Image(rainIcon, weatherIocnSize, ImVec2(0, 1), ImVec2(1, 0));
						//ImGui::PopStyleColor();
						selectedIcon = rainIcon;
						if ((aiManager->getWeatherMap()[h][w] &= FOG))
						{
							selectedIcon = rainfogIcon;
						}
						else if ((aiManager->getWeatherMap()[h][w] &= WINDY))
						{
							selectedIcon = rainwindIcon;
						}

					}
					else if ((aiManager->getWeatherMap()[h][w] &= WINDY))
					{
						//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 200, 200)));
						//ImGui::Button("W", weatherIocnSize);
						//ImGui::Image(windIcon, weatherIocnSize, ImVec2(0, 1), ImVec2(1, 0));
						//ImGui::PopStyleColor();
						selectedIcon = windIcon;
						if ((aiManager->getWeatherMap()[h][w] &= RAIN))
						{
							selectedIcon = rainwindIcon;
						}
						else if ((aiManager->getWeatherMap()[h][w] &= FOG))
						{
							selectedIcon = windfogIcon;
						}
					}
					else if ((aiManager->getWeatherMap()[h][w] &= FOG))
					{
						//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(102, 204, 0)));
						//ImGui::Button("F", weatherIocnSize);
						//ImGui::Image(fogIcon, weatherIocnSize, ImVec2(0, 1), ImVec2(1, 0));
						//ImGui::PopStyleColor();
						selectedIcon = fogIcon;
						if ((aiManager->getWeatherMap()[h][w] &= RAIN))
						{
							selectedIcon = rainfogIcon;
						}
						else if ((aiManager->getWeatherMap()[h][w] &= WINDY))
						{
							selectedIcon = windfogIcon;
						}
					}
					else
						selectedIcon = rainwindfogIcon;
				}
				ImGui::Image(selectedIcon, weatherIocnSize, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine(0.0f, 2.f);
			}
			ImGui::NewLine();
		}

		if (!isViewportPaused)
		{
			aiManager->weatherAIupdate();
		}
	}
	ImGui::End();
}
/*!*****************************************************************************
\brief
	Free the Weather Panel editor

\return
None.
*******************************************************************************/
void WeatherPanel::Free()
{

}
