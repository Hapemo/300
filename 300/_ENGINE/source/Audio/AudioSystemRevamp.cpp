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
	ErrCodeCheck(system_obj->init(MAX_AUDIO_FILES_PLAYING, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr)); // RIGHT HANDED coz OPENGL

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

	system_obj->set3DSettings(0.1f, 1.0f, 0.1f);
}

/******************************************************************************/
/*!
	Update(float d)
	- Iterate through <Audio> Component.
	- Decides whether if an audio is going to be played.
 */
 /******************************************************************************/
void AudioSystem::Update([[maybe_unused]] float dt, bool calling_from_pause)
{
	if(!sys_paused && !calling_from_pause)
		ClearFinishedSounds(); // only do this when the system is running

	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	auto listener_entity = systemManager->ecs->GetEntitiesWith<AudioListener>(); // There will only be '1' <AudioListenr> object

	/*auto transform_entities = systemManager->ecs->GetEntitiesWith<Transform>();

	for (Entity e : transform_entities)
	{
		Transform& trans = e.GetComponent<Transform>();
		General& gen = e.GetComponent<General>();

		if (gen.name == "gun")
			int hi = 4;

		int hi = 3;
	}*/
	// Listener Stuff
	if (listener_entity.size() == 1) // if there is no listeners. skip this portion
	{
		for (Entity e : listener_entity)
		{
			Transform& listener_trans = e.GetComponent<Transform>();
			//AudioListener& listener = e.GetComponent<AudioListener>();

			FMOD_VECTOR listener_pos{};
			FMOD_VECTOR velocity{};
			FMOD_VECTOR camera_forward{};
			FMOD_VECTOR up_vector{};

			// Update Listener Attributes
			listener_pos = { listener_trans.mTranslate.x, listener_trans.mTranslate.y, listener_trans.mTranslate.z };

			velocity.x = (listener_pos.x - listener_trans.mPreviousPosition.x) / dt;
			velocity.y = (listener_pos.y - listener_trans.mPreviousPosition.y) / dt;
			velocity.z = (listener_pos.z - listener_trans.mPreviousPosition.z) / dt;

			listener_trans.mPreviousPosition = listener_trans.mTranslate; // update the previous position once velocity is calculated.

			// Camera Forward Vector
			glm::vec3 cam_dir = systemManager->mGraphicsSystem.get()->GetCameraDirection(CAMERA_TYPE::CAMERA_TYPE_GAME);

			camera_forward = { cam_dir.x, cam_dir.y, cam_dir.z };

			// Upwards Vector
			up_vector = { 0.0f, 1.0f, 0.0f };

			// Updates Listener every loop
			system_obj->set3DListenerAttributes(0, &listener_pos, 0, &camera_forward, &up_vector);
		}
	}

	// Check <Audio> component "mState" + "mNextActionState" loop
	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();
		Transform& transform = audio.GetComponent<Transform>(); // need to update "mPreviousPosition" for 
		General& general = audio.GetComponent<General>();

		if (general.name == "DialogueObjectiveIntro1" && audio_component.mNextActionState == Audio::SET_TO_PLAY)
		{
			std::string temp = "";
		}
		// Set 3D Flag
		if (audio_component.mFileName.find("3D") != std::string::npos)
		{
			audio_component.m3DAudio = true;
		}

		// Update 3D Channel Attributes (moving objects)
		if (audio_component.m3DAudio)
		{
			FMOD::Channel* chn_ptr = GetChannelPointer(audio_component.mAudioType, audio_component.mChannelID);
			
			FMOD_VECTOR position = { transform.mTranslate.x , transform.mTranslate.y, transform.mTranslate.z };
			glm::vec3 glm_vel = (transform.mTranslate - transform.mPreviousPosition) / dt;
			FMOD_VECTOR velocity = { glm_vel.x, glm_vel.y, glm_vel.z };

			if (chn_ptr != nullptr) // Safeguard
			{
				chn_ptr->set3DAttributes(&position, &velocity);
			}

			// Update new previous position. (after calculations are done)
			transform.mPreviousPosition = transform.mTranslate;
		}


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

		if (Input::CheckKey(PRESS, P))
		{
			if (audio.GetComponent<Audio>().mFileName == "VO_Tutorial_ObjectiveIntro.wav") {
				std::string name = audio.GetComponent<General>().name;
			}

		}


		UpdateVolume(audio_component.mChannelID, audio_component.mAudioType, audio_component.mVolume * global_modifier);

		// Check for panning updates
		if (audio_component.mPanAudio)
		{
			UpdatePanning(&audio_component);
		}

		switch (audio_component.mNextActionState)
		{
			case Audio::SET_TO_PLAY:

			/*	if (audio_component.mFileName == "Player_Movement_Walk.wav")
				{
					int i = 0;
				}*/

				if (FindSound(audio_component.mFileName) != nullptr) // Sound Exists ...
				{
					/*if (audio_component.mIsUnique)
					{
						mSoundsCurrentlyPlaying.find()
					}*/
			
					if (audio.GetComponent<Audio>().mFileName == "VO_Tutorial_LoadIn.wav") {
						std::string name = audio.GetComponent<General>().name;
					}

					if (audio.GetComponent<Audio>().mFileName == "VO_Tutorial_ObjectiveIntro.wav") {
						std::string name = audio.GetComponent<General>().name;

					}

					PINFO("AUDIO EXISTS");
					PINFO("PLAYING AUDIO %s AT: %f", audio_component.mFileName.c_str(), audio_component.mVolume);

					if (audio_component.mFileName == "Player_BasicGun.wav")
						{
							int i = 0;
							std::cout << i;
						}

					unsigned int play_bool = PlaySound(audio_component.mFileName, audio_component.mAudioType, audio_component.mVolume, &audio_component); // audio_component is used to access [3D] data
					if (play_bool)  // Plays the sound based on parameters
					{
						audio_component.mState = Audio::PLAYING; // Update State 
						audio_component.mChannelID = play_bool;
						audio_component.mNextActionState = Audio::INACTIVE;
						audio_component.mCurrentlyPlaying = audio_component.mFileName; // for info display in editor.

						audio_component.mListOfChannelIDs.push_back(audio_component.mChannelID); // push an instance of the audio where the channel is being played
						audio_component.mJustPlayed = true;
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
					if (audio.GetComponent<Audio>().mFileName == "VO_Tutorial_LoadIn.wav") {
						std::string name = audio.GetComponent<General>().name;
					}

					if (audio.GetComponent<Audio>().mFileName == "VO_Tutorial_ObjectiveIntro.wav") {
						std::string name = audio.GetComponent<General>().name;
					}

					audio_component.mState = Audio::STOPPED;
					audio_component.mNextActionState = Audio::INACTIVE;
					//audio_component.mChannelID = uid();
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

		if (audio_component.mJustPlayed == false)
		{
			switch (audio_component.mState)
			{
			case Audio::INACTIVE:
				break;

			case Audio::PLAYING:
				// Check if the current sound has finished playing...

				// [11/20] Supporting multiple audio instances
				for (auto it = audio_component.mListOfChannelIDs.begin(); it != audio_component.mListOfChannelIDs.end();)
				{
					uid channel_id = *it;

					if (!IsChannelPlaying(channel_id, audio_component.mAudioType))
					{
						audio_component.mState = Audio::FINISHED;
						NullChannelPointer(audio_component.mAudioType, channel_id);
						PINFO("DONE PLAYING: %s", audio_component.mFileName);
						//std::cout << "DONE PLAYING" << std::endl;

						// Remove the finished channel ID from the vector
						it = audio_component.mListOfChannelIDs.erase(it);
						if (it != audio_component.mListOfChannelIDs.end() && it != audio_component.mListOfChannelIDs.begin())
						{
							--it; // Adjust the iterator to stay at the current position after erasing
						}
					}

					else
					{
						++it;
					}
				}
				break;

			case Audio::PAUSED:
				//if (sys_was_paused && !sys_paused) // Systme Unpaused + System was paused (pause button use case)
				//{
				//	audio_component.mNextActionState = Audio::SET_TO_RESUME;
				//}
				break;
			case Audio::STOPPED:
				break;

			case Audio::FINISHED:
				if (audio_component.mIsLooping)
				{
					audio_component.mNextActionState = Audio::SET_TO_PLAY;
				}
				else
				{
					audio_component.mState = Audio::INACTIVE;
					audio_component.mNextActionState = Audio::INACTIVE;
				}
				break;
			case Audio::FAILED:
				break;
			}
		}

		else
		{
			audio_component.mJustPlayed = false;
		}




	

	}

	

	system_obj->update();

	//sys_was_paused = false; 
}

void AudioSystem::TogglePause()
{
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();
	bool once = false;

	for (Entity audio : audio_entities)
	{
		Audio& audio_component = audio.GetComponent<Audio>();

		// Pause Loop
		if (audio_component.mState != Audio::INACTIVE)
		{
			if (audio_component.mState == Audio::PLAYING)
			{
				audio_component.mNextActionState = Audio::SET_TO_PAUSE; // will set the state to PAUSED ltr in update loop
			}
		}

		if (sys_paused)
		{
			if(audio_component.mState == Audio::PAUSED)
				audio_component.mNextActionState = Audio::SET_TO_RESUME;
		}
	}

	Update(static_cast<float>(FPSManager::dt), true); // Update (once to pause)

	/*switch (pause_state)
	{
		case Audio::PAUSED:
			PauseAllSounds();
			break;
		case Audio::PLAYING:
			UnpauseAllSounds();
			break;
	}*/

}


void AudioSystem::Reset()
{   
	auto audio_entities = systemManager->ecs->GetEntitiesWith<Audio>();

	//std::list<Audio*> persisting_audio;

	for (Entity audio : audio_entities)
	{
		Audio* audio_component = &audio.GetComponent<Audio>();

		if (!audio_component->mGameStateRetain) // only for those that won't survive till next state
		{
			audio_component->mState = Audio::STARTUP;		    // Set to intiail startup state.
			audio_component->mNextActionState = Audio::INACTIVE; // Set to do nothing (reset)
		}

		/*else
		{
			persisting_audio.push_back(audio_component);
		}*/
	}

	// Stop all channels (must do it here)
	for (auto& channel_pair : mChannels[AUDIO_BGM])
	{
		/*for (Audio* persist : persisting_audio)
		{*/
			//if (channel_pair.first == persist->mChannelID) // skip stopping this audio
			//{
			//	continue;
			//}

			/*else
			{*/
				FMOD::Sound* sound = nullptr;
				channel_pair.second->getCurrentSound(&sound);

				if (sound != nullptr)
				{   
					if (scene_switched == true)
					{
						channel_pair.second->getPosition(&playback_position, FMOD_TIMEUNIT_MS);
						//std::cout << "PLAYBACK: " << playback_position << std::endl;
					}

					channel_pair.second->stop();
					//channel_pair.second = nullptr;
				}
			//}
			
			
		//}
	}


	for (auto& channel_pair : mChannels[AUDIO_SFX])
	{
		FMOD::Sound* sound = nullptr;
		channel_pair.second->getCurrentSound(&sound);

		if (sound != nullptr)
		{
			channel_pair.second->stop();
			channel_pair.second = nullptr;
		}
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
	// 3D Audio support
	/*Transform& ent_transform = entity.GetComponent<Transform>();
	Audio& ent_audio = entity.GetComponent<Audio>();*/


	for (auto& channel : mChannels[type])
	{
		FMOD::Sound* current_sound;
		channel.second->getCurrentSound(&current_sound);

		FMOD::Sound* sound = FindSound(audio_name);

		if (!current_sound)
		{	

			if (audio_name == "VO_Tutorial_ObjectiveIntro.wav") {
				int i = 0;
			}

			system_obj->playSound(sound, 0, true, &channel.second);
			channel.second->setVolume(vol);

			if (audio_name == "dash.wav")
			{
				int i = 0;
			}

			if (audio_name == "Troubleshooter_M4.wav" && scene_switched == true)
			{
				//std::cout << "PLAYING BACK AT: " << playback_position << std::endl;
				channel.second->setPosition(playback_position, FMOD_TIMEUNIT_MS);
				//std::cout << "HEY" << std::endl;
				scene_switched = false;
			}

	

			if (audio_component->m3DAudio)
			{
				PINFO("SETTING 3D MIN MAX SETTINGS");
				channel.second->set3DMinMaxDistance(audio_component->mMinDistance, audio_component->mMaxDistance);
			}

			channel.second->setPaused(false);


			// [3D Sounds] - Intializing 3D audio information to FMOD.
			//if (audio_component->m3DAudio)
			//{
			//	FMOD_VECTOR position = { audio_component->mPosition.x ,audio_component->mPosition.y , audio_component->mPosition.z };
			//	FMOD_VECTOR velocity = { audio_component->mVelocity.x ,audio_component->mVelocity.y , audio_component->mVelocity.z };

			//	// All this can only be set after audio has played.
			//	PINFO("SETTING 3D ATTRIBUTES");
			//	channel.second->set3DAttributes(&position, &velocity);
			//	PINFO("SETTING 3D MIN MAX SETTINGS");
			//	channel.second->set3DMinMaxDistance(audio_component->mMinDistance, audio_component->mMaxDistance);
			//}


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

float AudioSystem::GetSFXVolume() const
{
	return sfx_global_vol;
}
float AudioSystem::GetBGMVolume() const
{
	return bgm_global_vol;
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

void AudioSystem::ClearFinishedSounds()
{
	system_obj->update(); 

	for (auto& channel_pair : mChannels[AUDIO_SFX])
	{
		FMOD::Sound* current_sound;
		channel_pair.second->getCurrentSound(&current_sound);

		if (current_sound)  // not empty..
		{
			bool playing = false;
			channel_pair.second->isPlaying(&playing);

			bool paused = false;
			channel_pair.second->getPaused(&paused);

			// Check if paused ... then we don't clear this channel
			if (paused)
			{
				continue;
			}

			// Passed "pause" check, now we check if it's playing , if it isn't we clear the channel for other audio to use.
			if (!playing)
			{
				channel_pair.second = nullptr;
			}
		}

	/*	else
		{

		}*/
	}

	for (auto& channel_pair : mChannels[AUDIO_BGM])
	{
		FMOD::Sound* current_sound;
		channel_pair.second->getCurrentSound(&current_sound);

		if (current_sound)  // not empty..
		{
			bool playing = false;
			channel_pair.second->isPlaying(&playing);

			bool paused = false;
			channel_pair.second->getPaused(&paused);

			// Check if paused ... then we don't clear this channel
			if (paused)
			{
				continue;
			}

			// Passed "pause" check, now we check if it's playing , if it isn't we clear the channel for other audio to use.
			if (!playing)
			{
				channel_pair.second = nullptr;
			}
		}
	}
}

void AudioSystem::UpdatePanning(Audio* audio_component)
{
	std::vector<std::pair<uid, FMOD::Channel*>> sfx_or_bgm = mChannels[audio_component->mAudioType];

	for (auto& channel_pair : sfx_or_bgm)
	{
		if (channel_pair.first == audio_component->mChannelID)
		{
			FMOD::Sound* current_sound;
			channel_pair.second->getCurrentSound(&current_sound);

			if (current_sound)  // not empty..
			{
				
				channel_pair.second->setPan(audio_component->mPanBalance); 
			}
		}
	}

	//switch (audio_component->mAudioType)
	//{
	//	case AUDIOTYPE::AUDIO_BGM:

	//	case AUDIOTYPE::AUDIO_SFX:
	//}
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

FMOD::Channel* AudioSystem::GetChannelPointer(AUDIOTYPE audio_type, uid channel_id)
{
	auto it = mChannels.find(audio_type);

	if (it != mChannels.end())
	{
		for (auto& channel_pair : it->second)
		{
			if (channel_pair.first == channel_id)
			{
				return channel_pair.second;
			}
		}
	}

	return nullptr;
}

void AudioSystem::NullChannelPointer(AUDIOTYPE audio_type, uid channel_id)
{
	auto it = mChannels.find(audio_type);

	if (it != mChannels.end())
	{
		for (auto& channel_pair : it->second)
		{
			if (channel_pair.first == channel_id)
			{
				channel_pair.second = nullptr;
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

	/*if (Input::CheckKey(PRESS, _0))
	{
		if (general.name == "Dash")
			audio_component.SetPlay(0.2f);
	}

	if (Input::CheckKey(PRESS, _1))
	{
		if (general.name == "Dash")
			audio_component.SetPlay(0.8f);
	}*/


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