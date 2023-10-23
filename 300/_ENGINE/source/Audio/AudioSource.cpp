#include "Audio/AudioSource.h"
#include "ECS/ECS_Systems.h"
#include "Audio/AudioSystem.h"
#include <iostream>


bool LoadAudio(std::string file_path, std::string audio_name)
{
	bool load_status = systemManager->mAudioSystem.get()->LoadAudio(file_path, audio_name);
	return load_status;
}

bool LoadAudioFromDirectory(std::string directory_path)
{
	bool load_status = systemManager->mAudioSystem.get()->LoadAudioFromDirectory(directory_path);
	return load_status;
}

bool CrossFadeAudio(AudioSource& fade_out, AudioSource& fade_in, float fade_duration, float fade_max_vol)
{
	if (fade_out.mAudioComponent != nullptr && fade_in.mAudioComponent != nullptr)
	{
		// Fading functionaltiy (done in update() loop)
		fade_out.mAudioComponent->fade_duration = fade_duration;
		
		fade_out.mAudioComponent->mFadeOut = true;
		fade_out.mAudioComponent->mFadeIn = false;
		fade_in.mAudioComponent->fade_duration = fade_duration;
		fade_in.mAudioComponent->mFadeIn = true;
		fade_in.mAudioComponent->mFadeOut = false;
		fade_in.mAudioComponent->mFadeInMaxVol = fade_max_vol;
		systemManager->mAudioSystem.get()->fade_timer = 0.0f; // reset timer. (in case it was used before) -> this is in AudioSystem.h (static bool)
	
		return true;
	}

	return false;
}

bool FadeInAudio(AudioSource& fade_in, float fade_duration, float fade_max_vol)
{
	if (fade_in.mAudioComponent != nullptr)
	{
		// Fading functionaltiy (done in update() loop)
		fade_in.mAudioComponent->fade_duration = fade_duration;
		fade_in.mAudioComponent->mFadeIn = true;
		fade_in.mAudioComponent->mFadeOut = false;
		fade_in.mAudioComponent->mFadeInMaxVol = fade_max_vol;
		systemManager->mAudioSystem.get()->fade_timer = 0.0f; // reset timer. (in case it was used before) -> this is in AudioSystem.h (static bool)

		return true;
	}

	return false;
}

bool FadeOutAudio(AudioSource& fade_out, float fade_duration, float fade_down_vol)
{
	if (fade_out.mAudioComponent != nullptr)
	{
		// Fading functionaltiy (done in update() loop)
		fade_out.mAudioComponent->fade_duration = fade_duration;
		fade_out.mAudioComponent->mFadeIn = false;
		fade_out.mAudioComponent->mFadeOut = true;
		fade_out.mAudioComponent->mFadeOutToVol = fade_down_vol;
		systemManager->mAudioSystem.get()->fade_timer = 0.0f; // reset timer. (in case it was used before) -> this is in AudioSystem.h (static bool)

		return true;
	}

	return false;
}




AudioSource::AudioSource() : mAudioComponent(nullptr) {}

void AudioSource::GetAudioComponent(Entity id)
{
	mAudioComponent = &(id.GetComponent<Audio>()); // Change to "script_entity_id"
}

bool AudioSource::IsSoundAttached()
{
	if (mAudioComponent != nullptr)
	{
		if (mAudioComponent->mSound != nullptr)
		{
			return true;
		}
	}

	return false;

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

bool AudioSource::IsPlaying()
{
	return mAudioComponent->mIsPlaying;
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

