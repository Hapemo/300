#pragma once

/*!*****************************************************************************
\file PlayerDataManager.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-03-2022
\brief
This file contains function declarations for a player data system
*******************************************************************************/
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"
#include "pch.h"


class PlayerDataManager : public System
{
public:
	//add to scripting//////////////////
	void LoadPlayerData(std::string name);
	void SavePlayerData();

	std::string GetPlayerName();
	int GetLevelAt();
	void SetLevelAt(int levelat);

	int GetData1(int index);
	void SetData1(int index, int value);
	int GetData2(int index);
	void SetData2(int index, int value);
	//add to scripting/////////////////


	PlayerData GetPlayerData();
	void SetPlayerData(PlayerData playerData);

private:
	PlayerData myPlayerData;
};