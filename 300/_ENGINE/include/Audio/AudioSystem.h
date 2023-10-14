/*!*************************************************************************
****
\file		   AudioSystem.h
\author(s)	   Cheong Ming Lun
\par DP email: m.cheong@digipen.edu
\date		   16-8-2023
\brief

This file contains the base AudioSystem class that supports the following functionalities:
- Loading in Audio Files (from components/directory)
- Manipulating Audio (Play/Pause/Stop)
****************************************************************************/
#pragma once
#include <fmod.hpp>		// Include [FMOD] Library
#include <iostream>
#include <map>
#include <filesystem>
#include <stdexcept>   // Added (9/25)
#include "Input/Input.h"
#include "AudioType.h" // BGM/SFX
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Debug/Logger.h"

enum AUDIOTYPE : unsigned char;

// [9/25] Decided that we need this if we want to track if the audio is still playing in a particular channel
/******************************************************************************/
/*!
	[Struct] Channel
	 \brief
	  - Contains relevant channel information.
 */
 /******************************************************************************/
struct Channel
{
	unsigned int   mChannelID;
	FMOD::Channel* mChannel;
	bool		   mIsPlayingSound;
};

// [10/13] Interfacing for (Designers) to use in [Scripting] 
/******************************************************************************/
/*!
	[Class] AudioSource (Scripting Object)
	 \brief
	  - Interfaced functions that make it easy to use on scripting side (port to Lua)
	  - Container for [Scripting] side to use.
	  - Works with <Audio> component for the data.
	 \brief
	  - Each <AudioSource> will have their own channel.
 */
 /******************************************************************************/
class AudioSource				// Not part of the system.
{
public:
	Audio* mAudioComponent;  // Relevant <Audio> component data.
public:
	AudioSource() : mAudioComponent(nullptr) {}

	//void Play();
	//void Stop();


};

/******************************************************************************/
/*!
	[Class] AudioSystem
	 \brief
	  - Core Audio System that supports the base functionalities of loading / manipulating sounds.
 */
 /******************************************************************************/
class AudioSystem
{
public:
	int MAX_AUDIO_FILES_PLAYING = 32;														// Number of Sounds (that are allowed to be played simultaneously)
	int NO_OF_BGM_CHANNELS_TO_INIT = 16;
	int NO_OF_SFX_CHANNELS_TO_INIT = 16;
public:
	void Init();
	void Update(float dt);
	void Exit();
	AudioSystem();
	~AudioSystem();

	void UpdateLoadAudio(Entity id);																	// [For Engine] - Add Component mid-way

public:
	int  ErrCodeCheck(FMOD_RESULT result);													// Debugging tool
	bool LoadAudio(std::string file_path, std::string audio_name);
	void LoadAudioFiles(std::filesystem::path file_path);									// Load single file
	void LoadAudioFromDirectory(std::filesystem::path file_path);							// Load files from directory
	void PlayAudio(std::string audio_name, AUDIOTYPE audio_type, float audio_vol = 1.0f);	// Play an audio based on it's name
	int  PlaySFXAudio(std::string audio_name, float audio_vol = 1.0f);						// Play an SFX Audio (specify volume)
	int  PlayBGMAudio(std::string audio_name, float audio_vol = 1.0f);						// Play an BGM Audio (specify volume)
	void SetSpecificChannelVolume(AUDIOTYPE audio_type, int id, float audio_vol);			// Set Specific Volume
	void SetAllSFXVolume(float audio_vol);													// Global Volume Setting (SFX)
	void SetAllBGMVolume(float audio_vol);													// Global Volume Setting (BGM)
	void MuteSFX();
	void MuteBGM();
	void StopAllSFX();
	void StopAllBGM();
	void TogglePauseAllSounds();
	void TogglePauseSFXSounds();
	void TogglePauseBGMSounds();
	void TogglePauseSpecific(AUDIOTYPE audio_type, int channel_id);

	// Added [10/13] - Audio Specific Functions
	/*void StopAudio(std::string audio_name);
	void SetAudioVolume()*/

	// Helper Functions...
public:
	Channel& FindChannel(AUDIOTYPE audio_type, int id);
	bool	 CheckAudioExist(std::string audio_name); // [W7 - 10/14]

	// Will build as needs require.

public:
	// Scripting Testing Functions (later port to Lua)
	void TestAudioSource();

private:
	// std::unordered_map<AUDIOTYPE, std::vector<FMOD::Channel*>>  mChannels;	// Depreciated [9/25]
	std::unordered_map<AUDIOTYPE, std::vector<Channel>>         mChannelsNew;	// Database of Channels (SFX / BGM)
	std::unordered_map<std::string, FMOD::Sound*>				mSounds;        // Database of Sounds (SFX/BGM)

	// Global Volume 
	float sfxVolume;
	float bgmVolume;

	// Number of Channels
	int no_of_sfx_channels = 0;
	int no_of_bgm_channels = 0;

	// Registers (Sound ID) - for channel management
	int next_avail_id_sfx = 0;
	int next_avail_id_bgm = 0;

	FMOD::System* system_obj = nullptr;

	// for performance
	double startTime = 0.0f;
	double endTime   = 0.0f;
};