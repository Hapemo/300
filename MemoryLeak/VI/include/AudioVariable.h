/*!*****************************************************************************
\file AudioVariable.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-010-2022
\brief
This file contains struct Sound used in the sudio component and a Enum class for the channels used to play the audio
*******************************************************************************/
#pragma once
enum class E_CHANNEL
{
	BGM = 0,
	SFX,
	LAST
};
/*!*****************************************************************************
\brief
	This struct contains the data for a single Sound
*******************************************************************************/
struct Sound
{
	std::string path;
	bool toPlay = false;
	float volume =1.0f;
	float volumeMod =1.0f;
	float pitch = 1.0f;
	bool isPaused = false;
	bool isMute = false;
	bool isLoop = false;
	bool isRandPitch = false;
	bool toPlayOnCollision = false;
	int channel =0;
	
};
//play = vol *volmod*spacialdistance*slider