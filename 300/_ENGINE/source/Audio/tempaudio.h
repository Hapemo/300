#pragma once
#include <fmod.hpp>		// Include [FMOD] Library
#include <iostream>
#include <map>
#include <filesystem>
#include "Input.h"

enum class AUDIO_TYPE :unsigned char
{
	BGM, SFX
};

struct AudioComponent
{
	AUDIO_TYPE type;
	std::string name;
};

class AudioManager
{
	std::unordered_map<AUDIO_TYPE, std::vector<FMOD::Channel*>> mChannels;
	std::unordered_map<AUDIO_TYPE, std::unordered_map<std::string, FMOD::Sound*>> mSounds;
	float sfxVolume;
	float bgmVolume;

public:
	void Init();
	void Update();
	void Exit();
	AudioManager();
	~AudioManager();

};