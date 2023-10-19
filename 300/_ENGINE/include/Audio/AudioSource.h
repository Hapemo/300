#pragma once
#include "ECS/ECS_Components.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Systems.h"

// [10/13] Interfacing for (Designers) to use in [Scripting] 
/******************************************************************************/
/*!
	[Class] AudioSource (Scripting Object)
	 \brief
	  - Interfaced functions that make it easy to use on scripting side (port to Lua)
	  - Container for [Scripting] side to use.
	  - Works with <Audio> component for the data.
	 \brief
	  - Each <AudioSource> will have their own channel.
	 [Note] - Retrieve "script_entity_id" is used to retrieve the <Audio> component of 
			  the entity that is attached to.
 */
 /******************************************************************************/

// Not Encapsulated in the "AudioSource" object.
bool LoadAudio(std::string file_path, std::string audio_name);  // Way to LoadAudio in Script. (Have to Load first, then attach)
bool LoadAudioFromDirectory(std::filesystem::path file_path);

class AudioSource								// [LUA] - Interface to interact with [AudioSystem] functionality.
{
public:
	Audio* mAudioComponent;					    // Relevant <Audio> component data.
	Entity ent_id = 9;							// Will Remove (ID of the hardcorded object that has <Audio> component)
public:
	AudioSource();

public:
	// Data-related functions
	void GetAudioComponent(Entity id);
	bool IsSoundAttached();						// [Debug] Checks whether if the Sound Attached is in the <Audio> component.
	bool AttachSound(std::string audio_name);	// Can be used to attach a <Sound> object onto this <AudioSource> instance.

	void Play();								// Play the [Sound] that is attached to this [AudioSource] (Should run <IsSoundAttached> to make sure audio is there)
	void Pause();								// Pause the Audio.				
	void Unpause();
	void Stop();
	void Mute();
	void SetVolume(float volume);
	void SetIsLoop(float loop);

	// Planned (Specialized)
	// --------------------
	//void PlayDelayed();
	//void TransitionBGM(AudioSource* transition_to, float audio_delay);
	//void SetRandomnizedPitch();  // For Walking






};
