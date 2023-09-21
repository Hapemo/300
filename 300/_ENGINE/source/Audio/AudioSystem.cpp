#include "Audio/AudioSystem.h"


AudioSystem::AudioSystem() : sfxVolume(1.0f), bgmVolume(1.0f)
{
	
}

AudioSystem::~AudioSystem()
{

}

void AudioSystem::Init()
{	
	//todo
	// load audio for loop
	// [Create the Audio System] -> returns the system object to this class. (&system)'
	std::cout << "System Create: ";
	ErrCodeCheck(FMOD::System_Create(&system_obj));

	// Initialize the System settings	
	std::cout << "System Initialize: ";
	ErrCodeCheck(system_obj->init(MAX_AUDIO_FILES_PLAYING, FMOD_INIT_NORMAL, nullptr)); // Settings can be combined by doing OR operation

	// Create Channels (At Initialization)
	std::vector<FMOD::Channel*> bgm_channel;
	std::vector<FMOD::Channel*> sfx_channel;
	std::cout << "Initialize BGM Channel." << std::endl;
	mChannels.emplace(std::make_pair(AUDIO_BGM, bgm_channel));
	std::cout << "Initialize SFX Channel." << std::endl;
	mChannels.emplace(std::make_pair(AUDIO_SFX, sfx_channel));

	// Populate Channel
	/*for (; no_of_sfx_channels < NO_OF_SFX_CHANNELS_TO_INIT; no_of_sfx_channels++)
	{
		std::cout << "CREATING SFX CHANNEL #" << no_of_sfx_channels << "." << std::endl;

		FMOD::Channel* new_channel = nullptr;
		mChannels.find(AUDIO_SFX)->second.push_back(new_channel);
		
	}

	for (; no_of_bgm_channels < NO_OF_BGM_CHANNELS_TO_INIT; no_of_bgm_channels++)
	{
		std::cout << "CREATING SFX CHANNEL #" << no_of_sfx_channels << "." << std::endl;

		FMOD::Channel* new_channel = nullptr;
		mChannels.find(AUDIO_BGM)->second.push_back(new_channel);
	}*/

	std::cout << "HI";

	//LoadAudioFiles("../assets/Audio");
	LoadAudioFromDirectory("../assets/Audio");

	//PlayAudio("tuning-radio-7150", AUDIO_SFX);
	//PlaySFXAudio("NPC_Greeting");
	//PlayBGMAudio("farm_ambience");
}

void AudioSystem::Update(float dt)
{
	if (Input::CheckKey(PRESS, _1))
	{
		PlaySFXAudio("NPC_Greeting");
	}

	if (Input::CheckKey(PRESS, _2))
	{
		PlayBGMAudio("farm_ambience");
	}

	if (Input::CheckKey(PRESS, _3))
	{
		PlaySFXAudio("tuning-radio-7150");
	}

	if (Input::CheckKey(PRESS, Q))
	{
		SetAllSFXVolume(0.0f);
	}

	if (Input::CheckKey(PRESS, W))
	{
		SetAllBGMVolume(0.0f);
	}

	if (Input::CheckKey(PRESS, R))
	{
		SetSpecificChannelVolume(AUDIO_SFX, 0, 0.0f);
	}

	if (Input::CheckKey(PRESS, T))
	{
		SetSpecificChannelVolume(AUDIO_SFX, 1, 0.0f);
	}

	if (Input::CheckKey(PRESS, A))
	{
		StopAllSFX();
	}

	if (Input::CheckKey(PRESS, S))
	{
		StopAllBGM();
	}

	if (Input::CheckKey(PRESS, F))
	{
		TogglePauseSFXSounds();
	}
	
	if (Input::CheckKey(PRESS, G))
	{
		TogglePauseBGMSounds();
	}

	if (Input::CheckKey(PRESS, H))
	{
		TogglePauseAllSounds();
	}

	if (Input::CheckKey(PRESS, J))
	{
		TogglePauseSpecific(AUDIO_SFX, 1);
		TogglePauseSpecific(AUDIO_SFX, 3);
	}





}

void AudioSystem::Exit()
{

}

// Use this whenever you use a [FMOD] Functiom
int AudioSystem::ErrCodeCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD OPERATION ERROR: " << result << std::endl;

		switch (result)
		{
		case FMOD_ERR_HEADER_MISMATCH:
			std::cout << "(20) [FMOD_ERR_HEADER_MISMATCH] : There is a version mismatch between the FMOD header and either the FMOD Studio library or the FMOD Low Level library." << std::endl;
			break;
		}

		return 0; // failure
	}
	std::cout << "FMOD OPERATION OK." << std::endl;
	return 1; // success (no issues)
}

void AudioSystem::LoadAudioFiles(std::filesystem::path file_path)
{
	std::string file_name = file_path.filename().string();
	std::cout << "File Name Requested: " << file_name << std::endl;

}

void AudioSystem::LoadAudioFromDirectory(std::filesystem::path directory_path)
{
	for (auto& file : std::filesystem::directory_iterator(directory_path))
	{
		std::filesystem::path file_path = file.path();
		std::cout << "File Detected: " << file_path.string() << std::endl;
		std::string audio_name = file_path.filename().string();
		size_t extension_pos = audio_name.find_last_of('.');
		audio_name = audio_name.substr(0, extension_pos);
		std::cout << "Audio Name: " << audio_name << std::endl;

		std::cout << "Creating Sound: ";
		FMOD::Sound* new_sound;
		ErrCodeCheck(system_obj->createSound(file_path.string().c_str(), FMOD_LOOP_NORMAL, 0, &new_sound));

		mSounds.insert(std::make_pair(audio_name, new_sound));
	}
}

void AudioSystem::PlayAudio(std::string audio_name, AUDIOTYPE audio_type, float audio_vol)
{
	
	// Find the sound first ...
	auto map_it = mSounds.find(audio_name); // tries to find the audio with this name...

	if (map_it == mSounds.end()) // true if not found;
	{
		std::cerr << "Can't find the requested audio." << std::endl;
	}

	auto channel_it = mChannels.find(audio_type);

	FMOD::Channel* new_channel = nullptr;

	int check = ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &new_channel));

	if (check)
	{
		new_channel->setVolume(audio_vol);
		channel_it->second.push_back(new_channel);
	}
}

void AudioSystem::PlaySFXAudio(std::string audio_name, float audio_vol)
{
	// Find the sound first ...
	auto map_it = mSounds.find(audio_name); // tries to find the audio with this name...

	if (map_it == mSounds.end()) // true if not found;
	{
		std::cerr << "Can't find the requested audio." << std::endl;
		return;
	}

	auto channel_it = mChannels.find(AUDIO_SFX);

	FMOD::Channel* new_channel = nullptr;
	ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &new_channel));
	new_channel->setVolume(audio_vol);
	channel_it->second.push_back(new_channel);
	
}

void AudioSystem::PlayBGMAudio(std::string audio_name, float audio_vol)
{
	// Find the sound first ...
	auto map_it = mSounds.find(audio_name); // tries to find the audio with this name...

	if (map_it == mSounds.end()) // true if not found;
	{
		std::cerr << "Can't find the requested audio." << std::endl;
		return;
	}

	auto channel_it = mChannels.find(AUDIO_BGM);

	FMOD::Channel* new_channel = nullptr;
	ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &new_channel));
	new_channel->setVolume(audio_vol);
	channel_it->second.push_back(new_channel);
}

void AudioSystem::SetAllSFXVolume(float audio_vol)
{
	auto channel_it = mChannels.find(AUDIO_SFX);
	std::cout << "Setting SFX Volume: ";

	for (FMOD::Channel* channel : channel_it->second)
	{
		ErrCodeCheck(channel->setVolume(audio_vol));
	}

	sfxVolume = audio_vol;
}

void AudioSystem::SetAllBGMVolume(float audio_vol)
{
	auto channel_it = mChannels.find(AUDIO_BGM);
	std::cout << "Setting BGM Volume: ";

	for (FMOD::Channel* channel : channel_it->second)
	{
		ErrCodeCheck(channel->setVolume(audio_vol));
	}

	bgmVolume = audio_vol;
}

void AudioSystem::SetSpecificChannelVolume(AUDIOTYPE audio_type, int channel_id, float audio_vol)
{	
	auto channel_it = mChannels.find(audio_type);

	if (channel_it != mChannels.end())
	{
		if (channel_id < channel_it->second.size())
		{
			FMOD::Channel** channelpp = nullptr;
			channelpp = &channel_it->second[channel_id];

			if (channelpp)
			{
				channel_it->second[channel_id]->setVolume(audio_vol);
			}
		}
	}

	else
	{
		std::cout << "Sorry, requested channel ID is invalid." << std::endl;
	}

	switch (audio_type)
	{
		case AUDIO_BGM:
			bgmVolume = audio_vol;
			return;
		case AUDIO_SFX:
			sfxVolume = audio_vol;
			return;
	}
}

void AudioSystem::MuteSFX()
{
	std::cout << "Muting Global SFX." << std::endl;
	SetAllSFXVolume(0.0f);
}

void AudioSystem::MuteBGM()
{
	std::cout << "Muting Global BGM." << std::endl;
	SetAllBGMVolume(0.0f);
}

void AudioSystem::StopAllSFX()
{
	std::cout << "Stopping and Releasing All SFX." << std::endl;

	auto channel_it = mChannels.find(AUDIO_SFX);

	for (FMOD::Channel* channel : channel_it->second)
	{
		channel->stop();
	}

	channel_it->second.erase(channel_it->second.begin(), channel_it->second.end());
}

void AudioSystem::StopAllBGM()
{
	std::cout << "Stopping and Releasing All BGM." << std::endl;

	auto channel_it = mChannels.find(AUDIO_BGM);

	for (FMOD::Channel* channel : channel_it->second)
	{
		channel->stop();
		channel = nullptr;
	}
}

void AudioSystem::TogglePauseAllSounds()
{
	auto channel_sfx_it = mChannels.find(AUDIO_SFX);

	for (FMOD::Channel* channel : channel_sfx_it->second)
	{
		bool paused;
		std::cout << "Checking Channel Pause Status: " << std::endl;
		ErrCodeCheck(channel->getPaused(&paused));

		if (paused)
			std::cout << "Resuming SFX Channels. " << std::endl;
		else
			std::cout << "Pausing SFX Channels. " << std::endl;

		ErrCodeCheck(channel->setPaused(!paused));
	}

	auto channel_bgm_it = mChannels.find(AUDIO_BGM);

	for (FMOD::Channel* channel : channel_bgm_it->second)
	{
		bool paused;
		std::cout << "Checking Channel Pause Status: " << std::endl;
		ErrCodeCheck(channel->getPaused(&paused));

		if (paused)
			std::cout << "Resuming SFX Channels. " << std::endl;
		else
			std::cout << "Pausing SFX Channels. " << std::endl;

		ErrCodeCheck(channel->setPaused(!paused));
	}
}

void AudioSystem::TogglePauseSFXSounds()
{
	auto channel_it = mChannels.find(AUDIO_SFX);
	
	for (FMOD::Channel* channel : channel_it->second)
	{
		bool paused;
		std::cout << "Checking Channel Pause Status: " << std::endl;
		ErrCodeCheck(channel->getPaused(&paused));

		if (paused)
			std::cout << "Resuming SFX Channels. " << std::endl;
		else
			std::cout << "Pausing SFX Channels. " << std::endl;

		ErrCodeCheck(channel->setPaused(!paused));
	}
}

void AudioSystem::TogglePauseBGMSounds()
{
	auto channel_it = mChannels.find(AUDIO_BGM);

	for (FMOD::Channel* channel : channel_it->second)
	{
		bool paused;
		std::cout << "Checking Channel Pause Status: " << std::endl;
		ErrCodeCheck(channel->getPaused(&paused));

		if (paused)
			std::cout << "Resuming SFX Channels. " << std::endl;
		else
			std::cout << "Pausing SFX Channels. " << std::endl;

		ErrCodeCheck(channel->setPaused(!paused));
	}
}

void AudioSystem::TogglePauseSpecific(AUDIOTYPE audio_type, int channel_id)
{
	auto channel_it = mChannels.find(audio_type);

	if (channel_it != mChannels.end())
	{
		if (channel_id < channel_it->second.size())
		{
			FMOD::Channel** channelpp = nullptr;

			channelpp = &channel_it->second[channel_id];

			if (channelpp)
			{
				bool paused;
				std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel_it->second[channel_id]->getPaused(&paused));

				if (paused)
					std::cout << "Resuming Selected Channel. " << std::endl;
				else
					std::cout << "Pausing Selected Channel. " << std::endl;

				ErrCodeCheck(channel_it->second[channel_id]->setPaused(!paused));
			}
		}

		else
		{
			std::cout << "Sorry, requested channel ID is invalid." << std::endl;
		}
	}
}

