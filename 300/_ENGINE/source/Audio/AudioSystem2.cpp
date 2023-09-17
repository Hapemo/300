#include "Audio/AudioSystem2.h"

AudioSystem::AudioSystem() : sfxVolume(1.0f), bgmVolume(1.0f)
{
	
}

AudioSystem::~AudioSystem()
{

}

void AudioSystem::Init()
{	
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
	for (; no_of_sfx_channels < NO_OF_SFX_CHANNELS_TO_INIT; no_of_sfx_channels++)
	{
		std::cout << "CREATING SFX CHANNEL #" << no_of_sfx_channels << "." << std::endl;

		FMOD::Channel* new_channel;
		mChannels.find(AUDIO_SFX)->second.push_back(new_channel);
		
	}

	for (; no_of_bgm_channels < NO_OF_BGM_CHANNELS_TO_INIT; no_of_bgm_channels++)
	{
		std::cout << "CREATING SFX CHANNEL #" << no_of_sfx_channels << "." << std::endl;

		FMOD::Channel* new_channel;
		mChannels.find(AUDIO_SFX)->second.push_back(new_channel);
	}

	std::cout << "HI";

	//LoadAudioFiles("../assets/Audio");
	LoadAudioFromDirectory("../assets/Audio");
}

void AudioSystem::Update()
{

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

		return 1; // failure
	}
	std::cout << "FMOD OPERATION OK." << std::endl;
	return 0; // success (no issues)
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
		std::cout << "Audio Name: " << audio_name << std::endl;

		std::cout << "Creating Sound: ";
		FMOD::Sound* new_sound;
		ErrCodeCheck(system_obj->createSound(file_path.string().c_str(), FMOD_LOOP_NORMAL, 0, &new_sound));

		mSounds.insert(std::make_pair(audio_name, new_sound));
	}
}