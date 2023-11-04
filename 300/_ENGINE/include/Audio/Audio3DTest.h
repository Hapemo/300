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
}

//void TestListener()
//{
//	Entity e1 = systemManager->mGameStateSystem.get()->GetEntity("Radio", "Test1");				// Make Sure there's this entity in scene.
//
//	Audio& audio_source = e1.GetComponent<Audio>();
//	//Transform& audio_transform = e1.GetComponent<Transform>();
//
//	FMOD_VECTOR sample_velocity = { 0.0f, 0.0f, 0.0f }; // Simulate Initial Velocity
//
//	FMOD_VECTOR sample_forward_vector = { 0.0f, 0.0f, 0.0f }; // Will be determined when user input is done.
//
//	if (Input::CheckKey(PRESS, UP)) {
//		sample_forward_vector = { 1.0f, 0.0f, 0.0f }; // Character is facing along positive x-axis
//	}
//	else if (Input::CheckKey(PRESS, LEFT)) {
//		sample_forward_vector = { 0.0f, 1.0f, 0.0f }; // Character is facing along positive y-axis
//	}
//	else if (Input::CheckKey(PRESS, DOWN)) {
//		sample_forward_vector = { -1.0f, 0.0f, 0.0f }; // Character is facing along negative x-axis
//	}
//	else if (Input::CheckKey(PRESS, RIGHT)) {
//		sample_forward_vector = { 0.0f, -1.0f, 0.0f }; // Character is facing along negative y-axis
//	}
//
//	if (Input::CheckKey(PRESS, UP))
//	{
//
//	}
//
//	if (Input::CheckKey(PRESS, DOWN))
//	{
//
//	}
//		
//	//if (Input::CheckKey(PRESS, DOWN))
//	//	listener.mPosition.z -= 0.5f;
//
//	//FMOD_VECTOR listener_position = { audio_transform.mTranslate.x, audio_transform.mTranslate.y , audio_transform.mTranslate.z };
//	////FMOD_VECTOR listener_velocity = { listener_component.mVelocity.x, listener_component.mVelocity.y , listener_component.mVelocity.z };
//	//FMOD_VECTOR listener_forward = { 0.0f, 0.0f , 1.0f}; // hard coded value to test first (consult graphics/yujun for on details regarding forward vector)
//	////FMOD_VECTOR listener_up = { listener_component.mUp.x, listener_component.mUp.y , listener_component.mUp.z };
//
//	//PINFO("Initializing 3D AudioListener Parameters.");
//	//ErrCodeCheck(system_obj->set3DListenerAttributes(0, &listener_position, nullptr, &listener_forward, &listener_up)); // 1st param = 0 (means only 1 Listener can exist at one time)
//	////AudioListener& listener = e2.GetComponent<AudioListener>();
//	
//}