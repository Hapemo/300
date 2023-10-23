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
#include "Debug/EnginePerformance.h"
#include "GameState/GameStateManager.h"
#include "GameState/Scene.h"



enum AUDIOTYPE : unsigned char;

// [9/25] Decided that we need this if we want to track if the audio is still playing in a particular channel
/******************************************************************************/
/*!
	[Struct] Channel
	 \brief
	  - Contains relevant channel information.
	[Depreciated] - 10/15/2023
	  - Changing structure to each <Audio> component will hold their own channel.
	  - We will retain the [SFX] & [BGM] channels though.

 */
 /******************************************************************************/
struct Channel
{
	unsigned int   mChannelID;
	FMOD::Channel* mChannel;
	bool		   mIsPlayingSound;
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
	int MAX_AUDIO_FILES_PLAYING = 32;				 // Number of Sounds (that are allowed to be played simultaneously)
	int NO_OF_BGM_CHANNELS_TO_INIT = 16;
	int NO_OF_SFX_CHANNELS_TO_INIT = 16;
public:
	void Init();
	void Update(float dt);
	void PlayOnAwake();								// [For Editor] - Playing sounds on awake. (check for <Audio> component flag : "mPlayonAwake")
	void Pause();								    // [For Editor] - Pause sounds.
	void Reset();									// [For Editor] - Reset <Audio> component. Stop Playing all sounds.
	void Exit();
	AudioSystem();
	~AudioSystem();

	// Helper Functions (Loading)
	void UpdateLoadAudio(Entity id);				 // [For Engine] - Add Component mid-way
	void UpdateChannelReference(Entity id);		     // [For Engine] - Add Channel to the global [SFX/BGM] channels. (for global control)
	void InitAudioChannelReference(Entity id);		 // [For Engine]

	// Helper Functions (Sound)
	FMOD::Sound* FindSound(std::string audio_name);
	bool	     CheckAudioExist(std::string audio_name); // [W7 - 10/14]

public:
	// Retained Functions (Without Linking to <Audio> component)
	bool LoadAudioFromDirectory(std::filesystem::path file_path);					                                        // Load files from directory (Like just load all the audio files first) -> later then link
	bool LoadAudioFromDirectory(std::string directory_path);															    // [10/22] [For Lua]
	void SetAllSFXVolume(float audio_vol);																				    // Global Volume Setting (SFX)
	void SetAllBGMVolume(float audio_vol);													                                // Global Volume Setting (BGM)
	void MuteSFX();																											// Global Mute (SFX)
	void MuteBGM();																										    // Global Mute (BGM)
	void StopAllSFX();																								        // Global stop playing (SFX)
	void StopAllBGM();																										// Global stop playing (BGM)
	void PauseAllSounds();																									// Pause all sounds.
	void PauseSFXSounds();																									// Pause sfx sounds
	void PauseBGMSounds();																									// Pause bgm sounds
	void UnpauseAllSounds();																								// Unpause all sounds. 
	void UnpauseSFXSounds();																								// Unpause sfx sounds.
	void UnpauseBGMSounds();																								// Unpause bgm sounds. 

	/* Converted Functions(These functions link to each <Audio> component)
	* ---------------------------------------------------------------------------------- */
	// Functional 
	bool LoadAudio(std::string file_path, std::string audio_name, Audio* audio_component = nullptr);			            // Load a single audio. (Will be in [mSounds] database)
	void PlayAudio(std::string audio_name, AUDIOTYPE audio_type, float audio_vol = 1.0f, Audio* audio_component = nullptr);	// Toggle (mIsPlay) -> Plays in Update() loop.
	void StopAudio(Audio* audio_component);
	void PauseAudio(Audio* audio_component);

	// Helper (for Global Volume Functions)
	void UpdateSFXComponentVolume(float audio_vol);																		    // Used whenever there is an edit in volume [Audio in SFX]
	void UpdateBGMComponentVolume(float volume);																			// Used whenever there is an edit in volume [Audio in BGM]

public:
	int  ErrCodeCheck(FMOD_RESULT result);																					// Debugging tool				

	// Will build as needs require.

// Scripting Support
public:
	void PlayAudioSource(FMOD::Sound* comp_sound, FMOD::Channel* comp_channel, float vol = 1.0f);  // OK.
	void PlayAudioSource(Audio& audio_component, float vol = 1.0f);


// 3D Audio Stuff
public:
	float distance_factor = 1.0f;  // Units per meter. (per unit reference in game world)


public:
	FMOD::System* system_obj = nullptr;

	// For Editor (Pausing State)
	float system_paused = true; // initial yes.
	float fade_timer = 0.0f;

private:
	std::unordered_map<AUDIOTYPE, std::vector<std::pair<uid, FMOD::Channel*&>>> mChannelswID;   // Add [Channel ID] 
	std::unordered_map<std::string, FMOD::Sound*>				                mSounds;        // Database of Sounds (SFX/BGM)

	// Global Volume 
	float sfxVolume;
	float bgmVolume;

	// Number of Channels
	int no_of_sfx_channels = 0;
	int no_of_bgm_channels = 0;

	// Registers (Sound ID) - for channel management
	int next_avail_id_sfx = 0;
	int next_avail_id_bgm = 0;

	// for performance
	double startTime = 0.0f;
	double endTime = 0.0f;

};


