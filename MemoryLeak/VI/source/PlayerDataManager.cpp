/*!*****************************************************************************
\file PlayerDataManager.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-03-2022
\brief
This file contains function definations for a player data system
*******************************************************************************/
#include "PlayerDataManager.h"
#include <ECSManager.h>



void PlayerDataManager::LoadPlayerData(std::string name)
{
	serializationManager->LoadPlayerData(name);
}
void PlayerDataManager::SetPlayerData(PlayerData playerData)
{
	myPlayerData = playerData;
}
PlayerData PlayerDataManager::GetPlayerData()
{
	return myPlayerData;
}

void PlayerDataManager::SavePlayerData()
{
	serializationManager->SavePlayerData(myPlayerData.name);
}

std::string PlayerDataManager::GetPlayerName()
{
	return myPlayerData.name;
}
int PlayerDataManager::GetLevelAt()
{
	return myPlayerData.levelAt;
}
void PlayerDataManager::SetLevelAt(int levelat)
{
	myPlayerData.levelAt = levelat;
}

int PlayerDataManager::GetData1(int index)
{
	if (index >= 0 && index < myPlayerData.data1.size())
		return myPlayerData.data1[index];
	else
		return -1;
}
void PlayerDataManager::SetData1(int index, int value)
{
	if (index < 0)
		return;
	else if (index < myPlayerData.data1.size())
		myPlayerData.data1[index] = value;
	else
	{
		myPlayerData.data1.resize(index+1);
		myPlayerData.data1[index] = value;
	}
}
int PlayerDataManager::GetData2(int index)
{
	if (index >= 0 && index < myPlayerData.data2.size())
		return myPlayerData.data2[index];
	else
		return -1;
}
void PlayerDataManager::SetData2(int index, int value)
{
	if (index < 0)
		return;
	else if (index < myPlayerData.data2.size())
		myPlayerData.data2[index] = value;
	else
	{
		myPlayerData.data2.resize(index + 1);
		myPlayerData.data2[index] = value;
	}
}
