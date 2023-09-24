#pragma once

enum AUDIOTYPE :unsigned char
{
	AUDIO_BGM,
	AUDIO_SFX
};

// please update here
static std::unordered_map<std::string, AUDIOTYPE> AudioMap = { {"BGM", AUDIOTYPE::AUDIO_BGM},
															   {"SFX", AUDIOTYPE::AUDIO_SFX} };