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
	sys_paused = false; // the fact that it goes in here means its unpaused

	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		//if (pause_state) // was set to pause before resuming...
		//{
		//	if (audio_component.mState == Audio::PAUSED)
		//		audio_component.mNextActionState = Audio::RESUME;
		//}

		// On Awake Play (Check once only)
		if (audio_component.mState == Audio::STARTUP)
		{
			if (audio_component.mPlayonAwake)
			{
				audio_component.mNextActionState = Audio::SET_TO_PLAY;
				audio_component.mState = Audio::INACTIVE;		// In case it didn't do any operations in the 'mNextActionState' loop
			}

			else // This audio doesn't need to play on awake...
			{
				audio_component.mState = Audio::INACTIVE;
			}
		}

		// Test Cases - to test functionality (will remove)
		TestCaseEntity(audio, dt);

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
					/*if (audio_component.mIsUnique)
					{
						mSoundsCurrentlyPlaying.find()
					}*/

					PINFO("AUDIO EXISTS");
					PINFO("PLAYING AUDIO %s AT: %f", audio_component.mFileName.c_str(), audio_component.mVolume);

					unsigned int play_bool = PlaySound(audio_component.mFileName, audio_component.mAudioType, audio_component.mVolume, &audio_component); // audio_component is used to access [3D] data
					if (play_bool)  // Plays the sound based on parameters
					{
						audio_component.mState = Audio::PLAYING; // Update State 
						audio_component.mChannelID = play_bool;
						audio_component.mNextActionState = Audio::INACTIVE;
						audio_component.mCurrentlyPlaying = audio_component.mFileName; // for info display in editor.

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

			case Audio::SET_TO_RESUME:
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
				break;
			
			case Audio::SET_FADE_IN:
				if (FadeIn(audio, dt)) // will keep running this every loop till it's done fading in.
				{
					audio_component.mNextActionState = Audio::INACTIVE;
				}

				break;
			case Audio::SET_FADE_OUT:
				if (FadeOut(audio, dt)) // will keep running this every loop till it's done fading in.
				{
					audio_component.mNextActionState = Audio::INACTIVE;
				}
				break;
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
			if (sys_was_paused && !sys_paused) // Systme Unpaused + System was paused (pause button use case)
			{
				audio_component.mNextActionState = Audio::SET_TO_RESUME;
			}
			break;
		case Audio::STOPPED:
			break;

		case Audio::FINISHED:
			if (audio_component.mIsLooping)
			{
				audio_component.mNextActionState = Audio::SET_TO_PLAY;
			}
			break;
		case Audio::FAILED:
			break;
		}
	}

	//sys_was_paused = false; 
}

void AudioSystem::Pause()
{
	sys_paused = true;
	sys_was_paused = true;
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		audio_component.mState = Audio::PAUSED;

	}

	PauseAllSounds();
}


void AudioSystem::Reset()
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		audio_component.mState = Audio::STARTUP;		    // Set to intiail startup state.
		audio_component.mNextActionState = Audio::INACTIVE; // Set to do nothing (reset)
	}

	// Stop all channels (must do it here)
	for (auto& channel_pair : mChannels[AUDIO_BGM])
	{
		FMOD::Sound* sound = nullptr;
		channel_pair.second->getCurrentSound(&sound);
		
		if(sound != nullptr)
		{
			channel_pair.second->stop();
			channel_pair.second = nullptr; 
		}
	}



	for (auto& channel_pair : mChannels[AUDIO_SFX])
	{

	}
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
unsigned int AudioSystem::PlaySound(std::string audio_name, AUDIOTYPE type, float vol, Audio* audio_component)
{

	for (auto& channel : mChannels[type])
	{
		FMOD::Sound* current_sound;
		channel.second->getCurrentSound(&current_sound);

		FMOD::Sound* sound = FindSound(audio_name);

		// Check if current sound is already playing (should be the same FMOD::Sound*) 
		// [channel.second] -> represents the channel this audio is going to be played in.
		if (current_sound == sound)
		{
			PINFO("ALREADY PLAYING");
			channel.second->setVolume(vol);
			system_obj->playSound(sound, 0, true, &channel.second);
			channel.second->setVolume(vol);
			channel.second->setPaused(false);


			// [3D Sounds] - Intializing 3D audio information to FMOD.
			if (audio_component->m3DAudio)
			{
				FMOD_VECTOR position = { audio_component->mPosition.x ,audio_component->mPosition.y , audio_component->mPosition.z };
				FMOD_VECTOR velocity = { audio_component->mVelocity.x ,audio_component->mVelocity.y , audio_component->mVelocity.z };

				// All this can only be set after audio has played.
				PINFO("SETTING 3D ATTRIBUTES");
				channel.second->set3DAttributes(&position, &velocity);
				PINFO("SETTING 3D MIN MAX SETTINGS");
				channel.second->set3DMinMaxDistance(audio_component->mMinDistance, audio_component->mMaxDistance);
			}

			return (unsigned int)(channel.first);

		}

		if (!current_sound)
		{
			// This channel is free to play
			// Play the sound here using FMOD::System and FMOD::Sound

			if (current_sound != sound) // Safeguard ... (make sure the same sound isn't being played twice)
			{
				system_obj->playSound(sound, 0, true, &channel.second);
				channel.second->setVolume(vol);
				channel.second->setPaused(false);
			}

			// [3D Sounds] - Intializing 3D audio information to FMOD.
			if (audio_component->m3DAudio)
			{
				FMOD_VECTOR position = { audio_component->mPosition.x ,audio_component->mPosition.y , audio_component->mPosition.z };
				FMOD_VECTOR velocity = { audio_component->mVelocity.x ,audio_component->mVelocity.y , audio_component->mVelocity.z };

				// All this can only be set after audio has played.
				PINFO("SETTING 3D ATTRIBUTES");
				channel.second->set3DAttributes(&position, &velocity);
				PINFO("SETTING 3D MIN MAX SETTINGS");
				channel.second->set3DMinMaxDistance(audio_component->mMinDistance, audio_component->mMaxDistance);
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

bool AudioSystem::FadeIn(Entity id, float dt)
{
	Audio& audio_component = id.GetComponent<Audio>();

	for (auto& channel_pair : mChannels[audio_component.mAudioType])
	{
		if (channel_pair.first == audio_component.mChannelID) // find the correct channel. (that this is playing on)
		{
			audio_component.fade_timer += dt;

			float fade_step = audio_component.mFadeInMaxVol / audio_component.fade_duration;

		    audio_component.mVolume += audio_component.mFadeSpeedModifier * (fade_step * dt);

			FMOD::Sound* current_sound;
			channel_pair.second->getCurrentSound(&current_sound);

			if (current_sound)
			{
				bool playing = false;
				channel_pair.second->isPlaying(&playing);

				if (playing)
				{
					if (audio_component.mVolume < audio_component.mFadeInMaxVol) // if haven't reach the fade in max volume. 
					{	
						channel_pair.second->setVolume(audio_component.mVolume);
						PINFO("Volume currently at: %f", audio_component.mVolume);
					}

					else // exceed the "mFadeInMaxVol" (ends here)
					{
						channel_pair.second->setVolume(audio_component.mFadeInMaxVol); 
						audio_component.mVolume = audio_component.mFadeInMaxVol;
						PINFO("DONE FADING IN.");
						fade_timer = 0.0f; // reset timer.
						return true;
					}
				}
					
			}
			
		}
	}

	return false;

}

bool AudioSystem::FadeOut(Entity id, float dt)
{
	Audio& audio_component = id.GetComponent<Audio>();

	for (auto& channel_pair : mChannels[audio_component.mAudioType])
	{
		if (channel_pair.first == audio_component.mChannelID) // find the correct channel. (that this is playing on)
		{
			FMOD::Sound* current_sound;
			channel_pair.second->getCurrentSound(&current_sound);

			if (current_sound)
			{
				bool playing = false;
				channel_pair.second->isPlaying(&playing);

				if (playing)
				{
					audio_component.fade_timer += dt;

					float fade_step = audio_component.mVolume / audio_component.fade_duration; // 1 fade step

					audio_component.mVolume -= audio_component.mFadeSpeedModifier * (fade_step * dt); // if mfadeSpeedModifier is faster it works. 

					if (audio_component.mVolume > audio_component.mFadeOutToVol) // if haven't reach the fade in max volume. 
					{
						channel_pair.second->setVolume(audio_component.mVolume);
						PINFO("Volume currently at: %f", audio_component.mVolume);
					}

					else // exceed the "mFadeInMaxVol" (ends here)
					{
						channel_pair.second->setVolume(audio_component.mFadeOutToVol);
						audio_component.mVolume = audio_component.mFadeOutToVol;
						PINFO("DONE FADING OUT.");
						fade_timer = 0.0f;
						return true;
					}
				}
			}

		}
	}

	return false;
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

// Checks with internal database
bool AudioSystem::IsUniqueAudioPlaying(std::string audio_name)
{
	//auto unique_it = mUniqueSoundRecord.find(audio_name);

	//if (unique_it != mUniqueSoundRecord.end())
	//{
	//	return false; // unique audio already playing. 
	//}

	//return true;
	return true;
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

void AudioSystem::TestCaseEntity(Entity& entity, float dt)
{
	Audio& audio_component = entity.GetComponent<Audio>();
	General& general = entity.GetComponent<General>();

	if (Input::CheckKey(PRESS, _0))
	{
		if (general.name == "Dash")
			audio_component.SetPlay(0.2f);
	}

	if (Input::CheckKey(PRESS, _1))
	{
		if (general.name == "Dash")
			audio_component.SetPlay(0.8f);
	}


	if (Input::CheckKey(PRESS, P))
	{
		if(general.name == "BGM")
			audio_component.SetPause();
	}

	if (Input::CheckKey(PRESS, O))
	{
		if (general.name == "BGM")
			audio_component.SetResume();
	}
	
	if (Input::CheckKey(PRESS, I))
	{
		if (general.name == "BGM")
			audio_component.SetStop();
	}

	if (Input::CheckKey(PRESS, U))
		SetAllSFXVolume(0.0f);
	if (Input::CheckKey(PRESS, Y))
		SetAllSFXVolume(1.0f);
	if (Input::CheckKey(PRESS, L))
		SetAllBGMVolume(0.0f);
	if (Input::CheckKey(PRESS, K))
		SetAllBGMVolume(1.0f);
	if (Input::CheckKey(PRESS, H))
	{
		if (general.name == "Test Fade BGM2")  // we only want to try to fade this audio.
		{
			audio_component.FadeIn(0.9f, 0.5f, 3.0f);  // vol to fade to, fade speed , duration to fade
		}
	}

	if (Input::CheckKey(PRESS, G))
	{
		if (general.name == "BGM")  // we only want to try to fade this audio.
		{
			audio_component.FadeOut(0.0f, 2.0f, 3.0f); // vol to fade to, fade speed , duration to fade
		}
	}
	
}