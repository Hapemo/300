/*!*****************************************************************************
\file LevelEditor.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definations for a Viewport Panel Editor system that displays the scene
*******************************************************************************/
#pragma once
#include "Panel.h"

#define buttonSize ImVec2(100,25)
#define moveHorizontal Math::Vec2{ 1, 0 }
#define moveVertical Math::Vec2{ 0, 1 }
#define moveZoom 0.1f
/*!*****************************************************************************
\brief
	This class encapsulates the functions for the Level Editor
*******************************************************************************/
class ViewportPanel : public Panel
{
public:
	void Init() {};
	void Update() {};
	void Free() {};

	Math::Vec2 GetMousePos() { return  worldMousePos; };
protected:
	enum class E_CAMERA_TYPE
	{
		GAME,
		WORLD
	};
	void SetViewportAspectRatio();
	void CalculateMousePos(E_CAMERA_TYPE _type);
	bool IsMouseInScreen();
	void renderUI();
	bool isViewportPaused = true;
	Math::Vec2 viewportSize{};
	Math::Vec2 viewportPos{};
	Math::Vec2 screenMousePos{};
	Math::Vec2 worldMousePos{};
	Math::Vec2 camMousePos{};

	Math::Vec2 camOffset{};
	Math::Vec2 camPos{};
	Math::Vec2 objectOffset{};
	static int isSelected;
	ImTextureID fameBufferImage{};
private:

};
