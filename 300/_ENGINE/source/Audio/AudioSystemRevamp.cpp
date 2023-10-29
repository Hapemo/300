#include "Audio/AudioSystemRevamp.h"

AudioSystem::~AudioSystem() {}

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
	std::vector<std::pair<uid, FMOD::Channel*>> sfx_channels;
	std::vector<std::pair<uid, FMOD::Channel*>> bgm_channels;

	mChannels.insert(std::make_pair(AUDIO_SFX, sfx_channels));
	mChannels.insert(std::make_pair(AUDIO_BGM, bgm_channels));

	// Populate Channels
	for (int i = 0; i < NO_OF_SFX_CHANNELS_TO_INIT; ++i)
	{
		uid id;
		FMOD::Channel* new_channel;
		std::pair<uid, FMOD::Channel*> channel_pair = std::make_pair(id, new_channel);
		mChannels[AUDIO_SFX].push_back(channel_pair);
	}
	for (int i = 0; i < NO_OF_BGM_CHANNELS_TO_INIT; ++i)
	{
		uid id;
		FMOD::Channel* new_channel;
		std::pair<uid, FMOD::Channel*> channel_pair = std::make_pair(id, new_channel);
		mChannels[AUDIO_BGM].push_back(channel_pair);
	}

	// Load all Sounds from Directory... (at startup)
	LoadAudioFromDirectory("../assets\\Audio");


}

/******************************************************************************/
/*!
	Update(float d)
	- Iterate through <Audio> Component.
	- Decides whether if an audio is going to be played.
 */
 /******************************************************************************/
void AudioSystem::Update([[maybe_unused]] float dt)
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		if (pause_state) // was set to pause before resuming...
		{
			if (audio_component.mState == Audio::PAUSED)
				audio_component.mNextActionState = Audio::RESUME;
		}

		// On Awake Play
		if (audio_component.mPlayonAwake && (audio_component.mState != Audio::PLAYING) && (audio_component.mState != Audio::PAUSED) && (audio_component.mState != Audio::STOPPED))
		{
			audio_component.mNextActionState = Audio::SET_TO_PLAY;
		}

		// Test Cases - to test functionality (will remove)
		TestCases(audio_component);

		// Update Volume (every loop) - based on global volume also 
		float global_modifier = 1.0f;
		switch (audio_component.mAudioType)
		{
		case AUDIO_BGM:
			global_modifier = bgm_global_vol;
			break;
		case AUDIO_SFX:
			global_modifier = sfx_global_vol;
			break;
		}

		UpdateVolume(audio_component.mChannelID, audio_component.mAudioType, audio_component.mVolume * global_modifier);

		switch (audio_component.mNextActionState)
		{
		case Audio::SET_TO_PLAY:
			if (FindSound(audio_component.mFileName) != nullptr) // Sound Exists ...
			{
				PINFO("AUDIO EXISTS");
				PINFO("PLAYING AUDIO %s AT: %f", audio_component.mFileName,  audio_component.mVolume);

				unsigned int play_bool = PlaySound(audio_component.mFileName, audio_component.mAudioType, audio_component.mVolume);
				if (play_bool)  // Plays the sound based on parameters
				{
					audio_component.mState = Audio::PLAYING; // Update State 
					audio_component.mChannelID = play_bool;
					audio_component.mNextActionState = Audio::INACTIVE;
				}
				else
					audio_component.mState = Audio::FAILED;
			}

			break;

		case Audio::SET_TO_PAUSE:
			if (PauseSound(audio_component.mChannelID, audio_component.mAudioType))
			{
				audio_component.mState = Audio::PAUSED;
				audio_component.mNextActionState = Audio::INACTIVE;
			}
			break;

		case Audio::RESUME:
			if (ResumeSound(audio_component.mChannelID, audio_component.mAudioType))
			{
				audio_component.mState = Audio::PLAYING;
				audio_component.mNextActionState = Audio::INACTIVE;
			}

			else
			{
				audio_component.mState = Audio::FAILED;
			}

			break;

		case Audio::SET_STOP:
			if (StopSound(audio_component.mChannelID, audio_component.mAudioType))
			{
				audio_component.mState = Audio::STOPPED;
				audio_component.mNextActionState = Audio::INACTIVE;
			}

		}

		switch (audio_component.mState)
		{
		case Audio::INACTIVE:
			break;

		case Audio::PLAYING:
			// Check if the current sound has finished playing...
			if (!IsChannelPlaying(audio_component.mChannelID, audio_component.mAudioType))	// If finished playing...
			{
				audio_component.mState = Audio::FINISHED;
			}
			break;

		case Audio::PAUSED:
			
			break;
		case Audio::STOPPED:
			break;

		case Audio::FINISHED:
			if (audio_component.mIsLooping)
			{
				audio_component.mState = Audio::SET_TO_PLAY;
			}
			break;
		case Audio::FAILED:
			break;

		}
	}
}

void AudioSystem::Pause()
{
	pause_state = true;  // toggle back in update() loop when game loop resumes.

	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		audio_component.mState = Audio::PAUSED;

	}

	PauseAllSounds();
}

/******************************************************************************/
/*!
	LoadAudioFromDirectory()
	- Loads .wav + .mp3 files from the directory path provided (e.g. ../Assets/Audio)
	- Sound loaded will be populated in 'mSounds' (the database for all sounds available)
	- This will be used in the 'Init()' loop, where audio files are loaded at startup.
 */
 /******************************************************************************/
bool AudioSystem::LoadAudioFromDirectory(std::string directory_path)
{
	for (const auto& file : std::filesystem::directory_iterator(directory_path))
	{
		std::string audio_name = file.path().filename().string();

		if (!FindSound(audio_name))
		{
			PINFO("Audio Name: %s", audio_name.c_str());

			std::string file_path = file.path().string();
			PINFO("File Detected: %s", file_path.c_str());

			bool is3D = audio_name.find("3D") != std::string::npos; // Added [10/26] - label (3D) somewhere on audio file to load as a 3D audio

			FMOD_MODE mode = FMOD_DEFAULT;

			if (is3D)
			{
				mode = FMOD_3D;
			}


			PINFO("Creating Sound: ");
			FMOD::Sound* new_sound;
			bool check = ErrCodeCheck(system_obj->createSound(file_path.c_str(), mode, 0, &new_sound));


			if (!check)
			{
				PWARNING("Failed to create Sound: %s", audio_name.c_str());
				return false;
			}

			mSounds.insert(std::make_pair(audio_name, new_sound));
		}
	}

	return true;
}

/******************************************************************************/
/*!
	FindSound()
	- Goes through 'mSound' database and finds the user-specified audio sound. 
	- Returns a [FMOD::Sound*] , the reference to the specified audio sound. 
	- Returns a 'nullptr' if not found.

	- Used in "LoadAudioFromDirectory()"
 */
 /******************************************************************************/
FMOD::Sound* AudioSystem::FindSound(std::string audio_name)
{
	auto sound_it = mSounds.find(audio_name);

	if (sound_it != mSounds.end())
	{
		return sound_it->second;
	}

	return nullptr;

}

/******************************************************************************/
/*!
	SFXPlay()
	- Plays the audio file on a SFX channel.
	- Searches for "FMOD::Sound*" stored in "mSound" through given audio name (parameters)
	- Finds an available channel and plays that audio.
 */
 /******************************************************************************/
//void AudioSystem::SFXPlay(std::string audio_name, float vol)
//{
//	for (FMOD::Channel* channel : mChannels[AUDIO_SFX])
//	{
//		bool isPlaying;
//		channel->isPlaying(&isPlaying);
//
//		if (!isPlaying)
//		{
//			// This channel is free to play
//			// Play the sound here using FMOD::System and FMOD::Sound
//			FMOD::Sound* sound = FindSound(audio_name);
//			if (sound != nullptr) // Safeguard ...
//			{
//				system_obj->playSound(sound, 0, false, &channel);
//				channel->setVolume(vol);
//			}
//
//			return;
//		}
//	}
//}

/******************************************************************************/
/*!
	BGMPlay()
	- Plays the audio file on a BGM channel.
	- Searches for "FMOD::Sound*" stored in "mSound" through given audio name (parameters)
	- Finds an available channel and plays that audio.
 */
 /******************************************************************************/
//void AudioSystem::BGMPlay(std::string audio_name, float vol)
//{
//	for (FMOD::Channel* channel : mChannels[AUDIO_BGM])
//	{
//		bool isPlaying;
//		channel->isPlaying(&isPlaying);
//
//		if (!isPlaying)
//		{
//			// This channel is free to play
//			// Play the sound here using FMOD::System and FMOD::Sound
//			FMOD::Sound* sound = FindSound(audio_name);
//			if (sound != nullptr) // Safeguard ...
//			{
//				system_obj->playSound(sound, 0, false, &channel);
//				channel->setVolume(vol);
//			}
//
//			return;
//		}
//	}
//}

/******************************************************************************/
/*!
	PlaySound()
	- Plays the audio file on the user-specified channel (SFX / BGM)
	- Searches for "FMOD::Sound*" stored in "mSound" through given audio name (parameters)
	- Finds an available channel and plays that audio.
 */
 /******************************************************************************/
unsigned int AudioSystem::PlaySound(std::string audio_name, AUDIOTYPE type, float vol)
{
	for (auto& channel : mChannels[type])
	{
		FMOD::Sound* current_sound;
		channel.second->getCurrentSound(&current_sound);

		if (!current_sound)
		{
			// This channel is free to play
			// Play the sound here using FMOD::System and FMOD::Sound
			FMOD::Sound* sound = FindSound(audio_name);
			if (sound != nullptr) // Safeguard ...
			{
				system_obj->playSound(sound, 0, false, &channel.second);
				channel.second->setVolume(vol);
			}

			return (unsigned int)(channel.first);
		}
		
	}

	return 0; // failed.
}

/******************************************************************************/
/*!
	PauseSound()
	- Pauses the audio file on the user-specified channel (SFX / BGM)
	- Searches for "FMOD::Sound*" stored in "mSound" through given audio name (parameters)
	- Finds an available channel and plays that audio.
 */
 /******************************************************************************/
bool AudioSystem::PauseSound(uid channel_id, AUDIOTYPE type)
{
	for (auto& channel : mChannels[type])
	{
		if (channel.first == channel_id)
		{
			FMOD::Sound* current_sound;
			channel.second->getCurrentSound(&current_sound);

			if (current_sound)
			{
				bool playing = false;
				channel.second->isPlaying(&playing);

				if (playing)
				{
					channel.second->setPaused(true);
					return true;
				}
			}
		}
	}

	return false;
}

/******************************************************************************/
/*!
	UnpauseSound()
	- Unpauses the audio file on the user-specified channel (SFX / BGM)
	- Searches for "FMOD::Sound*" stored in "mSound" through given audio name (parameters)
	- Finds an available channel and plays that audio.
 */
 /******************************************************************************/
bool AudioSystem::ResumeSound(uid channel_id, AUDIOTYPE type)
{
	for (auto& channel : mChannels[type])
	{
		if (channel.first == channel_id)
		{
			FMOD::Sound* current_sound;
			channel.second->getCurrentSound(&current_sound);

			if (current_sound)
			{
				bool playing = false;
				channel.second->isPlaying(&playing);

				if (playing)
				{
					channel.second->setPaused(false);
					return true;
				}
			}
		}
	}

	return false;
}



/******************************************************************************/
/*!
	StopSound()
	- Stops the Sound (reset playback)
 */
 /******************************************************************************/
bool AudioSystem::StopSound(uid channel_id, AUDIOTYPE type)
{
	for (auto& channel : mChannels[type])
	{
		if (channel.first == channel_id)
		{
			channel.second->stop();
			return true;
		}
	}

	return false;
}

bool AudioSystem::UpdateVolume(uid channel_id, AUDIOTYPE type, float volume)
{
	for (auto& channel : mChannels[type])
	{
		if (channel.first == channel_id)
		{
			FMOD::Sound* current_sound;
			channel.second->getCurrentSound(&current_sound);

			if (current_sound)
			{
				bool playing = false;
				channel.second->isPlaying(&playing);

				if (playing)
				{
					channel.second->setVolume(volume);
					return true;
				}
			}
		}
	}

	return false;
}

void AudioSystem::SetAllSFXVolume(float volume)
{
	sfx_global_vol = volume;
}

void AudioSystem::SetAllBGMVolume(float volume)
{
	bgm_global_vol = volume;
}

void AudioSystem::PauseAllSounds()
{
	// Need to pause every channel.
	for (auto& channel_pair : mChannels[AUDIO_SFX])
	{
		FMOD::Sound* current_sound;
		channel_pair.second->getCurrentSound(&current_sound);

		if (current_sound)  // not empty..
		{
			bool playing = false;
			channel_pair.second->setPaused(true);
		}
	}

	for (auto& channel_pair : mChannels[AUDIO_BGM])
	{
		FMOD::Sound* current_sound;
		channel_pair.second->getCurrentSound(&current_sound);

		if (current_sound)  // not empty..
		{
			bool playing = false;
			channel_pair.second->setPaused(true);
		}
	}
}


void AudioSystem::UnpauseAllSounds()
{
	// Need to pause every channel.
	for (auto& channel_pair : mChannels[AUDIO_SFX])
	{
		FMOD::Sound* current_sound;
		channel_pair.second->getCurrentSound(&current_sound);

		if (current_sound)  // not empty..
		{
			bool playing = false;
			channel_pair.second->setPaused(false);
		}
	}

	for (auto& channel_pair : mChannels[AUDIO_BGM])
	{
		FMOD::Sound* current_sound;
		channel_pair.second->getCurrentSound(&current_sound);

		if (current_sound)  // not empty..
		{
			bool playing = false;
			channel_pair.second->setPaused(false);
		}
	}
}



bool AudioSystem::IsChannelPlaying(uid id, AUDIOTYPE type)
{
	std::vector<std::pair<uid, FMOD::Channel*>> sfx_or_bgm = mChannels[type];
	
	for (auto& channel_pair : sfx_or_bgm)
	{
		if (channel_pair.first == id)
		{
			FMOD::Sound* current_sound;
			channel_pair.second->getCurrentSound(&current_sound);
			
			if (current_sound)  // not empty..
			{
				bool playing = false;
				channel_pair.second->isPlaying(&playing);
				
				return playing;
			}
		}
	}

	return false;

}

bool AudioSystem::IsChannelPaused(uid id, AUDIOTYPE type)
{
	std::vector<std::pair<uid, FMOD::Channel*>> sfx_or_bgm = mChannels[type];

	for (auto& channel_pair : sfx_or_bgm)
	{
		if (channel_pair.first == id)
		{
			FMOD::Sound* current_sound;
			channel_pair.second->getCurrentSound(&current_sound);

			if (current_sound)  // not empty..
			{
				bool paused = false;
				channel_pair.second->getPaused(&paused);

				return paused;
			}
		}
	}

	return false;
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
		case FMOD_ERR_INVALID_HANDLE:
			PWARNING("(30) [FMOD_ERR_INVALID_HANDLE] : An invalid object has been passed into the FMOD function (check for nullptr)");
			break;
		}

		PWARNING("Unregistered Error Code %d", result);
		return result; // failure
	}
	PINFO("FMOD OPERATION OK.");
	//std::cout << "FMOD OPERATION OK." << std::endl;
	return 1; // success (no issue)
}

void AudioSystem::TestCases(Audio& audio_component)
{
	if (Input::CheckKey(PRESS, P))
		audio_component.SetPause();
	if (Input::CheckKey(PRESS, O))
		audio_component.SetResume();
	if (Input::CheckKey(PRESS, I))
		audio_component.SetStop();
	if (Input::CheckKey(PRESS, U))
		SetAllSFXVolume(0.0f);
	if (Input::CheckKey(PRESS, Y))
		SetAllSFXVolume(1.0f);
	if (Input::CheckKey(PRESS, L))
		SetAllBGMVolume(0.0f);
	if (Input::CheckKey(PRESS, K))
		SetAllBGMVolume(1.0f);
	/*if (Input::CheckKey(PRESS, J))*/
		
}