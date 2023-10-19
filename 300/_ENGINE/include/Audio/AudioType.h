#pragma once
/*!*************************************************************************
****
\file		   AudioType.h
\author(s)	   Cheong Ming Lun
\par DP email: m.cheong@digipen.edu
\date		   16-8-2023
\brief		   AudioTypes 
****************************************************************************/

enum AUDIOTYPE :unsigned char
{
	AUDIO_BGM,
	AUDIO_SFX, 
	AUDIO_NULL, // For Empty Instantiation
	UNDEFINED // for serialization helper functions
};