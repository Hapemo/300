/*!*************************************************************************
****
\file		   AudioSource.cpp
\author(s)	   Cheong Ming Lun
\par DP email: m.cheong@digipen.edu
\date		   11-4-2023
\brief

This file contains the "AudioSource" class that wraps around features and functionality
of the audio system.

This is the object that will be used in "LUA" scripting for functionality. Trying to replicate
what is familiar in unity.
****************************************************************************/
#include "Audio/AudioSource.h"
#include "ECS/ECS_Systems.h"
#include "Audio/AudioSystem.h"
#include <iostream>


/******************************************************************************/
/*!
	LoadAudio()
	- In Script, users can load new audio files into the system.
 */
 /******************************************************************************/
bool LoadAudio(std::string file_path, std::string audio_name)
{
	bool load_status = systemManager->mAudioSystem.get()->LoadAudio(file_path, audio_name);
	return load_status;
}


/******************************************************************************/
/*!
	LoadAudioFromDirectory()
	- In Script, users can load new audio files into the system by specifying the directory path.
 */
 /******************************************************************************/
bool LoadAudioFromDirectory(std::string directory_path)
{
	bool load_status = systemManager->mAudioSystem.get()->LoadAudioFromDirectory(directory_path);
	return load_status;
}

/******************************************************************************/
/*!
	CrossFadeAudio 
	- By providing 2 audiosource objects
	- [1st AudioSource] : the audio that the user wants to fade out
	- [2nd AudioSource] : the audio that the user wants to fade in
	- User can also provide the fade duration. 
	- This is created to facilitate the changing of different bgms in a natural way.
 */
 /******************************************************************************/
bool CrossFadeAudio(AudioSource& fade_out, AudioSource& fade_in, float fade_duration)
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
		systemManager->mAudioSystem.get()->fade_timer = 0.0f; // reset timer. (in case it was used before) -> this is in AudioSystem.h (static bool)
	
		return true;
	}

	return false;
}


/******************************************************************************/
/*!
	FadeInAudio()
	- Providing the relevant information can control how an <AudioSource> audio fades in from it's current volume
 */
 /******************************************************************************/
bool FadeInAudio(AudioSource& fade_in, float fade_duration, float speed_modifier, float fade_max_vol)
{
	if (fade_in.mAudioComponent != nullptr)
	{
		// Fading functionaltiy (done in update() loop)
		fade_in.mAudioComponent->fade_duration = fade_duration;
		fade_in.mAudioComponent->mVolume = 0.0f; // Start from volume : 0.
		fade_in.mAudioComponent->mFadeIn = true;
		fade_in.mAudioComponent->mFadeOut = false;
		fade_in.mAudioComponent->mFadeInMaxVol = fade_max_vol;
		fade_in.mAudioComponent->mFadeSpeedModifier = speed_modifier;
		systemManager->mAudioSystem.get()->fade_timer = 0.0f; // reset timer. (in case it was used before) -> this is in AudioSystem.h (static bool)

		return true;
	}

	return false;
}

/******************************************************************************/
/*!
	FadeOutAudio()
	- Providing the relevant information can control how an <AudioSource> audio fades out from it's current volume
 */
 /******************************************************************************/
bool FadeOutAudio(AudioSource& fade_out, float fade_duration, float speed_modifier, float fade_down_vol)
{
	if (fade_out.mAudioComponent != nullptr)
	{
		// Fading functionaltiy (done in update() loop)
		fade_out.mAudioComponent->fade_duration = fade_duration;
		fade_out.mAudioComponent->mFadeIn = false;
		fade_out.mAudioComponent->mFadeOut = true;
		fade_out.mAudioComponent->mFadeOutToVol = fade_down_vol;
		fade_out.mAudioComponent->mFadeSpeedModifier = speed_modifier;
		systemManager->mAudioSystem.get()->fade_timer = 0.0f; // reset timer. (in case it was used before) -> this is in AudioSystem.h (static bool)

		return true;
	}

	return false;
}

/******************************************************************************/
/*!
	[Default Constructor] AudioSource
	- Has a reference to a specific <Audio> component to have functionality onto
 */
 /******************************************************************************/
AudioSource::AudioSource() : mAudioComponent(nullptr) {}


/******************************************************************************/
/*!
	GetAudioComponent()
	- Attach a <Audio> component to this <AudioSource> to work functionality on it.
 */
 /******************************************************************************/
void AudioSource::GetAudioComponent(Entity id)
{
 	mAudioComponent = &(id.GetComponent<Audio>()); // Change to "script_entity_id"
}

/******************************************************************************/
/*!
	IsSoundAttached()
	- Check if this <Audio> component has already have a "Sound" attached to it, before doing operations to it (playing etc)
 */
 /******************************************************************************/
bool AudioSource::IsSoundAttached()
{
	return mAudioComponent->mIsLoaded;
}


/******************************************************************************/
/*!
	AttachSound()
	- Attach a FMOD::Sound* object to the <Audio> component that this <AudioSource> is currently holding onto.
 */
 /******************************************************************************/
bool AudioSource::AttachSound(std::string audio_name)
{
	if (!mAudioComponent->mIsEmpty) // if it's not empty
	{
		if (systemManager->mAudioSystem.get()->CheckAudioExist(audio_name)) // Check if the audio exists in the database...
		{
			mAudioComponent->mSound = systemManager->mAudioSystem.get()->FindSound(audio_name);
			mAudioComponent->mFileName = audio_name; // for play check in Update() -> CheckAudioExist

			// For Editor side 
			mAudioComponent->mIsEmpty = false; // no longer empty anymore...
			mAudioComponent->mFullPath = "../assets\\Audio/" + audio_name; // Temporary 

			return true;
		}
	}
	
	return false;
}



/******************************************************************************/
/*!
	Play()
	- Play the sound attached to this <AudioSource>
 */
 /******************************************************************************/
void AudioSource::Play()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{	
		if (!mAudioComponent->mIsPlaying)
		{
			if (mAudioComponent->mSound != nullptr)
			{
				// The [playing] should be in the [Update()] loop.
				mAudioComponent->mIsPlay = true;
				mAudioComponent->mChannel->setPosition(0, FMOD_TIMEUNIT_MS);
			}
		}
	}
}

/******************************************************************************/
/*!
	Pause()
	- Pause the sound attached to this <AudioSource>
 */
 /******************************************************************************/
void AudioSource::Pause()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		if(mAudioComponent->mIsPlaying)
		{
			// Doesn't Pause in Update() loop (coz AudioSystem doesnt run in "pause" button)
			mAudioComponent->mChannel->setPaused(true);
			mAudioComponent->mPaused = true;
			mAudioComponent->mIsPlaying = false;
		}
	}
}

/******************************************************************************/
/*!
	Unpause()
	- Unpause the sound attached to this <AudioSource>
 */
 /******************************************************************************/
void AudioSource::Unpause()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{


		if (mAudioComponent->mPaused)
		{
			// The [playing] should be in the [Update()] loop.
			mAudioComponent->mChannel->setPaused(false);
			mAudioComponent->mPaused = false;
			mAudioComponent->mWasPaused = true;  // to prevent replaying of clip (if update())
		}
	}
}

/******************************************************************************/
/*!
	Stop()
	- Stop the sound attached to this <AudioSource>
 */
 /******************************************************************************/
void AudioSource::Stop()
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		mAudioComponent->mChannel->stop();	
		mAudioComponent->mIsPlaying = false;
		mAudioComponent->mWasPaused = false;
		mAudioComponent->mPaused = false;
		mAudioComponent->mIsPlay = false;
	}
}

/******************************************************************************/
/*!
	Mute()
	- Mute the sound attached to this <AudioSource>
 */
 /******************************************************************************/
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

/******************************************************************************/
/*!
	SetVolume()
	- Set the volume for this current <AudioSource>
 */
 /******************************************************************************/
void AudioSource::SetVolume(float volume)
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		FMOD::Sound* current_sound;
		mAudioComponent->mChannel->getCurrentSound(&current_sound);
		if (current_sound)
		{
			bool playing;
			mAudioComponent->mChannel->isPlaying(&playing);
			if (playing)
			{
				mAudioComponent->mChannel->setVolume(volume);
				mAudioComponent->mVolume = volume;
				PINFO("SETTING VOLUME : %f", volume);
			}

			else
			{
				mAudioComponent->mVolume = volume;
			}
		}
	
	}
}

/******************************************************************************/
/*!
	SetIsLoop()
	- Set it so that this audio is looping.
 */
 /******************************************************************************/
void AudioSource::SetIsLoop(float looping)
{
	if (mAudioComponent != nullptr) // Make sure there is the <Audio> component reference.
	{
		mAudioComponent->mIsLooping = looping;
	}
}

