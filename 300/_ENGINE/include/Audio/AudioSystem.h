

//#include "../../lib/FMOD Studio API Windows/api/studio/inc/fmod_studio.hpp"
#include <fmod_studio.hpp>
#include <fmod.hpp>		// Include [FMOD] Library
#include <iostream>
#include <map>
#include <filesystem>
#include "Input.h"
#include "Singleton.h"

class AudioManager;
//class AudioListener;

enum AUDIO_TYPE
{
	AUDIO_SFX, 
	AUDIO_BGM, 
	AUDIO_TOTAL
};

// Added 9/4/2023 (thinking how to integrate with component data)
struct Sound
{
	FMOD_VECTOR	 position; 
	FMOD::Sound* sound;
};
	
static int MAX_AUDIO_FILES_PLAYING = 32;         // Number of Sounds (that are allowed to be played simultaneously)
static int NO_OF_SFX_CHANNELS_TO_INIT  = 32;	 // Number of SFX Channels (to init at the start)
static int NO_OF_BGM_CHANNELS_TO_INIT = 16;		 // Number of BGM Channels

//extern std::unique_ptr<AudioManager> mAudio; // Audio Manager Reference (only 1)

// Initialize the Audio System


// [Audio System] object - data 
// - Manages Audio Files & Relevant Data 
//struct AudioSystem {
	//AudioSystem();

	//void Update();

	//typedef std::map<int, FMOD::Channel*>				ChannelMap;		
	//typedef std::map<std::string, Sound*>				SoundMap;
	//typedef std::map<std::string, FMOD::Studio::Bank*>  BankMap;

	//int channel_size = 0;									// Acts as a counter + naming of ID of newly created Channels.

	//FMOD::System* system;									// System Object
	//FMOD::Studio::System* studio_system;					// [FMOD Studio API]	 - Studio System Object (Events)
	//FMOD_RESULT	  sys_result_code;							// Used to troubleshoot stuff, tells us if an operation is successful or not.
	//ChannelMap	  aChannels;								// [List of Channels]    - Keeps Track of which Channels are Available. (Channels Start from ID: 1)
	//SoundMap	  aSoundsSFX;							    // [List of SFX]	     - Stores all the (SFX) Sound files that has been loaded into the system.
	//SoundMap	  aSoundsBGM;								// [List of BGM]         - Stores all the (BGM) Sound files that has been loaded into the system.
	//BankMap		  aBank;									// [Banks - Event Based] - Stores all the sounds and information for each event.
//};



class Channel
{
public:
	Channel();
	//Channel(FMOD::Channel* new_channel, bool active);
public:
	FMOD::Channel* channel;
	bool		   isActive;
};


class AudioManager : public Singleton<AudioManager>
{
public: 
	typedef std::unordered_map<int, Channel*>				      ChannelMap;
	typedef std::unordered_map<std::string, Sound*>				  SoundMap;
	typedef std::unordered_map<std::string, FMOD::Studio::Bank*>  BankMap;

	int sfx_channel_no = 0;

	int bgm_channel_no = 0;

public:
	static int ErrCodeCheck(FMOD_RESULT result);		    // Status Check for (FMOD Function Calls) -> can be used outside of class

	/* Create System Object + Initialization */
	AudioManager();											// Initialization of (FMOD System)

	void Init();
	void Update();
	void Exit();

	/* [Load Sounds]
	   [TODO] - Work with [Serialization Team] to port over relevant data files in the future
	*/
	void LoadAudioFile(std::string audiofilePath, std::string audio_name, AUDIO_TYPE audio_type, bool spatial = false, FMOD_VECTOR audio_position = {0.0f,0.0f,0.0f} ,bool looping = false);

	/* [Play Audio]
	   - Play the specified audio file. 
	*/
	void PlayAudio(std::string audio_name, AUDIO_TYPE audio_type, FMOD_VECTOR audio_pos, float audio_vol, int channel_no = 0); // Specify Channel
	
#pragma region VOLUME STUFF
	/* [Set Specific Channel Volume]
	   - Alter the volume for a specific channel.
	   - [Must Specify] : (1) AUDIO_BGM / AUDIO_SFX
       -				  (2) Channel Id
	*/
	bool SetChannelVolume(AUDIO_TYPE audio_type, int channel_id, float channel_vol);

	/* [Set Global Volume]
	*  - Global volume control 
	*/
	void SetAllVolume(float channel_vol);

	void SetBGMVolume(float channel_vol);

	void SetSFXVolume(float channel_vol);
#pragma endregion
	



	// void SetGlobalVolume(int channel_id,a)

	//bool isPlaying(int channel_no);

	void SetChannel3DPosition(int channel_id, FMOD_VECTOR audio_pos);


	/* [TODO] 
	*	1. StopPLay
	*/

	// For [FMOD Studios]
	void LoadAudioBank(std::string bankfilepath, std::string bank_name); // Support for different modes (?) - research



	//void LoadEvent()
	//void LoadEvents();
	/* Play Sounds */
	// void PlayBGMAudio(std::string audio_name);
	/* Testing Spatial Audio Stuff*/
	//void PlaySpatialAudio(std::string audio_name);

	/* Helper Function */
	//Audio* FindSFXAudio(std::string audio_name)
	//{
	//	std::map<std::string, Audio*>::iterator it = sfx.find(audio_name); 

	//	if (it != sfx.end()) // found
	//	{
	//		std::cout << "Audio (" << audio_name << ") is found." << std::endl;
	//		return it->second; // return the audio object.
	//	}
	//	else
	//		std::cout << "Audio cannot be found." << std::endl;
	//}

	//Audio* FindBGMAudio(std::string audio_name)
	//{
	//	std::map<std::string, Audio*>::iterator it = bgm.find(audio_name);

	//	if (it != bgm.end()) // found
	//	{
	//		std::cout << "Audio (" << audio_name << ") is found." << std::endl;
	//		return it->second; // return the audio object.
	//	}
	//	else
	//		std::cout << "Audio cannot be found." << std::endl;
	//}

	


public:
	// AudioSystem* system_obj;								// Stores all the relevant FMOD data.

	FMOD::System* system;									// System Object
	FMOD::Studio::System* studio_system;					// [FMOD Studio API]		 - Studio System Object (Events)
	FMOD_RESULT	  result_code;								// Used to troubleshoot stuff, tells us if an operation is successful or not.
	ChannelMap	  mSFXChannels;								// [List of SFX Channels]	 - Keeps Track of which Channels are Available. (Channels Start from ID: 0)
	ChannelMap	  mBGMChannels;								// [List of BGM Channels]    - (BGM) Reserved Channels
	SoundMap	  mSoundsSFX;							    // [List of SFX]			 - Stores all the (SFX) Sound files that has been loaded into the system.
	SoundMap	  mSoundsBGM;								// [List of BGM]			 - Stores all the (BGM) Sound files that has been loaded into the system.
	BankMap		  mBank;									// [Banks - Event Based]	 - Stores all the sounds and information for each events
	
};

// Listener Object (Component-ish data)
//class AudioListener
//{
//public: 
//	
//	// Default Constructor (must have a audio system reference)
//	AudioListener(AudioSystem* audiosys);
//
//	// Parameterized Constructor
//	// - Thinking of inheriting he player's/gameobject's position (that his listener is attached to)
//	AudioListener(AudioSystem* audiosys, FMOD_VECTOR object_pos);
//	
//	// Set Position - x,y,z
//	void SetListenerPosition(float x, float y, float z);
//	// Set Position - Vector Support
//	void SetListenerPosition(FMOD_VECTOR object_pos);
//	// Get Position - Vector Position
//	FMOD_VECTOR GetListenerPosition() const;
//
//	//void SetVolume(float volume)
//	//{
//	//	if (mAudio->channel)
//	//	{
//	//		mAudio->channel->setVolume(volume);
//	//	}
//	//}
//
//	//float GetVolume() const
//	//{
//	//	float volume = 0.0f; // to return
//
//	//	if (mAudio->channel)
//	//	{
//	//		mAudio->channel->getVolume(&volume); // channel volume will be returned to the variable.
//	//	}
//
//	//	return volume;
//
//	//}
//
//	void Update()
//	{
//		//if (audioSystem)
//		//{
//		//	float distance = 0.0f; 
//		//	FMOD_VECTOR relativePos = { sound_pos.x - listener_pos.x, sound_pos.y - listener_pos.y, sound_pos.z - listener_pos.z };
//		//	distance = std::sqrt(relativePos.x * relativePos.x + relativePos.y * relativePos.y + relativePos.z * relativePos.z);
//
//		//	float adjustedVolume = 1.0f - (distance - minDistance) / (maxDistance - minDistance);
//		//	adjustedVolume = std::max(0.0f, std::min(1.0f, adjustedVolume));
//
//		//	//SetVolume(adjustedVolume);
//		//}
//	}
//
//	
//
//private:
//	AudioSystem*  audioSystem;		// References to the same audio system. 
//	FMOD_VECTOR	  listener_pos;		// [3D Audio] - Listener's Position 
//	//FMOD_VECTOR   sound_pos;		// Ideally cycle through each audio's position.
//
//	// Temporary (will swap out with Audio Objects' Position)
//	//float minDistance; // do i need this?
//	//float maxDistance; // do i need this?
//
//};

// Audio Source - audio emitters (that listeners pick up)
class AudioSource
{

};

//class AudioManager : public Singleton<AudioManager>
//{
//	void Init();
//	void Update();
//	void Exit();
//};



