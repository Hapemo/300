/*!*****************************************************************************
\file LevelEditor.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for a AssetPanel that view recources 
*******************************************************************************/
#pragma once
#include "Panel.h"

/*!*****************************************************************************
\brief
	This class encapsulates the functions for the AssetPanel
*******************************************************************************/
class AssetPanel : public Panel
{
public:
	void Init();
	void Update();
	void Free();

	
private:
	void calculateSize();
	std::string texExt;
	std::string texPath;
	std::string texParent;
	std::string texFilename;
	std::string rootPath;
	
	ImTextureID sfxIcon;
	ImTextureID bgmIcon;
	ImTextureID backIcon;
	ImTextureID textIcon;
	ImTextureID sceneIcon;
	ImTextureID folderIcon;
	ImTextureID scriptIcon;
	ImTextureID prefabIcon;
	ImTextureID refreshIcon;
	ImTextureID dialogueIcon;
	ImTextureID selectedIcon;
	ImTextureID gamestateIcon;
	ImVec2 folderSize;
	int colNum;
};
