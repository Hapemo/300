#pragma once
#include <fmod.hpp>		// Include [FMOD] Library
#include <iostream>
#include <unordered_set>
#include <map>
#include <filesystem>
#include <stdexcept>   // Added (9/25)
#include <functional>
#include <algorithm>
#include "Input/Input.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Debug/Logger.h"
#include "Debug/EnginePerformance.h"
#include "GameState/GameStateManager.h"
#include "Graphics/GraphicsSystem.h"
#include "Audio/AudioType.h"
#include "FPSManager.h"


enum AUDIOTYPE :unsigned char;
enum STATE : unsigned char;


class AudioSystem
{
public:
	int MAX_AUDIO_FILES_PLAYING = 516;				                  // Number of Sounds (that are allowed to be played simultaneously)
	int NO_OF_BGM_CHANNELS_TO_INIT = 16;                              // we do not need so many channels for BGM
	int NO_OF_SFX_CHANNELS_TO_INIT = 500; 

public:
	AudioSystem() = default;
	~AudioSystem();

	// Core Loops
public:
	void Init();
	void Update(float dt, bool calling_from_pause);
	void TogglePause();
	void Reset();
	int  ErrCodeCheck(FMOD_RESULT result);												// Debugging tool				

	// Loading Sounds
public:
	bool         LoadAudioFromDirectory(std::string directory_path);
	FMOD::Sound* FindSound(std::string audio_name);

	// Playback Functions (Used Internally) - Update() Loop
public:
	unsigned int PlaySound(std::string audio_name, AUDIOTYPE type, float vol = 1.0f, Audio* audio_component = nullptr);	// Finds the next available in (specified sfx/bgm) channel and plays it in there. 
	bool		 PauseSound(uid channel_id, AUDIOTYPE type);
	bool		 ResumeSound(uid channel_id, AUDIOTYPE type);
	bool		 StopSound(uid channel_id, AUDIOTYPE type);
	bool		 UpdateVolume(uid channel_id, AUDIOTYPE type, float volume);
	void         SetAllSFXVolume(float volume);											// Built for Sound Settings
	void		 SetAllBGMVolume(float volume);											// Built for Sound Settings
	float		 GetSFXVolume() const;
	float		 GetBGMVolume() const;
	void		 PauseAllSounds();
	void		 UnpauseAllSounds();

	// Effects Playback
	// Audio Fade Functions 
	bool		 FadeIn(Entity id, float dt);    // Pass in the data from the <Audio> component 
	bool		 FadeOut(Entity id, float dt);   // Pass in the data from the <Audio> component 

	// Channel Management (used in every loop)
	void		 ClearFinishedSounds();


	// [3/13] Audio Panning
	void		 UpdatePanning(Audio* audio_component = nullptr);				        // Update Loop - go into channel and update panning


	// Scripting 
	bool		 AudioDonePlaying(Audio* audio_component);


	
public:
	// Channel Check (still playing or not)
public:
	bool         IsChannelPlaying(uid id, AUDIOTYPE type);
	bool		 IsChannelPaused(uid id, AUDIOTYPE type);
	bool		 IsUniqueAudioPlaying(std::string audio_name);   // Unique Audio check
	
	FMOD::Channel* GetChannelPointer(AUDIOTYPE audio_type, uid channel_id);
	void           NullChannelPointer(AUDIOTYPE audio_type, uid channel_id);


	// Data Members (Global Volume Multiplier)
public:
	bool  sys_paused = true;
	bool  sys_was_paused = false;
	float fade_timer = 0.0f;		// For Fading. 

	// Databases (Sounds + Channels) + FMOD System
public:
	FMOD::System* system_obj = nullptr;
	std::unordered_map<AUDIOTYPE, std::vector<std::pair<uid,FMOD::Channel*>>>     mChannels;
	std::unordered_map<std::string, FMOD::Sound*>							      mSounds;
	// std::unordered_set<std::string>						                          mSoundsCurrentlyPlaying;

	bool scene_switched = false;
	unsigned int playback_position;
	//static std::unordered_map< 
	/*std::unordered_map<AUDIOTYPE, std::vector*/

	// Test Case (will remove)
public:
	void TestCases(Audio& comp);
	void TestCaseEntity(Entity& entity, float dt); 

private: 
	float sfx_global_vol = 1.0f;
	float bgm_global_vol = 1.0f;
};