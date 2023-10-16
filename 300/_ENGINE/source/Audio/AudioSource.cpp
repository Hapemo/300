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
			/*if (system->CheckAudioExist(mAudioComponent->mFileName))
			{
				system->PlayAudioSource(mAudioComponent->mSound, mAudioComponent->mChannel);
			}*/

			// The [playing] should be in the [Update()] loop.
			mAudioComponent->mIsPlay = true;
		}
	}
}


void AudioSource::Pause()
{
	//if(mAudioComponent)
}
