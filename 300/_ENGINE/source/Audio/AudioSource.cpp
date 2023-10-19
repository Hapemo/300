#include "Audio/AudioSource.h"
#include "ECS/ECS_Systems.h"
#include "Audio/AudioSystem.h"
#include <iostream>


bool LoadAudio(std::string file_path, std::string audio_name)
{
	bool load_status = systemManager->mAudioSystem.get()->LoadAudio(file_path, audio_name);
	return load_status;
}

bool LoadAudioFromDirectory(std::filesystem::path file_path)
{
	bool load_status = systemManager->mAudioSystem.get()->LoadAudioFromDirectory(file_path);
	return load_status;
}


AudioSource::AudioSource() : mAudioComponent(nullptr) {}

void AudioSource::GetAudioComponent(Entity id)
{
	mAudioComponent = &(id.GetComponent<Audio>());
}

bool AudioSource::IsSoundAttached()
{
	if (mAudioComponent->mSound != nullptr)
	{
		return true;
	}

	return false;
}

bool AudioSource::AttachSound(std::string audio_name)
{
	if (systemManager->mAudioSystem.get()->CheckAudioExist(audio_name)) // Check if the audio exists in the database...
	{
		mAudioComponent->mSound = systemManager->mAudioSystem.get()->FindSound(audio_name);
		return true;
	}

	return false;
}



void AudioSource::Play()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{	
		AudioSystem* system = systemManager->mAudioSystem.get();

		if (mAudioComponent->mSound != nullptr)
		{   
			// The [playing] should be in the [Update()] loop.
			mAudioComponent->mIsPlay = true;
		}
	}
}


void AudioSource::Pause()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		AudioSystem* system = systemManager->mAudioSystem.get();

		bool playing;
		mAudioComponent->mChannel->isPlaying(&playing);

		if (playing)
		{
			// The [playing] should be in the [Update()] loop.
			mAudioComponent->mSetPause = true;
		}
	}
}


void AudioSource::Unpause()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		AudioSystem* system = systemManager->mAudioSystem.get();

		if (mAudioComponent->mPaused)
		{
			// The [playing] should be in the [Update()] loop.
			mAudioComponent->mSetUnpause = true;
		}
	}
}

void AudioSource::Stop()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		AudioSystem* system = systemManager->mAudioSystem.get();

		mAudioComponent->mChannel->stop();		
	}
}

void AudioSource::Mute()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		bool playing; 
		mAudioComponent->mChannel->isPlaying(&playing);
		if (playing)
		{
			mAudioComponent->mChannel->setVolume(0.0f);
		}
	}
}

void AudioSource::SetVolume(float volume)
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		bool playing;
		mAudioComponent->mChannel->isPlaying(&playing);
		if (playing)
		{
			mAudioComponent->mChannel->setVolume(volume);
		}
	}
}

void AudioSource::SetIsLoop(float looping)
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		mAudioComponent->mIsLooping = looping;
	}
}

