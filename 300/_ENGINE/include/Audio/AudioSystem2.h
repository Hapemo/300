#pragma once
#include <fmod.hpp>		// Include [FMOD] Library
#include <iostream>
#include <map>
#include <filesystem>
#include "Input.h"

#include "SoundComponent.h"

//struct AudioComponent
//{
//	AUDIO_TYPE type;
//	std::string name;
//	bool isPlaying;
//};

enum AUDIOTYPE :unsigned char
{
	AUDIO_BGM, 
	AUDIO_SFX
};

class AudioSystem
{	 
public:
	int MAX_AUDIO_FILES_PLAYING = 32;         // Number of Sounds (that are allowed to be played simultaneously)
	int NO_OF_BGM_CHANNELS_TO_INIT = 16;
	int NO_OF_SFX_CHANNELS_TO_INIT = 32;

public:
	void Init();
	void Update();
	void Exit();
	AudioSystem();
	~AudioSystem();

public:
	int ErrCodeCheck(FMOD_RESULT result); // Debugging tool
	void LoadAudioFiles(std::filesystem::path file_path); // Load single file
	void LoadAudioFromDirectory(std::filesystem::path file_path); // Load files from directory

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
	
	FMOD::System* system_obj;
};