/*!*****************************************************************************
\file DialoguePanel.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains function declarations for a Dialogue Panel Editor system that modifies
and view dialogues between a player and npc.
*******************************************************************************/
#pragma once
#include "ViewportPanel.h"

/*!*****************************************************************************
\brief
	This class encapsulates the functions for the Dialogue Panel Editor
*******************************************************************************/
class DialoguePanel : public ViewportPanel
{
public:
	
	void Init();
	void Update();
	void Free();

	
private:
	void NarrateDialogue();
	static std::string& BreakString(std::string& _str, int _offset, char _breakAt = ' ', std::string _breakWith = "\n");
	std::string FormatString(std::string);
	ImTextureID playerIcon;
	ImTextureID npcIcon;
	ImTextureID sendIcon;

	
	int selectedID;
	std::string dialog;
	std::string dialog2;
	std::string dialogEdit{};
	std::string dialogPrev{};
	int wrapsize;
	ImVec4 unselectedCol;
	ImVec4 selectedCol;
	ImVec4 npcCol;
	ImVec4 backgroundCol;

};