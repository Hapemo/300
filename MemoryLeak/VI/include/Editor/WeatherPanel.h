/*!*****************************************************************************
\file WeatherPanel.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 30-10-2022
\brief
This file contains function declarations for a WeatherPanel Editor system that modifies
and Weather tile map
*******************************************************************************/
#pragma once
#include "ViewportPanel.h"

/*!*****************************************************************************
\brief
	This class encapsulates the functions for the Weather Panel Editor
*******************************************************************************/
class WeatherPanel : public ViewportPanel
{
public:
	
	void Init();
	void Update();
	void Free();

	
private:
	//bool isWeatherPaused;
	ImTextureID sunIcon;
	ImTextureID rainIcon;
	ImTextureID windIcon;
	ImTextureID fogIcon;
	ImTextureID rainwindfogIcon;
	ImTextureID rainwindIcon;
	ImTextureID windfogIcon;
	ImTextureID rainfogIcon;
	ImTextureID selectedIcon;
};
