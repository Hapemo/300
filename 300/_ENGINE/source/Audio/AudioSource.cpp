#include "Audio/AudioSource.h"
#include "ECS/ECS_Systems.h"
#include "Audio/AudioSystem.h"
#include <iostream>


AudioSource::AudioSource() : mAudioComponent(nullptr) {}

void AudioSource::GetAudioComponent(Entity id)
{
	mAudioComponent = &(id.GetComponent<Audio>());
}

bool AudioSource::IsSoundAttached()
{
	//if(mAudioComponent->mSound != nullptr)
	return false;
}

bool AudioSource::LoadAudio(std::string file_path, std::string audio_name)
{
	bool load_status = systemManager->mAudioSystem.get()->LoadAudio(file_path, audio_name);
	return load_status;
}

void AudioSource::Play()
{
	/*if (mAudioComponent != nullptr)
	{
		std::cout << "NOT EMPTY" << std::endl;
	}*/

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
	//if(mAudioComponent)
}
