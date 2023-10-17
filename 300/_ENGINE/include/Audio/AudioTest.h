#pragma once
#include "AudioSource.h"
#include "../Input/Input.h" // testing

// Test Functions
// - Treat these functions as how you would script in C++ (LUA side can replicate)
void TestLoadPlaySuccess()
{
	// The object created through hard-coding has the "script_entity_id" of 9.
	AudioSource footsteps;
	footsteps.GetAudioComponent(footsteps.ent_id);	    // (1) Retrieve <Audio> Component data into the object.

	//if(Input::CheckKey(HOLD, P))
	if (footsteps.IsSoundAttached())					// (2a). Check whether if there's a <Sound> object attached to this "AudioSource" object.
	{
		footsteps.Play();							    // If yes, Play the Audio. (it should play)
	}

	else // (2b) If there is no sound attached...
	{
		LoadAudio("../assets\\Audio", "farm_ambience.wav"); // Load Audio by providing (1) Directory Name, (2) Audio Name
		footsteps.AttachSound("farm_ambience.wav");
		footsteps.Play();
	}
}

void TestLoadAudioDirectory()
{

}

