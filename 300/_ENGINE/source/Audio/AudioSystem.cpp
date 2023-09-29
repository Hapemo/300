/*!*************************************************************************
****
\file		   AudioSystem.cpp
\author(s)	   Cheong Ming Lun
\par DP email: m.cheong@digipen.edu
\date		   16-8-2023
\brief

This file contains the base AudioSystem class that supports the following functionalities:
- Loading in Audio Files (from components/directory)
- Manipulating Audio (Play/Pause/Stop)
****************************************************************************/
#include "Audio/AudioSystem.h"
#include "Debug/EnginePerformance.h"

/******************************************************************************/
/*!
	[Default Constructor] AudioSystem()
 */
 /******************************************************************************/
AudioSystem::AudioSystem() : sfxVolume(1.0f), bgmVolume(1.0f)
{
}

/******************************************************************************/
/*!
	[Destructor] ~AudioSystem()
 */
 /******************************************************************************/
AudioSystem::~AudioSystem()
{
}

/******************************************************************************/
/*!
	Init() 
	- Creates [FMOD System] 
	- Initializes [FMOD System]
	- Initalizing [Channels]
	- Populating Audio from "Entities" with <Audio> Component.
 */
 /******************************************************************************/
void AudioSystem::Init()
{
	// [Create the Audio System] -> returns the system object to this class. (&system)'
	// PINFO("FMOD System Create: %d", ErrCodeCheck(FMOD::System_Create(&system_obj)));
	PINFO("FMOD System Create: +");
	/*std::cout << "FMOD System Create:";*/
	ErrCodeCheck(FMOD::System_Create(&system_obj));

	// Initialize the System settings
	// PINFO("FMOD System Initialize: %d" , ErrCodeCheck(system_obj->init(MAX_AUDIO_FILES_PLAYING, FMOD_INIT_NORMAL, nullptr))); // Settings can be combined by doing OR operation)
	PINFO("FMOD System Initialize: +");
	ErrCodeCheck(system_obj->init(MAX_AUDIO_FILES_PLAYING, FMOD_INIT_NORMAL, nullptr));

	// Create Channels (At Initialization) - NEW (w ID tracking)
	std::vector<Channel> bgm_channel_new;
	std::vector<Channel> sfx_channel_new;
	PINFO("Initializing BGM Channels.");
	mChannelsNew.emplace(std::make_pair(AUDIO_BGM, bgm_channel_new));
	PINFO("Initializing SFX Channels.");
	mChannelsNew.emplace(std::make_pair(AUDIO_SFX, sfx_channel_new));

	// [9/25] Decided to just init a number of channels from the start.
	auto bgm_channel_it = mChannelsNew.find(AUDIO_BGM);
	auto sfx_channel_it = mChannelsNew.find(AUDIO_SFX);

	// [9/25] Populate the (Channel) Object
	PINFO("Populating BGM Channels.");
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

	PINFO("Populating BGM Channels.");
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


	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	if (audio_entities.size() > 0)
		PINFO("Loading Audio Entities");

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();
		std::string audio_path = audio_component.mFilePath + "/" + audio_component.mFileName;
		std::string audio_name = audio_component.mFileName;
		if (LoadAudio(audio_path, audio_name))
		{
			audio_component.mIsLoaded = true;
		}
		else
		{
			audio_component.mIsLoaded = false;
		}
	}
}


/******************************************************************************/
/*!
	Update()
	- Iterate through <Audio> Component. 
	- Decides whether if an audio is going to be played. 
 */
 /******************************************************************************/
void AudioSystem::Update([[maybe_unused]]float dt)
{
	EnginePerformance::StartTrack("Audio");
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	/*
		[Check Play Loop]
		- Checks for (mIsPlay)    flag : signify that the audio is expected to play.
		- Checks for (mIsPlaying) flag : signify that it's already playing, so do not play again.
	*/
	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		if (audio_component.mIsPlay) // if it's supposed to play.
		{
			if (!audio_component.mIsPlaying)
			{
				int channel_id;

				switch (audio_component.mAudioType)
				{
				case AUDIO_BGM:
					channel_id = PlayBGMAudio(audio_component.mFileName, 1.0f);
					if (channel_id != -1)
						audio_component.mPlayBGMChannelID.push_back(channel_id);
					break;
				case AUDIO_SFX:
					channel_id = PlaySFXAudio(audio_component.mFileName, 1.0f);
					if (channel_id != -1)
						audio_component.mPlaySFXChannelID.push_back(channel_id);
					break;
				}
			}

			audio_component.mIsPlaying = true; // currently playing.
			audio_component.mIsPlay = false;   // don't need to play again.
		}

		/*
			[Check if Channel is still Playing]
			- Iterate through [Audio Component]

		*/

		for (Entity audio1 : audio_entities)
		{
			Audio& audio_comp = audio1.GetComponent<Audio>();

			for (int id : audio_comp.mPlayBGMChannelID)
			{
				Channel& channel = FindChannel(AUDIO_BGM, id);

				channel.mChannel->isPlaying(&channel.mIsPlayingSound);

				if (!channel.mIsPlayingSound) // channel stopped playing
				{
					audio_comp.mIsPlaying = false;
					audio_comp.mIsPlay = false;
				}
			}

			for (int id : audio_comp.mPlaySFXChannelID)
			{
				Channel& channel = FindChannel(AUDIO_SFX, id);

				channel.mChannel->isPlaying(&channel.mIsPlayingSound);

				if (!channel.mIsPlayingSound) // channel stopped playing
				{
					audio_comp.mIsPlaying = false;
					audio_comp.mIsPlay = false;
				}
			}
		}
	}

	system_obj->update();

	EnginePerformance::EndTrack("Audio");
	EnginePerformance::UpdateSystemMs("Audio");
}

/******************************************************************************/
/*!
	Exit()
 */
 /******************************************************************************/
void AudioSystem::Exit()
{
}

/******************************************************************************/
/*!
	[Helper Function] UpdateLoadAudio()
	- Support for editor (when a new <Audio> component has been added to an "Entity"
 */
 /******************************************************************************/
void AudioSystem::UpdateLoadAudio()
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();
		if (!audio_component.mIsLoaded)
		{
			std::string audio_path = audio_component.mFilePath + "/" + audio_component.mFileName;
			std::string audio_name = audio_component.mFileName;
			if (LoadAudio(audio_path, audio_name))
			{
				audio_component.mIsLoaded = true;
				PINFO("Audio Successfully Loaded :)");

			}
			else
			{
				audio_component.mIsLoaded = false;
				PWARNING("Audio could not be loaded... Please Check (1) Directory Name , (2) File");
			}
		}
	}
}

/******************************************************************************/
/*!
	[Troubleshooter] ErrCodeCheck(FMOD_RESULT result)
	- Error Code Checker for "FMOD" related functions.
 */
 /******************************************************************************/
// Use this whenever you use a [FMOD] Functiom
int AudioSystem::ErrCodeCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		// PWARNING("FMOD OPERATION ERROR: %d", result);
		//std::cout << "FMOD OPERATION ERROR: " << result << std::endl;
		PINFO(" "); // concatanate nothing...

		switch (result)
		{
		case FMOD_ERR_FILE_NOTFOUND:
			PWARNING("(18) [FMOD_ERR_FILE_NOTFOUND] : The requested audio file path cannot be found, unsuccessfully loaded.");
			break;
		case FMOD_ERR_HEADER_MISMATCH:
			PWARNING("(20) [FMOD_ERR_HEADER_MISMATCH] : There is a version mismatch between the FMOD header and either the FMOD Studio library or the FMOD Low Level library.");
			//std::cout << "(20) [FMOD_ERR_HEADER_MISMATCH] : There is a version mismatch between the FMOD header and either the FMOD Studio library or the FMOD Low Level library." << std::endl;
			break;
		}

		return 0; // failure
	}
	PINFO("FMOD OPERATION OK.");
	//std::cout << "FMOD OPERATION OK." << std::endl;
	return 1; // success (no issues)
}

/******************************************************************************/
/*!
	 LoadAudio()
	 - Load Audio file based on the file path + audio name provided.
 */
 /******************************************************************************/
bool AudioSystem::LoadAudio(std::string file_path, std::string audio_name)
{
	PINFO("File Detected: %s", file_path.c_str());
	//std::cout << "File Detected: " << file_path << std::endl;
	PINFO("Creating Sound: +");
	/*std::cout << "Creating Sound: ";*/
	FMOD::Sound* new_sound;
	int check = ErrCodeCheck(system_obj->createSound(file_path.c_str(), FMOD_LOOP_OFF, 0, &new_sound));

	if (!check)
	{
		PWARNING("Error: Sound Not Loaded.");
		return 0;
		//std::cout << "Error: Sound Not Loaded." << std::endl;
	}


	mSounds.insert(std::make_pair(audio_name, new_sound));
	return true;
}

/******************************************************************************/
/*!
	 LoadAudioFiles()
	 - Iterate through provided file path and load every audio related files into the system.
 */
 /******************************************************************************/
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
		PINFO("File Detected: %s", file_path.string());
		/*std::cout << "File Detected: " << file_path.string() << std::endl;*/
		std::string audio_name = file_path.filename().string();
		size_t extension_pos = audio_name.find_last_of('.');
		audio_name = audio_name.substr(0, extension_pos);
		PINFO("Audio Name: %s", audio_name);
		//std::cout << "Audio Name: " << audio_name << std::endl;

		PINFO("Creating Sound: ");
		//std::cout << "Creating Sound: ";
		FMOD::Sound* new_sound;
		ErrCodeCheck(system_obj->createSound(file_path.string().c_str(), FMOD_LOOP_OFF, 0, &new_sound));

		mSounds.insert(std::make_pair(audio_name, new_sound));
	}
}

/******************************************************************************/
/*!
	 PlayAudio()
	 - Play Audio (user provide (1) Audio Name, (2) Audio Type , (3) Audio Volume
 */
 /******************************************************************************/
void AudioSystem::PlayAudio(std::string audio_name, AUDIOTYPE audio_type, float audio_vol)
{

	// Find the sound first ...
	auto map_it = mSounds.find(audio_name); // tries to find the audio with this name...

	if (map_it == mSounds.end()) // true if not found;
	{
		PWARNING("Can't find the requested audio.");
		//std::cerr << "Can't find the requested audio." << std::endl;
	}

	auto channel_it = mChannelsNew.find(audio_type);

	if (channel_it != mChannelsNew.end())
	{
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

/******************************************************************************/
/*!
	 PlaySFXAudio()
	 - Play Audio on a SFX Channel
	 - User provide (1) Audio Name (2) Audio Volume
 */
 /******************************************************************************/
int AudioSystem::PlaySFXAudio(std::string audio_name, float audio_vol)
{
	// Find the sound first ...
	auto map_it = mSounds.find(audio_name); // tries to find the audio with this name...

	if (map_it == mSounds.end()) // true if not found;
	{
		PWARNING("Can't find the requested audio.");
		//std::cerr << "Can't find the requested audio." << std::endl;
		return -1;
	}

	auto channel_it = mChannelsNew.find(AUDIO_SFX);

	if (channel_it != mChannelsNew.end())
	{

		for (Channel& channel : channel_it->second)
		{
			if (!channel.mIsPlayingSound)
			{
				int check_play = ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &(channel.mChannel)));

				if (check_play)
				{
					PINFO("Currently Playing (%s) on SFX Channel : #%d", audio_name.c_str(), channel.mChannelID);
					//std::cout << "Currently Playing (" << audio_name << ") on SFX Channel : #" << channel.mChannelID << std::endl;
					channel.mChannel->setVolume(audio_vol);
					channel.mIsPlayingSound = true;
					return channel.mChannelID;
					break;
				}
			}
		}
	}
	return -1;
}

/******************************************************************************/
/*!
	 PlayBGMAudio()
	 - Play Audio on a BGM Channel
	 - User provide (1) Audio Name (2) Audio Volume
 */
 /******************************************************************************/
int AudioSystem::PlayBGMAudio(std::string audio_name, float audio_vol)
{
	// Find the sound first ...
	auto map_it = mSounds.find(audio_name); // tries to find the audio with this name...

	if (map_it == mSounds.end()) // true if not found;
	{
		PWARNING("Can't find the requested audio.");
		//std::cerr << "Can't find the requested audio." << std::endl;
		return -1;
	}

	// auto channel_it = mChannels.find(AUDIO_BGM);
	auto channel_it = mChannelsNew.find(AUDIO_BGM);

	if (channel_it != mChannelsNew.end())
	{

		for (Channel& channel : channel_it->second)
		{
			if (!channel.mIsPlayingSound)
			{
				int check_play = ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &(channel.mChannel)));

				if (check_play)
				{
					PINFO("Currently Playing (%s) on BGM Channel : #%d", audio_name.c_str(), channel.mChannelID);
					//std::cout << "Currently Playing (" << audio_name << ") on BGM Channel : #" << channel.mChannelID << std::endl;
					channel.mChannel->setVolume(audio_vol);
					channel.mIsPlayingSound = true;
					return channel.mChannelID;
				}
			}
		}
	}
	return -1;
}

/******************************************************************************/
/*!
	 SetAllSFXVolume()
	 - Set every SFX Channel based on the provided (1) Audio Volume
 */
 /******************************************************************************/
void AudioSystem::SetAllSFXVolume(float audio_vol)
{
	auto channel_it = mChannelsNew.find(AUDIO_SFX);
	PINFO("Setting SFX Volume: ");
	//std::cout << "Setting SFX Volume: ";

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			ErrCodeCheck(channel.mChannel->setVolume(audio_vol));
		}
	}

	sfxVolume = audio_vol;
}

/******************************************************************************/
/*!
	 SetAllBGMVolume()
	 - Set every BGM Channel based on the provided (1) Audio Volume
 */
 /******************************************************************************/
void AudioSystem::SetAllBGMVolume(float audio_vol)
{
	auto channel_it = mChannelsNew.find(AUDIO_BGM);
	PINFO("Setting BGM Volume: ");
	//std::cout << "Setting BGM Volume: ";

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			ErrCodeCheck(channel.mChannel->setVolume(audio_vol));
		}
	}

	bgmVolume = audio_vol;
}

/******************************************************************************/
/*!
	 SetSpecificChannelVolume()
	 - Set volume for a specific audio channel.
 */
 /******************************************************************************/
void AudioSystem::SetSpecificChannelVolume(AUDIOTYPE audio_type, int channel_id, float audio_vol)
{
	auto channel_it_new = mChannelsNew.find(audio_type);

	if (channel_it_new != mChannelsNew.end())
	{
		if (channel_id < channel_it_new->second.size())
		{
			PINFO("CHANNEL REQUEST: %d", channel_id);
			//std::cout << "CHANNEL REQUEST: " << channel_id << std::endl;
			Channel& channel_found = FindChannel(audio_type, channel_id);
			PINFO("SETTING VOLUME:");
			//std::cout << "SETTING VOLUME:";
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
		PWARNING("Sorry, requested channel ID is invalid.");
		//std::cout << "Sorry, requested channel ID is invalid." << std::endl;
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

/******************************************************************************/
/*!
	 MuteSFX()
	 - Mute all SFX Channels
 */
 /******************************************************************************/
void AudioSystem::MuteSFX()
{
	PINFO("Muting Global SFX.");
	//std::cout << "Muting Global SFX." << std::endl;
	SetAllSFXVolume(0.0f);
}

/******************************************************************************/
/*!
	 MuteBGM()
	 - Mute all BGM Channels
 */
 /******************************************************************************/
void AudioSystem::MuteBGM()
{
	PINFO("Muting Global BGM.");
	//std::cout << "Muting Global BGM." << std::endl;
	SetAllBGMVolume(0.0f);
}

/******************************************************************************/
/*!
	 StopAllSFX()
	 - Stop all SFX Channels
 */
 /******************************************************************************/
void AudioSystem::StopAllSFX()
{
	PINFO("Stopping and Releasing All SFX.");
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

/******************************************************************************/
/*!
	 StopAllBGM()
	 - Stop All BGM Channels
 */
 /******************************************************************************/
void AudioSystem::StopAllBGM()
{
	PINFO("Stopping and Releasing All BGM.");
	//std::cout << "Stopping and Releasing All BGM." << std::endl;

	auto channel_it = mChannelsNew.find(AUDIO_BGM);

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			channel.mChannel->stop();
		}
	}
}

/******************************************************************************/
/*!
	 TogglePauseAllSounds()
	 - Pause if not Paused.
	 - Unpause if paused.
 */
 /******************************************************************************/
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
				PINFO("Checking Channel Pause Status: ");
				//std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel.mChannel->getPaused(&paused));

				if (paused)
				{
					PINFO("Resuming SFX Channels. ");
				}

				else
				{
					PINFO("Pausing SFX Channels. ");
				}


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
				PINFO("Checking Channel Pause Status: ");
				//std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel.mChannel->getPaused(&paused));

				if (paused)
				{
					PINFO("Resuming BGM Channels. ");
					//std::cout << "Resuming BGM Channels. " << std::endl;
				}

				else
				{
					PINFO("Pausing BGM Channels. ");
					//std::cout << "Pausing BGM Channels. " << std::endl;
				}

				ErrCodeCheck(channel.mChannel->setPaused(!paused));
			}
		}
	}
}

/******************************************************************************/
/*!
	 TogglePauseSFXSounds()
	 - Pause if not Paused.
	 - Unpause if paused.
 */
 /******************************************************************************/
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
				PINFO("Checking Channel Pause Status: ");
				//std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel.mChannel->getPaused(&paused));

				if (paused)
				{
					PINFO("Resuming SFX Channels. ");
					//std::cout << "Resuming SFX Channels. " << std::endl;
				}

				else
				{
					PINFO("Pausing SFX Channels. ");
					std::cout << "Pausing SFX Channels. " << std::endl;
				}

				ErrCodeCheck(channel.mChannel->setPaused(!paused));
			}
		}
	}
}

/******************************************************************************/
/*!
	 TogglePauseBGMSounds()
	 - Pause if not Paused.
	 - Unpause if paused.
 */
 /******************************************************************************/
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
				PINFO("Checking Channel Pause Status: +");
				//std::cout << "Checking Channel Pause Status: " << std::endl;
				ErrCodeCheck(channel.mChannel->getPaused(&paused));

				if (paused)
				{
					PINFO("Checking Channel Pause Status: +");
					//std::cout << "Resuming BGM Channels. " << std::endl;
				}

				else
				{
					PINFO("Pausing BGM Channels. +");
					//std::cout << "Pausing BGM Channels. " << std::endl;
				}


				ErrCodeCheck(channel.mChannel->setPaused(!paused));
			}
		}
	}
}

/******************************************************************************/
/*!
	 TogglePauseSpecific
	 - Pause if not Paused.
	 - Unpause if paused.
	 - Specific Channel
 */
 /******************************************************************************/
void AudioSystem::TogglePauseSpecific(AUDIOTYPE audio_type, int channel_id)
{
	auto channel_it = mChannelsNew.find(audio_type);

	if (channel_it != mChannelsNew.end())
	{
		if (channel_id < channel_it->second.size())
		{
			PINFO("CHANNEL REQUEST: %d", channel_id);
			//std::cout << "CHANNEL REQUEST: " << channel_id << std::endl;
			bool paused;
			PINFO("Checking Channel Pause Status: ");
			//std::cout << "Checking Channel Pause Status: " << std::endl;
			ErrCodeCheck(FindChannel(audio_type, channel_id).mChannel->getPaused(&paused));

			if (paused)
			{
				PINFO("Resuming Selected Channel. ");
				//std::cout << "Resuming Selected Channel. " << std::endl;
			}

			else
			{
				PINFO("Pausing Selected Channel. ");
				//std::cout << "Pausing Selected Channel. " << std::endl;
			}


			ErrCodeCheck(FindChannel(audio_type, channel_id).mChannel->setPaused(!paused));
		}
		else
		{
			PWARNING("Sorry, requested channel ID is invalid.");
			//std::cout << "Sorry, requested channel ID is invalid." << std::endl;
		}
	}
}

/******************************************************************************/
/*!
	 FindChannel()
	 - Return a reference to a channel based on user request
 */
 /******************************************************************************/
Channel& AudioSystem::FindChannel(AUDIOTYPE audio_type, int channel_id)
{
	if (channel_id > mChannelsNew.size())
	{
		PWARNING("Requested Index does not exists...");
		//ristd::cout << "Requested Index does not exists..." << std::endl;
		// throw std::out_of_range("Requested Index does not exists...");
	}

	auto channel_it = mChannelsNew.find(audio_type);

	if (channel_it != mChannelsNew.end())
	{
		for (Channel& channel : channel_it->second)
		{
			if (channel.mChannelID == (unsigned)channel_id)
			{
				return channel;
			}
		}
	}
	PERROR("channel not found, assigning default channel");
	return mChannelsNew[AUDIOTYPE::AUDIO_BGM][0];
}