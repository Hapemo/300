#pragma once
#include "AudioSource.h"
#include "../Input/Input.h" // testing

// Test Functions
// - Treat these functions as how you would script in C++ (LUA side can replicate)


/******************************************************************************/
/*!
*   Test Case #1 (Load + Play)
*   --------------------------------------------------------------------------
*	Q. This is to test if basic functions such as "LoadAudio" & "PlayAudio" works.
*	Also simulating a error-check code flow using "IsSoundAttached" to troubleshoot
*	whether the <AudioSource> object is currently holding onto a [Sound] object. (for playing)
* 
*	- This test requires the [Scriptor] to already have "Loaded" the audio file 
*   by dragging the "Audio" file on the editor to the <Audio> component.
*
	Tested Functions
	--------------------------------------------------------------------------
	- GetAudioComponent()			// Retrive <Audio> Component onto this <AudioSource>
	- IsSoundAttached()				// Check with <AudioSystem>'s database whether the [Sound] has been uploaded.
	- LoadAudio()					// Loads [1 specific Audio]
	- AttachSound()					// Attach a Sound to this <AudioSource>
	- Play()						// Play Sound Attached to this <AudioSource> instance.
 */
 /******************************************************************************/
void TestLoadPlaySuccess()
{
	// The object created through hard-coding has the "script_entity_id" of 9.
	AudioSource footsteps;

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	Entity ent_id = 0;

	for (Entity ent : audio_ent)
	{
		ent_id = ent.id;
	}
	// [Not in script] -----------------------------------------------------------------------------

	// [ent_id] will be replaced with "script_entity_id" in Lua.
	footsteps.GetAudioComponent(ent_id);				 // (1) Retrieve <Audio> Component data into the object.

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


/******************************************************************************/
/*!
*   Test Case #2 (Load + Play)
*   --------------------------------------------------------------------------
*	Q. This is to test if "LoadAudioFromDirectory" works & shows an example use case
*	   for the scriptor to use.
* 
	Tested Functions
	--------------------------------------------------------------------------
	- LoadAudioFromDirectory()		// Loading [Audio Files] from a specified directory (std::string)
	- GetAudioComponent()			// Retrive <Audio> Component onto this <AudioSource>
	- AttachSound()					// Attach a Sound to this <AudioSource>
	- Play()						// Play Sound Attached to this <AudioSource> instance.
 */
 /******************************************************************************/

void TestLoadAudioDirectory()
{
	LoadAudioFromDirectory("../assets\\Audio");
	
	AudioSource footsteps;

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	Entity ent_id = 0;

	for (Entity ent : audio_ent)
	{
		ent_id = ent.id;
	}
	// [Not in script] -----------------------------------------------------------------------------

	// [ent_id] will be replaced with "script_entity_id" in Lua.
	footsteps.GetAudioComponent(ent_id);				 // (1) Retrieve <Audio> Component data into the object.


	footsteps.AttachSound("footsteps-boots-101657.mp3");
	footsteps.Play();
}

/******************************************************************************/
/*!
*   Test Case #3 (Load + Play)
*   --------------------------------------------------------------------------
*	Q. This is to test if "AttachSound" can override the previous exisitng <Sound> 
*	   object attached to this <AudioSource> instance.
* 
	Tested Functions
	--------------------------------------------------------------------------
	- LoadAudioFromDirectory()		// Loading [Audio Files] from a specified directory (std::string)
	- GetAudioComponent()			// Retrive <Audio> Component onto this <AudioSource>
	- AttachSound()					// Attach a Sound to this <AudioSource>
	- Play()						// Play Sound Attached to this <AudioSource> instance.
 */
 /******************************************************************************/
void TestOverrideAttachSound()
{
	LoadAudioFromDirectory("../assets\\Audio");

	AudioSource footsteps;

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	Entity ent_id = 0;

	for (Entity ent : audio_ent)
	{
		ent_id = ent.id;
	}
	// [Not in script] -----------------------------------------------------------------------------

	// [ent_id] will be replaced with "script_entity_id" in Lua.
	footsteps.GetAudioComponent(ent_id);				 // (1) Retrieve <Audio> Component data into the object.


	footsteps.AttachSound("footsteps-boots-101657.mp3");
	footsteps.AttachSound("tuning-radio-7150.wav");
	footsteps.Play();
}

/******************************************************************************/
/*!
*   Test Case #4 (Pause)
*   --------------------------------------------------------------------------
*	! Require you to run "Test #3" first. !
*   Q. Testing the "Pause" function on an already playing audio.
*
	Tested Functions 
	- Copied over Test #3 (refer for functions tested there) 
	--------------------------------------------------------------------------
	- Pause()						// Pauses the [Sound] attached.
 */
 /******************************************************************************/
void TestPauseSound()
{
	AudioSource footsteps;

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	Entity ent_id = 0;

	for (Entity ent : audio_ent) 
	{
		ent_id = ent.id;
	}
	// [Not in script] -----------------------------------------------------------------------------
	
	// [ent_id] will be replaced with "script_entity_id" in Lua.
	footsteps.GetAudioComponent(ent_id);				 // (1) Retrieve <Audio> Component data into the object.


	footsteps.Pause();
}

/******************************************************************************/
/*!
*   Test Case #5 (Unpause)
*   --------------------------------------------------------------------------
*	! Require you to run "Test #3" first. !
*   Q. Testing the "Pause" function on an already playing audio.
*
	Tested Functions
	- Using Test #3 + Test #4
	--------------------------------------------------------------------------
	- Unpause()						// Pauses the [Sound] attached.
 */
 /******************************************************************************/
void TestUnpauseSound()
{
	AudioSource footsteps;

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	Entity ent_id = 0;

	for (Entity ent : audio_ent)
	{
		ent_id = ent.id;
	}
	// [Not in script] -----------------------------------------------------------------------------

	// [ent_id] will be replaced with "script_entity_id" in Lua.
	footsteps.GetAudioComponent(ent_id);				 // (1) Retrieve <Audio> Component data into the object.

	footsteps.Unpause();
}


/******************************************************************************/
/*!
*   Test Case #6 (Mute)
*   --------------------------------------------------------------------------
*	! Require you to run "Test #3" first. !
*   Q. Testing the "Pause" function on an already playing audio.
*
	Tested Functions
	- Using Test #3 + Test #4
	--------------------------------------------------------------------------
	- Mute()						// Mute the [Sound] attached.
 */
 /******************************************************************************/
void TestMuteSound()
{
	AudioSource footsteps;

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	Entity ent_id = 0;

	for (Entity ent : audio_ent)
	{
		ent_id = ent.id;
	}
	// [Not in script] -----------------------------------------------------------------------------

	// [ent_id] will be replaced with "script_entity_id" in Lua.
	footsteps.GetAudioComponent(ent_id);				 // (1) Retrieve <Audio> Component data into the object.

	footsteps.Mute();
}

void TestSetVolume()
{
	AudioSource footsteps;

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	Entity ent_id = 0;

	for (Entity ent : audio_ent)
	{
		ent_id = ent.id;
	}
	// [Not in script] -----------------------------------------------------------------------------

	footsteps.GetAudioComponent(ent_id);				 // (1) Retrieve <Audio> Component data into the object.

	footsteps.SetVolume(0.1f);

}


void TestCrossFade()
{
	AudioSource farm;		// Have it's specific "entity id"
	AudioSource battle;		// Have it's specific "entity id"

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity e : audio_ent)
	{
		int hi = 3;
	}
	auto audio1 = audio_ent[0];
	auto audio2 = audio_ent[1];

	battle.GetAudioComponent(audio1);
	farm.GetAudioComponent(audio2);

	//for (Entity ent : audio_ent)
	//{
	//	ent_id = ent.id;
	//}
	// [Not in script] -----------------------------------------------------------------------------
	PINFO("CROSS FADING");
	CrossFadeAudio(farm, battle, 5.0);
}

void TestCrossFadeBack()
{
	AudioSource farm;		// Have it's specific "entity id"
	AudioSource battle;		// Have it's specific "entity id"

	// [Not in script] - Ignore this part. (the script entity id is different everytime i reset)
	auto audio_ent = systemManager->ecs->GetEntitiesWith<Audio>();

	for (Entity e : audio_ent)
	{
		int hi = 3;
	}
	auto audio1 = audio_ent[0];
	auto audio2 = audio_ent[1];

	battle.GetAudioComponent(audio1);
	farm.GetAudioComponent(audio2);

	//for (Entity ent : audio_ent)
	//{
	//	ent_id = ent.id;
	//}
	// [Not in script] -----------------------------------------------------------------------------
	CrossFadeAudio(battle, farm, 5.0);
}
