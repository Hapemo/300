#pragma once
#include <fmod.hpp>		// Include [FMOD] Library
#include <iostream>
#include <map>
#include <filesystem>
#include "Input.h"
#include "AudioType.h" // BGM/SFX
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"

enum AUDIOTYPE : unsigned char;

class AudioSystem
{	 
public:
	int MAX_AUDIO_FILES_PLAYING = 32;         // Number of Sounds (that are allowed to be played simultaneously)
	int NO_OF_BGM_CHANNELS_TO_INIT = 16;
	int NO_OF_SFX_CHANNELS_TO_INIT = 32;

	

public:
	void Init();
	void Update(float dt);
	void Exit();
	AudioSystem();
	~AudioSystem();

public:
	int  ErrCodeCheck(FMOD_RESULT result);													// Debugging tool
	void LoadAudio(std::string file_path, std::string audio_name);
	void LoadAudioFiles(std::filesystem::path file_path);									// Load single file
	void LoadAudioFromDirectory(std::filesystem::path file_path);							// Load files from directory
	void PlayAudio(std::string audio_name, AUDIOTYPE audio_type, float audio_vol = 1.0f);	// Play an audio based on it's name
	void PlaySFXAudio(std::string audio_name , float audio_vol = 1.0f);						// Play an SFX Audio (specify volume)
	void PlayBGMAudio(std::string audio_name , float audio_vol = 1.0f);						// Play an BGM Audio (specify volume)
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

	// Will build as needs require.
private:
	std::unordered_map<AUDIOTYPE, std::vector<FMOD::Channel*>> mChannels;
	std::unordered_map<std::string, FMOD::Sound*> mSounds;
	//std::unordered_map<AUDIOTYPE, std::unordered_map<std::string, FMOD::Sound*>> mSounds;

	// Global Volume 
	float sfxVolume;
	float bgmVolume;

	// Number of Channels
	int no_of_sfx_channels;
	int no_of_bgm_channels;

	// Registers (Sound ID) - for channel management
	//int next_avail_id_sfx;
	//int next_avail_id_bgm;

	FMOD::System* system_obj;
};