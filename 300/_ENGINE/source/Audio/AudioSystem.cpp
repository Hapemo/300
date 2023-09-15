#include "Audio/AudioSystem.h"


// Initialize a (single "AudioManager")
//std::unique_ptr<AudioManager> mAudio;
//std::unique_ptr<AudioListener> lAudio;

static bool once = false;

#pragma region <SoundInformation> Class
SoundInformation::SoundInformation(std::string afile_path, std::string asound_name, bool ais3D, bool aisLooping, AUDIO_TYPE audiotype) : file_path(afile_path), sound_name(asound_name), is3D(ais3D),
																																		 isLooping(aisLooping), isLoaded(false) , audio_type(audiotype)
{
	switch (audio_type)
	{
		case AUDIO_BGM:
			isBGM = true;
			isSFX = false;
			break;
		case AUDIO_SFX:
			isSFX = true;
			isBGM = false;
			break;
	}
}

bool SoundInformation::IsLoaded() const
{
	return isLoaded;
}

bool SoundInformation::Is3D() const
{
	return is3D;
}

bool SoundInformation::IsLooping() const
{
	return isLooping;
}

std::string SoundInformation::GetFilePath() const
{
	return file_path; 
}

bool SoundInformation::IsSFX() const
{
	return isSFX;
}

bool SoundInformation::IsBGM() const
{
	return isBGM;
}


void SoundInformation::SetIsLoaded(bool isloaded)
{
	isLoaded = isloaded;
}

void SoundInformation::SetIs3D(bool is3d)
{
	is3D = is3d;
}
void SoundInformation::SetIsLooping(bool islooping)
{
	isLooping = islooping;
}

void SoundInformation::SetFilePath(std::string filepath)
{
	file_path = filepath;
}

#pragma endregion

#pragma region <Sound> Class
Sound::Sound() : position({0.0f,0.0f,0.0f}) , sound(nullptr)
{

}

Sound::Sound(FMOD_VECTOR aPosition, FMOD::Sound* aSound) : position(aPosition) , sound(aSound)
{

}
#pragma endregion

#pragma region SOUND DATABASE THINGS
void AudioManager::AddSoundInfoDatabase(SoundInformation* sound_info)
{
	mSoundInfo.insert(std::make_pair(sound_info_uniqueID,sound_info));
	sound_info_uniqueID++;
}

void AudioManager::PopulateSoundInfoDatabase(std::vector<SoundInformation*> sound_info_database)
{
	for (SoundInformation* sound_info : sound_info_database)
	{
		mSoundInfo.insert(std::make_pair(sound_info_uniqueID, sound_info));
		sound_info_uniqueID++;
	}
}
#pragma endregion

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
	ErrCodeCheck(result_code);
	}

	while (bgm_channel_no < NO_OF_BGM_CHANNELS_TO_INIT)
	{

		std::cout << "CREATING BGM CHANNEL #" << bgm_channel_no << "." << std::endl;

		Channel* new_channel = new Channel();

		mBGMChannels.insert(std::make_pair(bgm_channel_no, new_channel));

		bgm_channel_no++;
	}

	// [9/13/2023] - Integrating <SoundInfo> into [AudioSystem] 
	// * in the future de-serialize
	SoundInformation* sound_1 = new SoundInformation("../assets/Audio/farm_ambience.wav", "farm", false, false, AUDIO_BGM);
	SoundInformation* sound_2 = new SoundInformation("../assets/Audio/NPC_Greeting.wav", "npc", false, false, AUDIO_SFX); 
	SoundInformation* sound_3 = new SoundInformation("../assets/Audio/tuning-radio-7150.wav", "radio", false, false, AUDIO_SFX);

	AddSoundInfoDatabase(sound_1);
	AddSoundInfoDatabase(sound_2); 
	AddSoundInfoDatabase(sound_3);

	LoadAudioDatabase();

	// Test Load Sound
	//LoadAudioFile("../assets/Audio/farm_ambience.wav", "farm", AUDIO_TYPE::AUDIO_BGM);
	//LoadAudioFile("../assets/Audio/NPC_Greeting.wav", "greeting", AUDIO_TYPE::AUDIO_BGM);
	//// Test Spatial Audio
	//LoadAudioFile("../assets/Audio/tuning-radio-7150.wav", "radio", AUDIO_TYPE::AUDIO_SFX, true, { 0.0f,0.0f,0.0f });
}

void AudioManager::Update(float dt)
{
	dt = dt + 1; // just to use the argument only.

	//std::cout << "AudioUpdate" << std::endl;
	if (!once)
	{
		FMOD_VECTOR vector{ 0.0f,0.0f,0.0f };
		//PlayAudio("radio", AUDIO_TYPE::AUDIO_SFX, vector, 1.0f, 0);
		// PlayAudio("farm", AUDIO_TYPE::AUDIO_BGM, vector, 1.0f, 0);

		PlaySFXAudio("radio", 1.0f, 0);
		PlayBGMAudio("farm", 1.0f, 1);
		PlayBGMAudio("greeting", 1.0f, 2);
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

	if (Input::CheckKey(PRESS, S))
	{
		std::cout << "PRESSED S." << std::endl;
		StopAllSounds();
	}

	if (Input::CheckKey(PRESS, D))
	{
		std::cout << "PRESSED D." << std::endl;
		StopAllBGMSounds();
	}

	if (Input::CheckKey(PRESS, F))
	{
		std::cout << "PRESSED F." << std::endl;
		StopAllSFXSounds();
	}

	if (Input::CheckKey(PRESS, _0))
	{
		std::cout << "PRESSED 0." << std::endl;
		StopChannel(AUDIO_SFX, 0);
	}

	if (Input::CheckKey(PRESS, _1))
	{
		std::cout << "PRESSED 1." << std::endl;
		StopChannel(AUDIO_BGM, 1);
	}

	if (Input::CheckKey(PRESS, _2))
	{
		std::cout << "PRESSED 2." << std::endl;
		StopChannel(AUDIO_BGM, 2);
	}

	if (Input::CheckKey(PRESS, _3))
	{
		std::cout << "PRESSED 3." << std::endl;
		PauseAllSounds();
	}
	
	if (Input::CheckKey(PRESS, _4))
	{
		std::cout << "PRESSED 4." << std::endl;
		PauseSFXChannel(0);
	}

	if (Input::CheckKey(PRESS, _5))
	{
		std::cout << "PRESSED 5." << std::endl;
		PauseBGMChannel(1);
	}

	if (Input::CheckKey(PRESS, _6))
	{
		std::cout << "PRESSED 6." << std::endl;
		ResumeAllSounds();
	}

	if (Input::CheckKey(PRESS, _7))
	{
		std::cout << "PRESSED 7." << std::endl;
		ResumeSFXChannel(0);
	}

	if (Input::CheckKey(PRESS, _8))
	{
		std::cout << "PRESSED 8." << std::endl;
		ResumeBGMChannel(1);
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


#pragma region AudioManager Core Functions
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
	// [TODO] - Update [mSoundInfo] database. -> if successful

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

void AudioManager::LoadAudioDatabase()
{
	std::cout << "Loading Audio from [SoundInfo Database] into the FMOD Core System ..." << std::endl;

	for (auto sound_info : mSoundInfo)
	{
		// Load File Path
		if (!sound_info.second->IsLoaded()) // if this sound file has not loaded ...
		{ 
			// Mode Selector  
			FMOD_MODE mode = FMOD_DEFAULT;												   // [Normal Setting] - by default.
			mode |= sound_info.second->Is3D() ? FMOD_3D : FMOD_2D;					       // [3D Audio]       - checks for (spatial flag) -> user-defined arguement
			mode |= sound_info.second->IsLooping() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;     // [Looping]		- checks for (looping flag) -> user-defined arguement

			FMOD_VECTOR audio_pos = { 0.0f,0.0f,0.0f };
			Sound* new_sound = new Sound{audio_pos , nullptr };

			std::string audio_file_path = sound_info.second->GetFilePath();
			bool check = ErrCodeCheck(system->createSound(sound_info.second->sound_name.c_str(), mode, 0, &new_sound->sound));

			if (check)
			{
				switch (sound_info.second->audio_type)
				{
					case AUDIO_BGM:
						mSoundsBGM[sound_info.second->sound_name] = new_sound; // push into database.
						sound_info.second->SetIsLoaded(true);
						break;
					case AUDIO_SFX:
						mSoundsSFX[sound_info.second->sound_name] = new_sound; // push into database.
						sound_info.second->SetIsLoaded(true);
						break;

				}
			}
		}
	}

	/*for (unsigned int i = 0; i < mSoundInfo.size(); i++)
	{
		std::cout << "Attempting to load: " << mSoundInfo[]
	}
	for (std::pair<unsigned int, SoundInformation*> sound_info : mSoundInfo)
	{
		std::cout << "Attempting to load : " << sound_info.second;

		

		FMOD_MODE mode = FMOD_DEFAULT;
	}*/
}
#pragma endregion

#pragma region PLAY SOUNDS STUFF
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
				channel_it->second->isActive = true;
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
				channel_it->second->isActive = true;
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

/* [PlaySFXAudio]
*  - Will play specified (audio_name) at specified (channel_no) at a volume of (audio_vol)
*  - If chanel_no is not specified... 
*		- Will find the next available channel and play it there. 
*/
void AudioManager::PlaySFXAudio(std::string audio_name, float audio_vol, int channel_no)
{
	SoundMap::iterator map_it = mSoundsSFX.find(audio_name); // tries to find the user-specified audio file 

	if (map_it != mSoundsSFX.end()) // if it's found ...
	{
		ChannelMap::iterator sfx_channel_it = mSFXChannels.find(channel_no); // check if channel exists...

		if (sfx_channel_it != mSFXChannels.end()) // if the specified channel is found ...
		{	
			// Check if it's currently occupied ... 
			ErrCodeCheck(system->playSound(map_it->second->sound, nullptr, false, &(sfx_channel_it->second)->channel));

			if (result_code)
			{
				sfx_channel_it->second->isActive = true;
				sfx_channel_it->second->isPlaying = true;
			}
		}

		else
		{
			std::cout << "Channel Not Available." << std::endl;
			return;
		}
	}
	else // no such audio file ...
	{
		std::cout << "Cannot Find Audio" << std::endl;
		return; // stop the program
	}
}

void AudioManager::PlayBGMAudio(std::string audio_name, float audio_vol, int channel_no)
{
	SoundMap::iterator map_it = mSoundsBGM.find(audio_name); // tries to find the user-specified audio file 

	if (map_it != mSoundsBGM.end()) // if it's found ...
	{
		ChannelMap::iterator bgm_channel_it = mBGMChannels.find(channel_no); // check if channel exists...

		if (bgm_channel_it != mBGMChannels.end()) // if the specified channel is found ...
		{
			// Check if it's currently occupied ... 
			ErrCodeCheck(system->playSound(map_it->second->sound, nullptr, false, &(bgm_channel_it->second)->channel));

			if (result_code)
				std::cout << "BGM PLAYING at : " << channel_no << std::endl;
		}

		else
		{
			std::cout << "Channel Not Available." << std::endl;
			return;
		}
	}
	else // no such audio file ...
	{
		std::cout << "Cannot Find Audio" << std::endl;
		return; // stop the program
	}
}
#pragma endregion

#pragma region ADJUST VOLUME STUFF
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

#pragma region STOP SOUND STUFF

/* Global Silence */
void AudioManager::StopAllSounds()
{
	// Go through every [SFX] channel
	for (auto channel : mSFXChannels)
	{
		if (channel.second->channel)
			channel.second->channel->stop();
	}

	std::cout << "MUTING & FREEING FOR ALL SFX CHANNELSS" << std::endl;

	for (auto channel : mBGMChannels)
	{
		if (channel.second->channel)
			channel.second->channel->stop();
	}

	std::cout << "MUTING & FREEING FOR ALL BGM CHANNELSS" << std::endl;
}

/* Silence all BGM Channels */
void AudioManager::StopAllBGMSounds()
{
	for (auto channel : mBGMChannels)
	{
		if (channel.second->channel)
			channel.second->channel->stop();
	}

	std::cout << "MUTING & FREEING FOR ALL BGM CHANNELSS" << std::endl;
}

void AudioManager::StopAllSFXSounds()
{
	// Go through every [SFX] channel
	for (auto channel : mSFXChannels)
	{
		if (channel.second->channel)
			channel.second->channel->stop();
	}

	std::cout << "MUTING & FREEING FOR ALL SFX CHANNELSS" << std::endl;

}

void AudioManager::StopChannel(AUDIO_TYPE audio_type, int channel_no)
{
	ChannelMap::iterator channel_it;

	switch (audio_type)
	{
	case AUDIO_BGM:
		channel_it = mBGMChannels.find(channel_no);

		if (channel_it != mBGMChannels.end()) // if the specified channel is found ...
		{
			if (channel_it->second->channel)
				channel_it->second->channel->stop();
		}
		break;
		//return 1; // SUCCESS
	case AUDIO_SFX:
		channel_it = mSFXChannels.find(channel_no); // check if channel exists...

		if (channel_it != mSFXChannels.end()) // if the specified channel is found ...
		{
			if (channel_it->second->channel)
				channel_it->second->channel->stop();
		}
		break;
		//return 1; // SUCCESS
	}

	//return 0; // FAILURE
}

#pragma endregion

#pragma region PAUSE & RESUME SOUND
// These pauses the playing of the sound only.
void AudioManager::PauseAllSounds()
{
	for (auto channel : mSFXChannels)
	{
		channel.second->channel->setPaused(true);
	}

	for (auto channel : mBGMChannels)
	{
		channel.second->channel->setPaused(true);
	}
}

void AudioManager::PauseSFXChannel(int channel_no)
{
	ChannelMap::iterator map_it = mSFXChannels.find(channel_no);

	if (map_it != mSFXChannels.end()) // found ...
	{
		std::cout << "FOUND" << std::endl;
		map_it->second->channel->setPaused(true);
	}
}

void AudioManager::PauseBGMChannel(int channel_no)
{
	ChannelMap::iterator map_it = mBGMChannels.find(channel_no);

	if (map_it != mBGMChannels.end()) // found ...
	{
		map_it->second->channel->setPaused(true);
	}

}

void AudioManager::ResumeAllSounds()
{
	for (auto channel : mSFXChannels)
	{
		channel.second->channel->setPaused(false);
	}

	for (auto channel : mBGMChannels)
	{
		channel.second->channel->setPaused(false);
	}
}


void AudioManager::ResumeSFXChannel(int channel_no)
{
	ChannelMap::iterator map_it = mSFXChannels.find(channel_no);

	if (map_it != mSFXChannels.end()) // found ...
	{
		std::cout << "FOUND" << std::endl;
		map_it->second->channel->setPaused(false);
	}
}

void AudioManager::ResumeBGMChannel(int channel_no)
{
	ChannelMap::iterator map_it = mBGMChannels.find(channel_no);

	if (map_it != mBGMChannels.end()) // found ...
	{
		map_it->second->channel->setPaused(false);
	}
}

#pragma region AUDIO INFORMATION

bool AudioManager::isActive(AUDIO_TYPE audito_type, int channel_no)
{
	return true;
}

bool AudioManager::isPlaying(AUDIO_TYPE audito_type, int channel_no)
{
	return true;
}
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