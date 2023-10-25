#pragma once
#include "AudioSource.h"
#include "../Input/Input.h" // testing
#include "GameState/GameStateManager.h"
#include "fmod.hpp"



void TestMovementAroundAudioSource()
{
	Entity e1 = systemManager->mGameStateSystem.get()->GetEntity("Radio", "Test1");				// Make Sure there's this entity in scene.
	Entity e2 = systemManager->mGameStateSystem.get()->GetEntity("Listener", "Test1");			// Make sure there's this entity in scene. 

	Audio& audio_source = e1.GetComponent<Audio>();
	AudioListener& listener = e2.GetComponent<AudioListener>();
	
	// Input to modify (lets test with 1 axis for now)
	if (Input::CheckKey(PRESS, UP))
		listener.mPosition.z += 0.5f;
	if (Input::CheckKey(PRESS, DOWN))
		listener.mPosition.z -= 0.5f;

	// Check if listener is beyond max distance
	if (audio_source.mSound != nullptr)
	{
		audio_source.mSound->get3DMinMaxDistance(&audio_source.mMinDistance, &audio_source.mMaxDistance);
		
		float distance = glm::distance(listener.mPosition, audio_source.mPosition);

		if (distance > audio_source.mMaxDistance)
		{
			PINFO("<Listener> has exited the <AudioSource>'s range."); 
			PINFO("Distance: %d", distance);
		}

		PINFO("Distance: %d", distance);

		//glm::distance(listener.mPosition, )
	}
	else
	{
		PINFO("ATTACH A SOUND TO TEST PLEASE.");
	}

	int hi = 3;
}