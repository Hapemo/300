/*!*****************************************************************************
\file WorldViewPanel.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function definations for a WorldView Panel Editor system that displays the world scene
*******************************************************************************/
#pragma once
#include "ViewportPanel.h"

/*!*****************************************************************************
\brief
	This class encapsulates the functions for the WorldView Panel
*******************************************************************************/
class WorldViewPanel : public ViewportPanel
{
public:
	void Init();
	void Update();
	void Free();

	
private:
	void ArrowKeyMoveCam();
	void MouseClickMoveCam();
	void ScrollMoveCam();
	void NewDragDropEntity();
	void NewPrefabee();
	void SetSelectedEntity();
	void MoveSelectedEntity();
	void pointPicker();
	void UseGuizmo();
	void MouseOverObject();
	void PopObject();
	//static int newEntityCount;
	Entity e;
	const Entity* pop = nullptr;
	std::vector <std::pair<const Entity*, int>> popList{};
};