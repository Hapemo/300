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
	std::vector<FMOD::Channel*> sfx_channels;
	std::vector<FMOD::Channel*> bgm_channels;

	mChannels.insert(std::make_pair(AUDIO_SFX, sfx_channels));  
	mChannels.insert(std::make_pair(AUDIO_BGM, bgm_channels)); 

	// Populate Channels
	for (int i = 0; i < NO_OF_SFX_CHANNELS_TO_INIT; ++i)
	{
		FMOD::Channel* new_channel;
		mChannels[AUDIO_SFX].push_back(new_channel);
	}
	for (int i = 0; i < NO_OF_BGM_CHANNELS_TO_INIT; ++i)
	{
		FMOD::Channel* new_channel;
		mChannels[AUDIO_BGM].push_back(new_channel);
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
	return 1; // success (no issues)
}