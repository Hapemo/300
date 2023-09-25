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

	// Create Channels (At Initialization) - OLD
	std::vector<FMOD::Channel*> bgm_channel;
	std::vector<FMOD::Channel*> sfx_channel;
	std::cout << "Initialize BGM Channel." << std::endl;
	mChannels.emplace(std::make_pair(AUDIO_BGM, bgm_channel));
	std::cout << "Initialize SFX Channel." << std::endl;
	mChannels.emplace(std::make_pair(AUDIO_SFX, sfx_channel));

	// Create Channels (At Initialization) - NEW (w ID tracking)
	std::vector<Channel> bgm_channel_new;
	std::vector<Channel> sfx_channel_new;
	std::cout << "Initialize BGM NEW Channel." << std::endl;
	mChannelsNew.emplace(std::make_pair(AUDIO_BGM, bgm_channel_new));
	std::cout << "Initialize SFX NEW Channel." << std::endl;
	mChannelsNew.emplace(std::make_pair(AUDIO_SFX, sfx_channel_new));

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

	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();
		std::string audio_path = audio_component.mFilePath + "/" + audio_component.mFileName;
		std::string audio_name = audio_component.mFileName;
		LoadAudio(audio_path, audio_name);
	}

	// [9/25] Decided to just init a number of channels from the start.
	auto bgm_channel_it = mChannelsNew.find(AUDIO_BGM);
	auto sfx_channel_it = mChannelsNew.find(AUDIO_SFX);

	// [9/25] Populate the (Channel) Object
	if (bgm_channel_it != mChannelsNew.end())
	{
		for (int i = 0; i < NO_OF_BGM_CHANNELS_TO_INIT; ++i)
		{
			FMOD::Channel* new_channel = nullptr;
			Channel new_bgm_channel = Channel(next_avail_id_bgm, new_channel, false); // We need this for the channel status.
			next_avail_id_bgm++;
			bgm_channel_it->second.push_back(new_bgm_channel);
		}
	}

	if (sfx_channel_it != mChannelsNew.end())
	{
		for (int i = 0; i < NO_OF_SFX_CHANNELS_TO_INIT; ++i)
		{
			FMOD::Channel* new_channel = nullptr;
			Channel new_sfx_channel = Channel(next_avail_id_sfx, new_channel, false); // We need this for the channel status.
			next_avail_id_sfx++;
			sfx_channel_it->second.push_back(new_sfx_channel);
		}
	}
}

void AudioSystem::Update(float dt)
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();
		
		if (audio_component.mIsPlay) // if it's supposed to play.
		{
			switch (audio_component.mAudioType)
			{
				case AUDIO_BGM:
					PlayBGMAudio(audio_component.mFileName, 1.0f);
					break;
				case AUDIO_SFX:
					PlaySFXAudio(audio_component.mFileName, 1.0f); 
					break;
			}

			audio_component.mIsPlaying = true;  // currently playing. 
			audio_component.mIsPlay = false;	// don't need to play again.
		}
		
	}


	if (Input::CheckKey(PRESS, _1))
	{
		PlaySFXAudio("NPC_Greeting.wav");
	}

	if (Input::CheckKey(PRESS, _2))
	{
		PlayBGMAudio("farm_ambience.wav");
	}

	if (Input::CheckKey(PRESS, _3))
	{
		PlaySFXAudio("tuning-radio-7150.wav");
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
		SetSpecificChannelVolume(AUDIO_BGM, 0, 0.0f);
	}

	if (Input::CheckKey(PRESS, T))
	{
		SetSpecificChannelVolume(AUDIO_SFX, 0, 0.0f);
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
		TogglePauseSpecific(AUDIO_BGM, 0);
	}

	if (Input::CheckKey(PRESS, J))
	{
		TogglePauseAllSounds();
	}



	system_obj->update();
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

void AudioSystem::LoadAudio(std::string file_path, std::string audio_name)
{
	std::cout << "File Detected: " << file_path << std::endl;
	std::cout << "Creating Sound: ";
	FMOD::Sound* new_sound;
	int check = ErrCodeCheck(system_obj->createSound(file_path.c_str(), FMOD_LOOP_OFF, 0, &new_sound));

	if (!check)
	{
		std::cout << "Error: Sound Not Loaded." << std::endl;
	}

	mSounds.insert(std::make_pair(audio_name, new_sound));
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
		ErrCodeCheck(system_obj->createSound(file_path.string().c_str(), FMOD_LOOP_OFF, 0, &new_sound));

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

	auto channel_it = mChannelsNew.find(audio_type);

	if (channel_it != mChannelsNew.end())
	{
		bool found_channel = false;

		for (Channel& channel : channel_it->second)
		{
			if (!channel.mIsPlayingSound)
			{
				int check_play = ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &(channel.mChannel)));

				if (check_play)
				{
					channel.mChannel->setVolume(audio_vol);
					channel.mIsPlayingSound = true;
					break;
				}

			}
		}
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

	auto channel_it = mChannelsNew.find(AUDIO_SFX);

	if (channel_it != mChannelsNew.end())
	{
		bool found_channel = false;

		for (Channel& channel : channel_it->second)
		{
			if (!channel.mIsPlayingSound)
			{
				int check_play = ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &(channel.mChannel)));

				if (check_play)
				{
					std::cout << "Currently Playing (" << audio_name << ") on SFX Channel : #" << channel.mChannelID << std::endl;
					channel.mChannel->setVolume(audio_vol);
					channel.mIsPlayingSound = true;
					break;
				}

			}
		}
	}
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

	//auto channel_it = mChannels.find(AUDIO_BGM);
	auto channel_it = mChannelsNew.find(AUDIO_BGM);

	if (channel_it != mChannelsNew.end())
	{
		bool found_channel = false;

		for (Channel& channel : channel_it->second)
		{
			if (!channel.mIsPlayingSound)
			{
				int check_play = ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &(channel.mChannel)));

				if (check_play)
				{
					std::cout << "Currently Playing (" << audio_name << ") on BGM Channel : #" << channel.mChannelID << std::endl;
					channel.mChannel->setVolume(audio_vol); 
					channel.mIsPlayingSound = true;
					break;
				}

			}
		}
	}
}

void AudioSystem::SetAllSFXVolume(float audio_vol)
{
	auto channel_it = mChannelsNew.find(AUDIO_SFX);
	std::cout << "Setting SFX Volume: ";

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			ErrCodeCheck(channel.mChannel->setVolume(audio_vol));
		}
	}

	sfxVolume = audio_vol;
}

void AudioSystem::SetAllBGMVolume(float audio_vol)
{
	auto channel_it = mChannelsNew.find(AUDIO_BGM);
	std::cout << "Setting BGM Volume: ";

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			ErrCodeCheck(channel.mChannel->setVolume(audio_vol));
		}
	}

	bgmVolume = audio_vol;
}

void AudioSystem::SetSpecificChannelVolume(AUDIOTYPE audio_type, int channel_id, float audio_vol)
{	
	auto channel_it_new = mChannelsNew.find(audio_type);

	if (channel_it_new != mChannelsNew.end())
	{
		if (channel_id < channel_it_new->second.size())
		{
			std::cout << "CHANNEL REQUEST: " << channel_id << std::endl;
			Channel& channel_found = FindChannel(audio_type, channel_id);
			std::cout << "SETTING VOLUME:";
			ErrCodeCheck(channel_found.mChannel->setVolume(audio_vol));

		/*	if (channel_it_new->second[channel_id].mIsPlayingSound)
			{
				std::cout << "Attempting to Change Sound @ Channel 0." << std::endl;
				ErrCodeCheck(FindChannel(audio_type, channel_id).mChannel->setVolume(audio_vol));
			}*/
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

	auto channel_it = mChannelsNew.find(AUDIO_SFX);

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			channel.mChannel->stop();
		}
	}
}

void AudioSystem::StopAllBGM()
{
	std::cout << "Stopping and Releasing All BGM." << std::endl;

	auto channel_it = mChannelsNew.find(AUDIO_BGM);

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			channel.mChannel->stop();
		}
	}
}

void AudioSystem::TogglePauseAllSounds()
{
	auto channel_it_sfx = mChannelsNew.find(AUDIO_SFX);

	if (channel_it_sfx != mChannelsNew.end())
	{
		for (Channel& channel : channel_it_sfx->second)
		{
			if (channel.mIsPlayingSound)
			{
				bool paused;
				std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel.mChannel->getPaused(&paused));

				if (paused)
					std::cout << "Resuming SFX Channels. " << std::endl;
				else
					std::cout << "Pausing SFX Channels. " << std::endl;

				ErrCodeCheck(channel.mChannel->setPaused(!paused));
			}
		}
	}

	auto channel_it_bgm = mChannelsNew.find(AUDIO_BGM);

	if (channel_it_bgm != mChannelsNew.end())
	{
		for (Channel& channel : channel_it_bgm->second)
		{
			if (channel.mIsPlayingSound)
			{
				bool paused;
				std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel.mChannel->getPaused(&paused));

				if (paused)
					std::cout << "Resuming BGM Channels. " << std::endl;
				else
					std::cout << "Pausing BGM Channels. " << std::endl;

				ErrCodeCheck(channel.mChannel->setPaused(!paused));
			}
		}
	}
}

void AudioSystem::TogglePauseSFXSounds()
{
	auto channel_it_sfx = mChannelsNew.find(AUDIO_SFX);

	if (channel_it_sfx != mChannelsNew.end())
	{
		for (Channel& channel : channel_it_sfx->second)
		{
			if (channel.mIsPlayingSound)
			{
				bool paused;
				std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel.mChannel->getPaused(&paused));

				if (paused)
					std::cout << "Resuming SFX Channels. " << std::endl;
				else
					std::cout << "Pausing SFX Channels. " << std::endl;

				ErrCodeCheck(channel.mChannel->setPaused(!paused));
			}
		}
	}
}

void AudioSystem::TogglePauseBGMSounds()
{
	auto channel_it = mChannelsNew.find(AUDIO_BGM);

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			if (channel.mIsPlayingSound)
			{
				bool paused;
				std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel.mChannel->getPaused(&paused));

				if (paused)
					std::cout << "Resuming BGM Channels. " << std::endl;
				else
					std::cout << "Pausing BGM Channels. " << std::endl;

				ErrCodeCheck(channel.mChannel->setPaused(!paused));
			}
		}
	}
}

void AudioSystem::TogglePauseSpecific(AUDIOTYPE audio_type, int channel_id)
{
	auto channel_it = mChannelsNew.find(audio_type);

	if (channel_it != mChannelsNew.end())
	{
		if (channel_id < channel_it->second.size())
		{
			std::cout << "CHANNEL REQUEST: " << channel_id << std::endl;
			Channel& channel_found = FindChannel(audio_type, channel_id);
			bool paused;
			std::cout << "Checking Channel Pause Status: " << std::endl;
			ErrCodeCheck(FindChannel(audio_type, channel_id).mChannel->getPaused(&paused));

			if (paused)
				std::cout << "Resuming Selected Channel. " << std::endl;
			else
				std::cout << "Pausing Selected Channel. " << std::endl;

			ErrCodeCheck(FindChannel(audio_type, channel_id).mChannel->setPaused(!paused));
		}
		else
		{
			std::cout << "Sorry, requested channel ID is invalid." << std::endl;
		}
	}
}

Channel& AudioSystem::FindChannel(AUDIOTYPE audio_type, int channel_id)
{
	if (channel_id > mChannelsNew.size())
	{
		std::cout << "Requested Index does not exists..." << std::endl;
		//throw std::out_of_range("Requested Index does not exists...");
	}

	auto channel_it = mChannelsNew.find(audio_type);

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			if (channel.mChannelID == channel_id)
			{
				return channel;
			}
		}
	}
}