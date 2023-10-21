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

bool CrossFadeAudio(AudioSource& fade_out, AudioSource& fade_in, float fade_duration)
{
	if (fade_out.mAudioComponent != nullptr && fade_in.mAudioComponent != nullptr)
	{
		float fade_timer = 0.0f;

		while (fade_timer < fade_duration)
		{
			float fadeLevelOut = 1.0f - (fade_timer / fade_duration);
			float fadeLevelIn = fade_timer / fade_duration;

			// Set Volume 
			if (fade_out.mAudioComponent->mIsPlaying) // Check if it's playing.
			{
				fade_out.mAudioComponent->mChannel->setVolume(fadeLevelOut);
				
				if (!fade_in.mAudioComponent->mIsPlaying)
				{
					fade_in.mAudioComponent->mIsPlay = true;
					fade_in.mAudioComponent->mChannel->setVolume(fadeLevelIn);
				}

				else
				{
					return false; // cannot be done (because audio not plaiyng at all)
				}
			}

			else
			{
				return false; // cannot be done (because audio not plaiyng at all)
			}
			
		}
	}
}


AudioSource::AudioSource() : mAudioComponent(nullptr) {}

void AudioSource::GetAudioComponent(Entity id)
{
	mAudioComponent = &(id.GetComponent<Audio>()); // Change to "script_entity_id"
}

bool AudioSource::IsSoundAttached()
{
	return mAudioComponent->mIsLoaded;
}

bool AudioSource::AttachSound(std::string audio_name)
{
	if (systemManager->mAudioSystem.get()->CheckAudioExist(audio_name)) // Check if the audio exists in the database...
	{
		mAudioComponent->mSound = systemManager->mAudioSystem.get()->FindSound(audio_name);
		mAudioComponent->mFileName = audio_name; // for play check in Update() -> CheckAudioExist
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

		if(mAudioComponent->mIsPlaying)
		{
			// Doesn't Pause in Update() loop (coz AudioSystem doesnt run in "pause" button)
			mAudioComponent->mChannel->setPaused(true);
			mAudioComponent->mPaused = true;
			mAudioComponent->mIsPlaying = false;
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
			mAudioComponent->mChannel->setPaused(false);
			mAudioComponent->mPaused = false;
			mAudioComponent->mWasPaused = true;  // to prevent replaying of clip (if update())
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

