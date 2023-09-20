#pragma once
#include <string>


enum class AUDIO_TYPE :unsigned char
{
	BGM, SFX
};

// 9/16/2023 - Basic Audio Component for Audio System.
struct SoundComponent
{
	std::string file_path;		// File Path to Load the Audio from
	AUDIO_TYPE  audio_type;		// Determines SFX or BGM
	bool		isLooping;		// Is the audio looping?
	bool		isPlaying;
	bool		isPlay;

	// bool is3D;
	// Transform a_transform	// Needed for positional audio
};

