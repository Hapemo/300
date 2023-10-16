#pragma once
#include "AudioSource.h"
#include "../Input/Input.h" // testing

// Test Functions
// - Treat these functions as how you would script in C++
void TestGetAudioComponent()
{
	// The object created through hard-coding has the "script_entity_id" of 9.
	AudioSource footsteps;
	footsteps.GetAudioComponent(footsteps.ent_id);	    // (1) Retrieve <Audio> Component data into the object.

	//if(Input::CheckKey(HOLD, P))
	footsteps.Play();							    // (2) Play the Audio.
}
