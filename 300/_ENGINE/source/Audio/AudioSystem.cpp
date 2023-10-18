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
#include "Audio/AudioTest.h" // Scripting Tests (Audio into Lua)


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

	// Channel Initialization (SFX/BGM) - Global Volume Control
	std::vector<FMOD::Channel*> sfx_channel;
	std::vector<FMOD::Channel*> bgm_channel;

	std::vector<std::pair<uid, FMOD::Channel*&>> sfx_id_channel;
	std::vector<std::pair<uid, FMOD::Channel*&>> bgm_id_channel;

	mChannelsNewA.insert(std::make_pair(AUDIO_SFX, sfx_channel));    // without ID
	mChannelsNewA.insert(std::make_pair(AUDIO_BGM, bgm_channel));    // without ID
	mChannelswID.insert(std::make_pair(AUDIO_SFX, sfx_id_channel));  // w ID
	mChannelswID.insert(std::make_pair(AUDIO_BGM, bgm_id_channel));  // w ID

	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	if (audio_entities.size() > 0)
	{
		PINFO("Loading Audio Entities");
	}

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		if (!audio_component.mFilePath.empty() && !audio_component.mFileName.empty()) // Accounting for empty <Audio> components.
		{
			std::string audio_path = audio_component.mFilePath + "/" + audio_component.mFileName;
			std::string audio_name = audio_component.mFileName;

			if (LoadAudio(audio_path, audio_name)) // (1) Load Audio + (2) Add [Sound] into Database.
			{
				audio_component.mIsLoaded = true;
				audio_component.mSound = FindSound(audio_name); // Register [Sound] reference into <Audio> component. (if successfully loaded)
			}
			else
			{
				audio_component.mIsLoaded = false;
			}
		}

		FMOD::Channel*& channel_ref = audio_component.mChannel;
		std::pair<uid, FMOD::Channel*&> channel_pair = std::make_pair(audio_component.mChannelID, std::ref(channel_ref));

		// (3) Check [AudioType] + Register <Audio> component's Channel into [SFX] / [BGM] global channel
		switch (audio_component.mAudioType)
		{
			case AUDIO_BGM:
				mChannelsNewA[AUDIO_BGM].push_back(audio_component.mChannel);
				mChannelswID[AUDIO_BGM].push_back(channel_pair);
				break;
			case AUDIO_SFX:
				mChannelsNewA[AUDIO_SFX].push_back(audio_component.mChannel);
				mChannelswID[AUDIO_SFX].push_back(channel_pair);
				break;
		}
	}


}


/******************************************************************************/
/*!
	Update()w
	- Iterate through <Audio> Component. 
	- Decides whether if an audio is going to be played. 
 */
 /******************************************************************************/
void AudioSystem::Update([[maybe_unused]] float dt)
{
	// [10/14] Scripting Test
	if (Input::CheckKey(PRESS, Q))
		TestLoadPlaySuccess();
	if (Input::CheckKey(PRESS, W))
		TestLoadAudioDirectory();
	if (Input::CheckKey(PRESS, E))
		TestOverrideAttachSound();
	if (Input::CheckKey(PRESS, R))  // Press 'E' first.
		TestPauseSound();
	if (Input::CheckKey(PRESS, T))   // Press 'E' then 'R' then this
		TestUnpauseSound();


	// [10/18] Global Channel Test
	if (Input::CheckKey(PRESS, _1))
		SetAllSFXVolume(0.0f);
	
		

	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	/*
		[Check Play Loop]
		- Checks for (mIsPlay)    flag : signify that the audio is expected to play.
		- Checks for (mIsPlaying) flag : signify that it's already playing, so do not play again.
	*/
	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		if (audio_component.mIsLooping)
		{
			audio_component.mSound->setMode(FMOD_LOOP_NORMAL);
		}

		if (audio_component.mIsPlay &&							// (1) Check if this <Audio> is set to play.
			CheckAudioExist(audio_component.mFileName) &&		// (2) Check if the [Sound] that we want to play exists.
			!audio_component.mIsPlaying)						// (3) Check if the <Audio> is currently already playing...
		{
			PINFO("Audio Exist");
			PlayAudioSource(audio_component);
		}

		// Every Loop -> check if the <Audio> is still playing.
		bool channelIsPlay = false;
		if (audio_component.mIsPlaying) // need this to be true (to indicate that there's a sound playing in the channel)
		{
			audio_component.mChannel->isPlaying(&channelIsPlay);
		}

		// Pause Loop
		if (audio_component.mIsPaused)
		{
			audio_component.mChannel->setPaused(true);
		}

		// Resume Loop
		if (!audio_component.mIsPaused && audio_component.mWasPaused) // Only will run this when [Start] is triggered again.
		{
			audio_component.mChannel->setPaused(false);
			PINFO("RESUME");
			audio_component.mIsPaused = false;
			audio_component.mWasPaused = false;
		}



		if (audio_component.mIsPlaying && !channelIsPlay) // Sound finished playing in channel.
		{
			PINFO("Finished Playing");
			audio_component.mIsPlaying = false;	   // Audio finished playing. 
			audio_component.mIsPlay = false;       // Don't Need to keep playing... (play once) -> if "mIsLooping" is true (channel will continue to play)
		}

	}
	//for (Entity audio : audio_entities)
	//{
	//	Audio& audio_component = audio.GetComponent<Audio>();
	//
	//	if (audio_component.mIsPlay) // if it's supposed to play.
	//	{
	//		if (!audio_component.mIsPlaying)
	//		{
	//			int channel_id;
	//
	//			switch (audio_component.mAudioType)
	//			{
	//			case AUDIO_BGM:
	//				channel_id = PlayBGMAudio(audio_component.mFileName, 1.0f);
	//				if (channel_id != -1)
	//					audio_component.mPlayBGMChannelID.push_back(channel_id);
	//				break;
	//			case AUDIO_SFX:
	//				channel_id = PlaySFXAudio(audio_component.mFileName, 1.0f);
	//				if (channel_id != -1)
	//					audio_component.mPlaySFXChannelID.push_back(channel_id);
	//				break;
	//			}
	//		}
	//
	//		audio_component.mIsPlaying = true; // currently playing.
	//		audio_component.mIsPlay = false;   // don't need to play again.
	//	}
	//
	//	/*
	//		[Check if Channel is still Playing]
	//		- Iterate through [Audio Component]
	//
	//	*/
	//
	//	for (Entity audio1 : audio_entities)
	//	{
	//		Audio& audio_comp = audio1.GetComponent<Audio>();
	//
	//		for (int id : audio_comp.mPlayBGMChannelID)
	//		{
	//			Channel& channel = FindChannel(AUDIO_BGM, id);
	//
	//			channel.mChannel->isPlaying(&channel.mIsPlayingSound);
	//
	//			if (!channel.mIsPlayingSound) // channel stopped playing
	//			{
	//				audio_comp.mIsPlaying = false;
	//				audio_comp.mIsPlay = false;
	//			}
	//		}
	//
	//		for (int id : audio_comp.mPlaySFXChannelID)
	//		{
	//			Channel& channel = FindChannel(AUDIO_SFX, id);
	//
	//			channel.mChannel->isPlaying(&channel.mIsPlayingSound);
	//
	//			if (!channel.mIsPlayingSound) // channel stopped playing
	//			{
	//				audio_comp.mIsPlaying = false;
	//				audio_comp.mIsPlay = false;
	//			}
	//		}
	//	}
	//}
	
	system_obj->update();

}


/******************************************************************************/
/*!
	Play()
	- Checks for "mPlayonAwake" flag from <Audio> component. 
	- Plays the respective audio.
	- Linked to "Start" Button ("Resume" state)
 */
 /******************************************************************************/
void AudioSystem::PlayOnAwake()
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		if (audio_component.mPlayonAwake && audio_component.mIsPaused == false) // Truly the first time playing
		{
			PINFO("Playing %s on Awake", audio_component.mFileName.c_str());
			audio_component.mIsPlay = true;
		}

		if(audio_component.mIsPaused)
		{
			PINFO("Resuming Audio: %s.", audio_component.mFileName.c_str());
			ErrCodeCheck(audio_component.mChannel->setPaused(false));
			audio_component.mIsPaused = false;
		}
	}
}

/******************************************************************************/
/*!
	Pause()
	- Pauses all playing audio.
 */
 /******************************************************************************/
void AudioSystem::Pause()
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		bool playing = false; // by default.

		if (audio_component.mIsPlaying)
		{
			audio_component.mChannel->isPlaying(&playing);
		}

		if (playing) // only pause if it's playing.
		{
			PINFO("Pausing Audio : %s", audio_component.mFileName);  // I think loop too fast to display on debugger.
			ErrCodeCheck(audio_component.mChannel->setPaused(true));
			audio_component.mIsPaused = true;
			audio_component.mWasPaused = true;
		}
	}
}

void AudioSystem::Reset()
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		bool playing = false;

		if (audio_component.mIsPlaying)
		{
			audio_component.mChannel->isPlaying(&playing);
		}

		if (playing)
		{
			ErrCodeCheck(audio_component.mChannel->stop());
		}

		audio_component.ClearAudioComponent();
		//int i = 3;
	}
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
	[Note] : <Audio> Component must be attached to this for it to load properly.
 */
 /******************************************************************************/
void AudioSystem::UpdateLoadAudio(Entity id)
{	
	Audio& audio_component = id.GetComponent<Audio>();

	//audio_component.mFullPath = audio_component.mFilePath + "/" + audio_component.mFileName;

	if (!audio_component.mIsLoaded)
	{	
		std::string audio_path = audio_component.mFilePath + "/" + audio_component.mFileName;
		std::string audio_name = audio_component.mFileName;

		if (LoadAudio(audio_path, audio_name)) // What if the sound is loaded? (we have to save a reference in this function also)
		{
			audio_component.mIsLoaded = true;
			audio_component.mSound = FindSound(audio_name);  // Save a reference to the sound loaded in.
			PINFO("Audio Successfully Loaded :)");

		}
		else
		{
			audio_component.mIsLoaded = false;
			PWARNING("Audio could not be loaded... Please Check (1) Directory Name , (2) File");
		}
	}



	/*auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

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
	}*/
}

void AudioSystem::UpdateChannelReference(Entity id)
{
	Audio& audio_component = id.GetComponent<Audio>();
	
	AUDIOTYPE type = audio_component.mAudioType;

	if (type != AUDIO_NULL && audio_component.mTypeChanged) // Make sure it's a valid <Audio> audiotype
	{
		switch (type)
		{
			//case AUDIO_BGM:
			//	for (int i = 0; i < mChannelsNewA[AUDIO_BGM].size(); i++)
			//	{
			//		//if(mChannelsNewA[AUDIO_BGM])
			//	}
			//	mChannelsNewA[AUDIO_BGM];
			//case AUDIO_SFX:

		}
		mChannelsNewA[type].push_back(audio_component.mChannel); // Saves a reference of the <Audio>'s dedicated channel into the global database.
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
		//PINFO(" "); // concatanate nothing...

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

		PWARNING("Unregistered Error Code %d", result);
		return result; // failure
	}
	PINFO("FMOD OPERATION OK.");
	//std::cout << "FMOD OPERATION OK." << std::endl;
	return 1; // success (no issues)
}


/******************************************************************************/
/*!
	 LoadAudio()
	 --------------------------------------------------------------------------
	 /brief
		- Load audio file by taking in user-defined (1) file path, (2) audio name
		- Saves a reference of the [Sound] into the <Audio> component that loads it (Editor)
	 /param
		[1] std::string file_path       - file path leading to the audio name.
		[2] std::string audio_name      - name of the audio file.
		[3] Audio*		audio_component - Pointer reference to the <Audio> component.
	 /return
		bool : Success code of the loading of audio (if it's successful or not)
 */
 /******************************************************************************/
bool AudioSystem::LoadAudio(std::string file_path, std::string audio_name, Audio* audio_component)
{
	// Check if <Audio> is already in the database.
	auto sound_it = mSounds.find(audio_name);

	if (sound_it == mSounds.end()) // Audio not in the database.
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
		} // At this point, audio successfully loaded.

		mSounds.insert(std::make_pair(audio_name, new_sound));

		if (audio_component != nullptr) // Make sure there is an <Audio> component to save into.
		{
			audio_component->mSound = new_sound;  // Save [Sound Reference] into <Audio> component. 
		}

		return true;
	}

	else
	{
		return false;
	}
}



 bool AudioSystem::LoadAudioFromDirectory(std::filesystem::path directory_path)
{
	for (auto& file : std::filesystem::directory_iterator(directory_path))
	{
		std::filesystem::path file_path = file.path();
		PINFO("File Detected: %s", file_path.string());
		/*std::cout << "File Detected: " << file_path.string() << std::endl;*/
		std::string audio_name = file_path.filename().string();
		//size_t extension_pos = audio_name.find_last_of('.');
		//audio_name = audio_name.substr(0, extension_pos);
		PINFO("Audio Name: %s", audio_name);
		//std::cout << "Audio Name: " << audio_name << std::endl;

		PINFO("Creating Sound: ");
		//std::cout << "Creating Sound: ";
		FMOD::Sound* new_sound;
		bool check = ErrCodeCheck(system_obj->createSound(file_path.string().c_str(), FMOD_LOOP_OFF, 0, &new_sound));

		if (!check)
		{
			PWARNING("Failed to created Sound: %s", audio_name.c_str());
			return false;
		}

		mSounds.insert(std::make_pair(audio_name, new_sound));
	}

	return true;
}

/******************************************************************************/
/*!
	 PlayAudio()
	 - "Flip" the "mIsPlay" to true.
	 - AudioSystem::Update() will play once it's toggled.
 */
 /******************************************************************************/
void AudioSystem::PlayAudio(std::string audio_name, AUDIOTYPE audio_type, float audio_vol, Audio* audio_component)
{
	// Find the sound first ...
	auto map_it = mSounds.find(audio_name); // tries to find the audio with this name...

	if (map_it == mSounds.end()) // true if not found;
	{
		PWARNING("Can't find the requested audio.");
		return; // ends here if [Sound] cannot be found.
	}

	if (audio_component != nullptr)
	{
		PINFO("Flipping (mIsPlay)");
		audio_component->mIsPlay = true;
	}

	//else // giving an option to play sound let's say it's not tied to any <Audio> component.
	//{
	//	PINFO("Playing without Component");
	//	ErrCodeCheck(system_obj->playSound(map_it->second, nullptr, false, &audio_component->mChannel));
	//}
}

/******************************************************************************/
/*!
	 SetAllSFXVolume()
	 - Set every SFX Channel based on the provided (1) Audio Volume
 */
 /******************************************************************************/
void AudioSystem::SetAllSFXVolume(float audio_vol)
{
	auto channel_id_it = mChannelswID.find(AUDIO_SFX);
	
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	if (channel_id_it != mChannelswID.end())
	{
		for (auto& channel_id_pair : channel_id_it->second)
		{
			for (Entity e : audio_entities)
			{
				Audio& audio_component = e.GetComponent<Audio>();

				if (audio_component.mIsPlaying) // Only those channels which are playing can be set volume.
				{
					ErrCodeCheck(channel_id_pair.second->setVolume(audio_vol));
				}
			}
		}
	}

	sfxVolume = audio_vol;

	UpdateSFXComponentVolume(sfxVolume);
}

/******************************************************************************/
/*!
	 SetAllBGMVolume()
	 - Set every BGM Channel based on the provided (1) Audio Volume
 */
 /******************************************************************************/
void AudioSystem::SetAllBGMVolume(float audio_vol)
{
	auto channel_it = mChannelsNewA.find(AUDIO_BGM);
	PINFO("Setting BGM Volume: ");
	//std::cout << "Setting BGM Volume: ";

	if (channel_it != mChannelsNewA.end())
	{
		for (FMOD::Channel* channel : channel_it->second)
		{
			ErrCodeCheck(channel->setVolume(audio_vol));
		}
	}

	bgmVolume = audio_vol;

	UpdateBGMComponentVolume(bgmVolume);
}

void AudioSystem::UpdateSFXComponentVolume(float volume)
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();
	
	for (Entity e : audio_entities)
	{
		Audio& audio_components = e.GetComponent<Audio>();

		if(audio_components.mAudioType == AUDIO_SFX)
			audio_components.mVolume = volume;
	}
}

void AudioSystem::UpdateBGMComponentVolume(float volume)
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity e : audio_entities)
	{
		Audio& audio_components = e.GetComponent<Audio>();

		if (audio_components.mAudioType == AUDIO_BGM)
			audio_components.mVolume = volume;
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
	PINFO("Muting Global SFX.")
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

FMOD::Sound* AudioSystem::FindSound(std::string audio_name)
{
	auto sound_it = mSounds.find(audio_name);

	if (sound_it != mSounds.end())
	{
		return sound_it->second;
	}
	
}

bool AudioSystem::CheckAudioExist(std::string audio_name)
{
	auto sound_it = mSounds.find(audio_name);

	if (sound_it != mSounds.end())
	{
		if (sound_it->first == audio_name) // if the audio name is found in the database.
		{	
			return true;
		}
	}

	else
	{
		return false;
	}
}

/******************************************************************************/
/*!
	 [AudioSource Helper] - PlayAudioSource
	 - Funnel <Audio> component's attached [FMOD::Sound*] & [FMOD::Channel*]
 */
 /******************************************************************************/
void AudioSystem::PlayAudioSource(FMOD::Sound* comp_sound, FMOD::Channel* comp_channel)
{
	PINFO("Playing Audio Source...");
	ErrCodeCheck(system_obj->playSound(comp_sound, nullptr, false, &comp_channel));
}

void AudioSystem::PlayAudioSource(Audio& audio_component)
{
	PINFO("Playing Audio Source from <Audio> Component");
	ErrCodeCheck(system_obj->playSound(audio_component.mSound, nullptr, false, &audio_component.mChannel));

	bool isPlaying; // check whether if it's playing.
	ErrCodeCheck(audio_component.mChannel->isPlaying(&isPlaying));

	if (isPlaying)
	{
		audio_component.mIsPlaying = true;
	}
}



#pragma region TestFunctions 
/******************************************************************************/
/*!
	 Simulate <AudioSource> creation -> getting data from component. (scripting)
	 ---------------------------------------------------------------
	 Notes:
	 a. In Script, access to "Entity's ID" (stored in a variable (?)) - Ask Michelle (for step 2.)
	 ---------------------------------------------------------------
	 
	 [Use Case]
	 1. Create <AudioSource> object (empty)
	 2. Populate data from GetComponent<Audio> into the <AudioSource> object
	 3. 
	 
	 1. Create empty <AudioSource> object
	 2. Grab <Audio> Component, save pointer reference to it using "script_entity_id" (LUA)
	 
	 
	 AudioSource m_audio;                    // Empty AudioSource (interface for designers to do audio functionality)
	 Entity entity = new (script_entity_id); 
	 m_audio = GetComponent<Audio>();        // Populate <AudioSource> with data from <Audio>

 */
 /******************************************************************************/
//void AudioSystem::TestAudioSource()
//{
//	/*
//		Testing with a "dummy" object that has <Audio> & <Scripting> Component attached to it.
//		- Using "object_id" to access the entity that we will be working on (in the script)
//	*/
//	entt::entity object_id; 
//	auto audioent = systemManager->ecs->GetEntitiesWith<Audio>();
//
//	for (Entity ent : audioent) // Only has 1 entity with <Audio>
//	{
//		object_id = ent.id; 
//	}
//	//---------------------------------------------------------------------------------
//	
//
//	/*
//		Script Start
//	*/
//	//AudioSource m_audio;													// Create Empty [AudioSource] object
//	//m_audio.mAudioComponent = &(Entity(object_id).GetComponent<Audio>());	// Point to <Audio> Component (for it's data)
//	
//
//	
//	
//
//	//m_audio.mAudioComponent = 
//
//}

#pragma endregion