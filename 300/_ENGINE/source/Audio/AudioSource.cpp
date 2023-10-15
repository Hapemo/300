#include "Audio/AudioSource.h"
#include <iostream>

AudioSource::AudioSource() : mAudioComponent(nullptr) {}

void AudioSource::GetAudioComponent(Entity id)
{
	mAudioComponent = &(id.GetComponent<Audio>());
}

void AudioSource::Play()
{
	if (mAudioComponent != nullptr)
	{
		std::cout << "NOT EMPTY" << std::endl;
	}
}
