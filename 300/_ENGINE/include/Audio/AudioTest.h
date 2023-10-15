#pragma once
#include "AudioSource.h"

// Test Functions
void TestGetAudioComponent()
{
	// The object created through hard-coding has the "script_entity_id" of 9.
	AudioSource footsteps;
	footsteps.GetAudioComponent(footsteps.ent_id);
}
