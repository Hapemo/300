#include "Audio/AudioSystem.h"


// Initialize a (single "AudioManager")
//std::unique_ptr<AudioManager> mAudio;
//std::unique_ptr<AudioListener> lAudio;

static bool once = false;

#pragma region Channel Class
//Channel::Channel(FMOD::Channel* new_channel, bool active) : channel(new_channel) ,
//{
//}

Channel::Channel() : isActive(false)
{
	FMOD::Channel* new_channel_obj = nullptr;
	channel = new_channel_obj;
}


#pragma endregion

// Audio System Life Cycle

#pragma region AudioSystem Core Loop
void AudioManager::Init()
{	
	// Create System
	result_code = FMOD::System_Create(&system);	 // [Initializes the Audio System] -> returns the system object to this class. (&system)'
	std::cout << "System Create: ";
	ErrCodeCheck(result_code);

	// Initialize System
	result_code = system->init(MAX_AUDIO_FILES_PLAYING, FMOD_INIT_NORMAL, nullptr);
	std::cout << "System Initialize: ";
	AudioManager::ErrCodeCheck(result_code);

	// Create Channels (at initialization)
	while (sfx_channel_no < NO_OF_SFX_CHANNELS_TO_INIT)
	{
		std::cout << "CREATING SFX CHANNEL #" << sfx_channel_no << "." << std::endl;
		
		Channel* new_channel = new Channel();

		mSFXChannels.insert(std::make_pair(sfx_channel_no, new_channel));
		sfx_channel_no++;
	}

	while (bgm_channel_no < NO_OF_BGM_CHANNELS_TO_INIT)
	{

		std::cout << "CREATING BGM CHANNEL #" << bgm_channel_no << "." << std::endl;

		Channel* new_channel = new Channel();

		mBGMChannels.insert(std::make_pair(bgm_channel_no, new_channel));

		bgm_channel_no++;
	}

	// Test Load Sound
	LoadAudioFile("../assets/Audio/farm_ambience.wav", "farm", AUDIO_TYPE::AUDIO_BGM);
	LoadAudioFile("../assets/Audio/NPC_Greeting.wav", "greeting", AUDIO_TYPE::AUDIO_BGM);
	// Test Spatial Audio
	LoadAudioFile("../assets/Audio/tuning-radio-7150.wav", "radio", AUDIO_TYPE::AUDIO_SFX, true, { 0.0f,0.0f,0.0f });
}

void AudioManager::Update()
{
	//std::cout << "AudioUpdate" << std::endl;
	if (!once)
	{
		FMOD_VECTOR vector{ 0.0f,0.0f,0.0f };
		PlayAudio("radio", AUDIO_TYPE::AUDIO_SFX, vector, 1.0f, 0);
		PlayAudio("farm", AUDIO_TYPE::AUDIO_BGM, vector, 1.0f, 0);
		//isPlaying(0);
		FMOD_VECTOR set_vector{ 1.0f, 0.0f, 0.0f };
		//SetChannel3DPosition(1, set_vector);
	/*	mAudio->channel->setVolume(0.5f);
		float volume = 0.0f;
		mAudio->channel->getVolume(&volume);*/
		//ERRGETCHECK(volume);

		//std::cout << "Current Volume is: " << volume << std::endl;

		once = true;
	}

	if (Input::CheckKey(PRESS, Q))
	{
		std::cout << "PRESSED Q." << std::endl;
		std::cout << "VOLUME SET : " << SetChannelVolume(AUDIO_SFX, 0, 0.0f);
	}

	if (Input::CheckKey(PRESS, W))
	{
		std::cout << "PRESSED W." << std::endl;
		SetAllVolume(1.0f);
	}

	if (Input::CheckKey(PRESS, E))
	{
		std::cout << "PRESSED E." << std::endl;
		SetBGMVolume(0.0f);
	}

	if (Input::CheckKey(PRESS, R))
	{
		std::cout << "PRESSED R." << std::endl;
		SetSFXVolume(0.0f);
	}
	//FMOD_VECTOR listener_pos = lAudio->GetListenerPosition();

	////std::cout << "Position : (" << listener_pos.x << "," << listener_pos.y << "," << listener_pos.z << ")" << std::endl;

	//lAudio->SetListenerPosition(listener_pos.x - 0.0005f, listener_pos.y - 0.0005f, listener_pos.z - 0.0005f);
	//lAudio->Update(); // [Important] - updates the volume 

	//float volume = 0.0f;
	//mAudio->channel->getVolume(&volume);

	//std::cout << "Current Volume is: " << volume << std::endl;

	//mAudio->system_obj->Update();
	

	//if (mAudio)
	//{
	//	mAudio->system->update(); // Updates (FMOD audio system)
	//	if (!once)
	//	{
	//		bool isPlaying;
	//		std::cout << mAudio->channel->isPlaying(&isPlaying) << std::endl;
	//		once = true;
	//		if (isPlaying)
	//			mAudio->channel->stop();
	//	}
	//	
	//}


	



	/*if (!once)
  {
	  mAudio->PlayBGMAudio("farm");
	  once = true;
  }

  if (!once2)
  {
	  mAudio->PlayBGMAudio("greeting");
	  once = true;
  }*/
}

void AudioManager::Exit()
{

}
#pragma endregion


#pragma region AudioManager
/*
*  AudioManager Helper Functions
*  -----------------------------------
*/ 

/*
   [Default Constructor]
   - Initializes the [FMOD::System]  
*/
AudioManager::AudioManager()
{
	//system_obj = new AudioSystem(); // Does the initialization for the system.
}

int AudioManager::ErrCodeCheck(FMOD_RESULT result)
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


		return 1; // failure
	}
	std::cout << "FMOD OPERATION OK." << std::endl;
	return 0; // success (no issues)
}

/*
   [Helper Function - Load Audio File]
   - Loads Audio File. 
   - [Arguments] : 1) File Path, 2) Name your Audio!, 3) Audio Type (enum) -> refer to header file.
*/
void AudioManager::LoadAudioFile(std::string audiofilePath, std::string audio_name, AUDIO_TYPE audio_type, bool spatial, FMOD_VECTOR audio_position, bool looping)
{
	Sound* new_sound = new Sound{ audio_position , nullptr }; // [1] Position , [2] FMOD::Sound * 

	// Mode Selector
	FMOD_MODE mode = FMOD_DEFAULT;							// [Normal Setting] - by default.
	mode |= spatial ? FMOD_3D : FMOD_2D;					// [3D Audio]       - checks for (spatial flag) -> user-defined arguement
	mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;     // [Looping]		- checks for (looping flag) -> user-defined arguement

	std::cout << "Creating Sound: " << audio_name << ": ";
	ErrCodeCheck(system->createSound(audiofilePath.c_str(), mode, 0, &new_sound->sound));

	if (&new_sound->sound) // if successfully created.
	{	
		switch (audio_type)
		{
		case AUDIO_BGM:
			mSoundsBGM[audio_name] = new_sound; // push into database.
			return;
		case AUDIO_SFX:
			mSoundsSFX[audio_name] = new_sound; // push into database.
			return;
		}
	}
}

/*
   [Play Audio]
   - Finds Requested Audio File
   - Plays at the specified position + volume.
*/
void AudioManager::PlayAudio(std::string audio_name, AUDIO_TYPE audio_type,FMOD_VECTOR audio_pos, float audio_vol, int channel_no)
{
	SoundMap* map_pointer = nullptr;

	switch (audio_type)
	{
	case AUDIO_BGM:
		map_pointer = &(mSoundsBGM);
		break;
	case AUDIO_SFX:
		map_pointer = &(mSoundsSFX);
		break;
	}

	SoundMap::iterator map_it = map_pointer->find(audio_name); // find the name.

	if (map_pointer)
	{
		if (map_it == map_pointer->end()) // if the audio name is found ...
		{
			std::cout << "Cannot Find Audio" << std::endl;
			return;
		}
	}

	ChannelMap::iterator channel_it;

	switch (audio_type)
	{
		case AUDIO_BGM:
			channel_it = mBGMChannels.find(channel_no);

			if (channel_it != mBGMChannels.end())	// if found...
			{
				std::cout << "BGM Channel is found." << std::endl;
				/*FMOD::Channel* found_channel = channel_it->second;*/
				//found_channel->setVolume(audio_vol);
				std::cout << "Now playing sound in channel: " << channel_no << std::endl;
				ErrCodeCheck(system->playSound(map_it->second->sound, nullptr, false, &(channel_it->second)->channel));
			}
			break;
		case AUDIO_SFX:
			channel_it = mSFXChannels.find(channel_no);

			if (channel_it != mSFXChannels.end())	// if found...
			{
				std::cout << "SFX Channel is found." << std::endl;
				/*FMOD::Channel* found_channel = channel_it->second;*/
				//found_channel->setVolume(audio_vol);
				std::cout << "Now playing sound in channel: " << channel_no << std::endl;
				ErrCodeCheck(system->playSound(map_it->second->sound, nullptr, false, &(channel_it->second->channel)));
			}
			break;
	}


	

	


	//FMOD::Channel* new_channel = nullptr;
	//ErrCodeCheck(mAudio->system_obj->system->playSound(map_it->second->sound, nullptr, true, &new_channel)); // Play in the new channel.

	//// For 3D Audios
	//if (new_channel) // if channel successfully utilized...
	//{
	//	FMOD_MODE audio_mode;
	//	map_it->second->sound->getMode(&audio_mode); // get the audio's setting. 

	//	if (audio_mode & FMOD_3D) // if the mode is [FMOD_3D]
	//	{
	//		result_code = new_channel->set3DAttributes(&audio_pos, nullptr); // Setting for 3D Position & velocity (for spatial audio)
	//		std::cout << "Setting 3D Attributes: ";
	//		ErrCodeCheck(result_code);
	//	}

	//	// Set the Volume of the sound.
	//	result_code = new_channel->setVolume(audio_vol);
	//	std::cout << "Setting Audio Volume: ";
	//	ErrCodeCheck(result_code);

	//	// Set back pause boolean. (play the audio after setting)
	//	result_code = new_channel->setPaused(false);
	//	std::cout << "Setting Pause Setting: ";
	//	ErrCodeCheck(result_code);
	//}

	//// Finally push it into the list of channels we have.
	//int& channel_size = mAudio->system_obj->channel_size;
	//channel_size = mAudio->system_obj->aChannels.size();
	//std::cout << "Number of Channels: " << channel_size << std::endl;
	//mAudio->system_obj->aChannels.insert(std::make_pair(channel_size + 1, new_channel));
	//channel_size++;
	//std::cout << "New Number of Channels: " << channel_size << std::endl;
}


//bool AudioManager::isPlaying(int channel_no)
//{
//	bool playing = false;
//
//	ChannelMap::iterator channel_it = mChannels.find(channel_no);
//
//	if (channel_it != mChannels.end())	// if found...
//	{
//		std::cout << "Channel is found." << std::endl;
//		channel_it->second->isPlaying(&playing);
//		return playing; 
//	}
//
//	return false;
//}

//void AudioManager::SetChannel3DPosition(int channel_id, FMOD_VECTOR audio_pos)
//{
//	AudioSystem::ChannelMap::iterator map_it = mAudio->system_obj->aChannels.find(channel_id); // Find using the channel ID.
//
//	// Does the Channel even exist?
//	if (map_it == mAudio->system_obj->aChannels.end())
//	{
//		std::cout << "Couldn't find the requested Channel. " << std::endl;
//		return;	// Return because no such channel.
//	}
//	
//	FMOD_VECTOR initial_pos;
//	FMOD_VECTOR final_pos;
//	result_code = map_it->second->get3DAttributes(&initial_pos, nullptr);
//	result_code = map_it->second->set3DAttributes(&audio_pos, nullptr);
//	std::cout << "3D Position for [Channel " << channel_id << "]: " << std::endl;
//	std::cout << "Setting Position [" << initial_pos.x << "," << initial_pos.y << "," << initial_pos.z << "]" << std::endl;
//	ErrCodeCheck(result_code);
//	
//	result_code = map_it->second->get3DAttributes(&final_pos, nullptr);
//	std::cout << "Final Position: [" << final_pos.x << "," << final_pos.y << "," << final_pos.z << "]" << std::endl;
//}

bool AudioManager::SetChannelVolume(AUDIO_TYPE audio_type, int channel_id, float channel_vol)
{	
	int max_channel_no;
	ChannelMap::iterator map_it;

	switch (audio_type)
	{
		case AUDIO_BGM:
			max_channel_no = NO_OF_BGM_CHANNELS_TO_INIT;
			map_it = mBGMChannels.find(channel_id);
			break;
		case AUDIO_SFX:
			max_channel_no = NO_OF_SFX_CHANNELS_TO_INIT;
			map_it = mSFXChannels.find(channel_id);
			break;
	}

	if (channel_id >= 0 && channel_id < max_channel_no)
	{	
		switch (audio_type)
		{
			case AUDIO_BGM: 
				// Does the Channel even exist?
				if (map_it == mBGMChannels.end())
				{
					std::cout << "Couldn't find the requested BGM Channel. " << std::endl;
					return 0;	// Return because no such channel.
				}

				else // found
				{
					if (map_it->second != nullptr)
						map_it->second->channel->setVolume(channel_vol);

				}

				break;

			case AUDIO_SFX:

				// Does the Channel even exist?
				if (map_it == mSFXChannels.end())
				{
					std::cout << "Couldn't find the requested SFX Channel. " << std::endl;
					return 0;	// Return because no such channel.
				}

				else // found
				{
					if (map_it->second != nullptr)
					{
						map_it->second->channel->setVolume(channel_vol);
						std::cout << "VOLUME CHANGED SUCCESSFULLY: " << channel_vol << std::endl;
					}
				

				

				}

				break;
		}
	}

	else
	{
		return 1;
	}
	
}

void AudioManager::SetAllVolume(float channel_vol)
{
	// Go through every [SFX] channel
	for (auto channel : mSFXChannels)
	{
		if (channel.second->channel)
			channel.second->channel->setVolume(channel_vol);
	}

	std::cout << "ADJUSTED ALL SFX CHANNELS: " << channel_vol << std::endl;

	for (auto channel : mBGMChannels)
	{
		if (channel.second->channel)
			channel.second->channel->setVolume(channel_vol);
	}

	std::cout << "ADJUSTED ALL BGM CHANNELS: " << channel_vol << std::endl;
}


void AudioManager::SetBGMVolume(float channel_vol)
{
	for (auto channel : mBGMChannels)
	{
		if(channel.second->channel)
			channel.second->channel->setVolume(channel_vol);
	}
}

void AudioManager::SetSFXVolume(float channel_vol)
{
	for (auto channel : mSFXChannels)
	{
		if (channel.second->channel)
			channel.second->channel->setVolume(channel_vol);
	}
}

///*
//	- Make sure that the bank exists in the database first.
//*/
//void AudioManager::LoadAudioBank(std::string bankfilepath, std::string bank_name)
//{
//	AudioSystem::BankMap::iterator map_it = mAudio->system_obj->aBank.find(bankfilepath);
//
//	// If [Bank] exist ...
//	if (map_it != mAudio->system_obj->aBank.end()) // Does not point to the end (found a bank that has the specified name)
//	{
//		std::cout << "Bank already exists" << std::endl; 
//		return; 
//	} // Requested [Bank] does not exist yet...
//
//	// Create [Bank] file and store it in system
//	FMOD::Studio::Bank* new_bank;
//	result_code = mAudio->system_obj->studio_system->loadBankFile(bankfilepath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &new_bank);
//	std::cout << "Creating New Bank: ";
//	ErrCodeCheck(result_code);
//
//	if (new_bank) // if new bank successfully created
//	{
//		mAudio->system_obj->aBank[bank_name] = new_bank; // Push in the new bank into database. 
//		return; 
//	}
//
//	return; 
//}

#pragma endregion

//#pragma region AudioSystem
//AudioSystem::AudioSystem()
//{
//	sys_result_code = FMOD::System_Create(&system);	 // [Initializes the Audio System] -> returns the system object to this class. (&system)'
//	std::cout << "System Create: ";
//	AudioManager::ErrCodeCheck(sys_result_code);
//
//	sys_result_code = system->init(MAX_AUDIO_FILES_PLAYING, FMOD_INIT_NORMAL, nullptr);
//	std::cout << "System Initialize: ";
//	AudioManager::ErrCodeCheck(sys_result_code);
//	
//	/*unsigned int version;
//	sys_result_code = system->getVersion(&version);
//	std::cout << "System Version Retrieve: ";
//	AudioManager::ErrCodeCheck(sys_result_code);*/
//
//	// [FMOD_STUDIO_LIVEUPDATE]   - Enables live update feature, allows for modifying & updating of sound assets (real-time)
//	// [FMOD_INIT_PROFILE_ENABLE] - 
//	sys_result_code = FMOD::Studio::System::create(&studio_system);
//	std::cout << "Studio System Create: ";
//	AudioManager::ErrCodeCheck(sys_result_code);
//
//	sys_result_code = studio_system->initialize(MAX_AUDIO_FILES_PLAYING, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL);
//	std::cout << "Studio System Initialize: ";
//	AudioManager::ErrCodeCheck(sys_result_code);
//}
//
//
///* 
//	- To manage audio channels
//	- Frees up channels, for use.
//	- Used to update event sounds also.
//*/
//
//void AudioSystem::Update()
//{
//	std::vector<ChannelMap::iterator> stop_this_channel;
//
//	ChannelMap::iterator channel_it = aChannels.begin();
//	ChannelMap::iterator channel_end_it = aChannels.end();
//
//	for (; channel_it != channel_end_it; channel_it++)
//	{
//		bool currently_playing = false;
//		channel_it->second->isPlaying(&currently_playing); // Checks if current channel is still playing an audio ...
//		if (!currently_playing) // if it's stopped playing ...
//		{
//			stop_this_channel.push_back(channel_it); // push back to my list of channels to delete.
//		}
//	}
//
//	for (auto& it : stop_this_channel)
//	{
//		aChannels.erase(it);
//	}
//
//	//std::cout << "Update FMOD Internal System: ";
//	//AudioManager::ErrCodeCheck(system->update()); // Update Event Sounds (FMOD System)
//}
//#pragma endregion

//#pragma AudioListener
//AudioListener::AudioListener(AudioSystem* audiosys) : audioSystem(audiosys)
//{
//	listener_pos = { 0.0f, 0.0f, 0.0f };
//}
//
//AudioListener::AudioListener(AudioSystem* audiosys, FMOD_VECTOR object_pos) : audioSystem(audiosys)
//{
//	listener_pos = object_pos;
//}
//
//void AudioListener::SetListenerPosition(float x, float y, float z)
//{
//	listener_pos = { x,y,z };
//}
//
//void AudioListener::SetListenerPosition(FMOD_VECTOR object_pos)
//{
//	listener_pos = object_pos;
//}
//
//FMOD_VECTOR AudioListener::GetListenerPosition() const
//{
//	return listener_pos; 
//}






#pragma endregion