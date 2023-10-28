#pragma once
#include <fmod.hpp>		// Include [FMOD] Library
#include <iostream>
#include <map>
#include <filesystem>
#include <stdexcept>   // Added (9/25)
#include "Input/Input.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Debug/Logger.h"
#include "Debug/EnginePerformance.h"
#include "GameState/GameStateManager.h"
#include "Graphics/GraphicsSystem.h"
#include "Audio/AudioType.h"

enum AUDIOTYPE :unsigned char;

class AudioSystem
{

public:
	int MAX_AUDIO_FILES_PLAYING = 128;				                  // Number of Sounds (that are allowed to be played simultaneously)
	int NO_OF_BGM_CHANNELS_TO_INIT = 16;                              // we do not need so many channels for BGM
	int NO_OF_SFX_CHANNELS_TO_INIT = 112; 

public:
	AudioSystem() = default;
	~AudioSystem();

	// Core Loops
public:
	void Init();
	void Update(float dt);
	int  ErrCodeCheck(FMOD_RESULT result);																					// Debugging tool				

	// Loading Sounds
public:
	bool         LoadAudioFromDirectory(std::string directory_path);
	FMOD::Sound* FindSound(std::string audio_name);


	// Data Members (Audio System)
public:
	float sfx_global_vol = 1.0f;
	float bgm_global_vol = 1.0f;

	// Databases (Sounds + Channels) + FMOD System
public:
	FMOD::System* system_obj = nullptr;
	std::unordered_map<AUDIOTYPE, std::vector<FMOD::Channel*>>        mChannels;
	std::unordered_map<std::string, FMOD::Sound*>					  mSounds;
	//static std::unordered_map<
	/*std::unordered_map<AUDIOTYPE, std::vector*/

};