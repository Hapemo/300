/*!*****************************************************************************
\file AudioManager.cpp
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definations for a audio system to play sound in the game
*******************************************************************************/
#include "AudioManager.h"
#include <sys/stat.h>
#include <ECSManager.h>

/*!*****************************************************************************
\brief
    Initializes FMOD

\return
None.
*******************************************************************************/
void AudioManager::Init()
{

    mResult = FMOD::System_Create(&system);      // Create the main system object.
    if (mResult != FMOD_OK)
    {
        //printf("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }

    mResult = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (mResult != FMOD_OK)
    {
        //printf("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
    mChannel.resize((int)E_AUDIO_CHANNEL::MAX);
    songVol = 0.05f;
    bgmVol=1.0f;
    sfxVol=1.f;
    ///LoadSound();
}

/*!*****************************************************************************
\brief
    Load all the sounds needed to FMOD

\return
None.
*******************************************************************************/
FMOD::Sound* AudioManager::LoadAudio(std::filesystem::path const& audio) //Load a sound needed in the game
{
  FMOD::Sound* snd;
  system->createSound(audio.string().c_str(), FMOD_DEFAULT, nullptr, &snd);
  if (audio.parent_path().string() == "..\\resources\\Audio\\SFX") 
  {
      mSfxSound[audio.stem().string()] = snd;
  
  }
  else  if (audio.parent_path().string() == "..\\resources\\Audio\\BGM")
  {
      mBgmSound[audio.stem().string()] = snd;
  }
  else  if (audio.parent_path().string() == "..\\resources\\Audio\\Songs")
  {
      mBgmSound[audio.stem().string()] = snd;
      songs.push_back(audio.stem().string());
  }
  return snd;
}
/*!*****************************************************************************
\brief
    Load all the dialogue sounds needed to FMOD

\return
None.
*******************************************************************************/
void AudioManager::LoadDialogueAudio(std::string audio) //Load a sound needed in the game
{
    for (auto& i : mDialogueSound)
        i.second->release();
    mDialogueSound.clear();
    FMOD::Sound* snd;
    std::string parent = "..\\resources\\Audio\\Dialogue\\";
    std::string rootPath = parent + audio;
    struct stat buffer;
    if (stat(rootPath.c_str(), &buffer) != 0)
    {
        LOG_WARN("No Dialogue Audio files found! ");
        return;
    }
    std::filesystem::path m_CurrentDirectory = std::filesystem::path(rootPath);
    for (auto& directory : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
        const auto& path = directory.path();
        if (path.extension() == ".meta")
            continue;
        system->createSound(path.string().c_str(), FMOD_DEFAULT, nullptr, &snd);
        mDialogueSound[path.stem().string()] = snd;
    }
    
}

/*!*****************************************************************************
\brief
    Update function to play sounds of entities that need to be played

\return
None.
*******************************************************************************/
void AudioManager::UpdateSound()
{
    if (!editorManager->IsScenePaused())
    {
        for (const Entity& e : mEntities)
        {
            assert(e.GetComponent<Audio>().sound.path != "" && "Audio path not loaded");
            if (!e.ShouldRun())
            {
                if (e.GetComponent<Audio>().sound.channel != 0)
                {
                    if (isPlaying(e.GetComponent<Audio>().sound.channel))
                    {
                        mChannel[e.GetComponent<Audio>().sound.channel]->stop();
                    }
                }
                continue;
            }
            if (e.GetComponent<Audio>().sound.toPlay == true)
            {
                if (e.GetComponent<Audio>().sound.channel == 0)
                {
                    e.GetComponent<Audio>().sound.channel = AddChannel();
                }
                //mChannel[e.GetComponent<Audio>().sound.channel]->setVolume(e.GetComponent<Audio>().sound.volume * sfxVol);
                PlayEntitySound(e);
            }
            if (e.GetComponent<Audio>().sound.channel != 0 && isPlaying(e.GetComponent<Audio>().sound.channel))
            {
                int channel = e.GetComponent<Audio>().sound.channel;
                if (e.GetComponent<Audio>().isSpacial && e.HasComponent<Transform>())
                {
                    float vol = e.GetComponent<Audio>().sound.volume;
                    Math::Vec2 distance = e.GetComponent<Transform>().translation - renderManager->GetGameCamera().GetPos();
                    Math::Vec2 max = { renderManager->GetGameCamera().GetCameraWidth() / 2.f * e.GetComponent<Audio>().spacialDistance,
                        renderManager->GetGameCamera().GetCameraHeight() / 2.f * e.GetComponent<Audio>().spacialDistance };

                    float spacial = (max.Magnitude() - distance.Magnitude()) / max.Magnitude();
                    spacial = spacial < 0.f ? 0.f : spacial;
                    if (e.GetComponent<Audio>().spacialRatio > 1.0f)
                        vol = spacial * e.GetComponent<Audio>().spacialRatio;
                    else
                        vol = vol * (1.f - e.GetComponent<Audio>().spacialRatio) + spacial * e.GetComponent<Audio>().spacialRatio;
                    //std::cout << "Spacila: " << spacial << " : " << vol << "\n";
                    mChannel[channel]->setVolume(vol * sfxVol);
                }
                else
                    mChannel[channel]->setVolume(e.GetComponent<Audio>().sound.volume*sfxVol);
                if (e.GetComponent<Audio>().sound.isRandPitch)
                {
                    mChannel[channel]->setPitch(((float)(std::rand() % 50)) / 100.f + 0.75f);
                }
                else
                    mChannel[channel]->setPitch(e.GetComponent<Audio>().sound.pitch);
            }
        }
    }
    else
    {
        for (int i = 20; i < mChannel.size(); i++)
        {
            mChannel[i]->stop();
        }
    }
    bool isAnyPlaying = false;
    for (int i = (int)E_AUDIO_CHANNEL::EDITORSONG+1; i < mChannel.size(); i++)
    {
        if (isPlaying(i))
        {
            isAnyPlaying = true;
            break;
        }
    }
    //static float songVol = 0.05f;
    if (isAnyPlaying)
    {
        songVol = 0.02f;
    }
    else
    {
        if (songVol < 0.6f)
        {
            songVol += 0.002f;
        }
    }
    mChannel[(int)E_AUDIO_CHANNEL::EDITORSONG]->setVolume(songVol);
    //mChannel[(int)E_AUDIO_CHANNEL::MAINBACKGROUND]->setVolume(0.2f+ songVol/4.f); //need manage volume properly aft m3


    system->update();
}


/*!*****************************************************************************
\brief
    Plays a single sound

\return
None.
*******************************************************************************/
void AudioManager::PlayEntitySound(const Entity& _e)
{
    //for (int i = 0; i < e.GetComponent<Audio>().sound.size(); i++) 
    if (!_e.GetComponent<Audio>().sound.isPaused)
    {
        std::string snd = _e.GetComponent<Audio>().sound.path;
        int channel = _e.GetComponent<Audio>().sound.channel;
        //mSfxSound[snd]->setMode(2);
        if (!isPlaying(channel))
        {
            LOG_INFO("Play Entity sound");
            system->playSound(mSfxSound[snd], nullptr, false, &mChannel[channel]);
           
           
        }
    }
    if (!_e.GetComponent<Audio>().sound.isLoop)
    {
        _e.GetComponent<Audio>().sound.toPlay = false;
    }
}
void AudioManager::PlaySound(const Entity& e)
{
    if (!e.HasComponent<Audio>())
        return;
    assert(e.GetComponent<Audio>().sound.path != "" && "Audio path not loaded");
    std::string snd = e.GetComponent<Audio>().sound.path;
    if (e.GetComponent<Audio>().sound.channel == 0)
    {
        e.GetComponent<Audio>().sound.channel = AddChannel();
    }
    int channel = (int)E_AUDIO_CHANNEL::TESTING;
    if (isPlaying(channel))
    {
        mChannel[channel]->stop();
    }
    system->playSound(mSfxSound[snd], nullptr, false, &mChannel[channel]);
    if (e.GetComponent<Audio>().isSpacial && e.HasComponent<Transform>())
    {
        float vol = e.GetComponent<Audio>().sound.volume;
        Math::Vec2 distance = e.GetComponent<Transform>().translation - renderManager->GetGameCamera().GetPos();
        Math::Vec2 max = { renderManager->GetGameCamera().GetCameraWidth() / 2.f * e.GetComponent<Audio>().spacialDistance,
            renderManager->GetGameCamera().GetCameraHeight() / 2.f * e.GetComponent<Audio>().spacialDistance };

        float spacial = (max.Magnitude() - distance.Magnitude()) / max.Magnitude();
        spacial = spacial < 0.f ? 0.f : spacial;
        if (e.GetComponent<Audio>().spacialRatio > 1.0f)
            vol = spacial * e.GetComponent<Audio>().spacialRatio;
        else
            vol = vol * (1.f - e.GetComponent<Audio>().spacialRatio) + spacial * e.GetComponent<Audio>().spacialRatio;
        mChannel[channel]->setVolume(vol * sfxVol);
    }
    else
        mChannel[channel]->setVolume(e.GetComponent<Audio>().sound.volume * sfxVol);
    if (e.GetComponent<Audio>().sound.isRandPitch)
    {
        mChannel[channel]->setPitch(((float)(std::rand() % 100)) / 100.f + 0.5f);
    }
    else
        mChannel[channel]->setPitch(e.GetComponent<Audio>().sound.pitch);

    
}
/*!*****************************************************************************
\brief
    Plays a single background sound

\return
None.
*******************************************************************************/
void AudioManager::PlayAnySound(std::string _snd, int _channel)
{
    /* bool f;
    mChannel[_channel]->isPlaying(&f);*/
    if (isPlaying(_channel))
    {
        mChannel[_channel]->stop();
    }
    
    LOG_INFO("Play a sound");
    if (mBgmSound.find(_snd) != mBgmSound.end())
    {
        mBgmSound[_snd]->setMode(1);
        system->playSound(mBgmSound[_snd], nullptr, false, &mChannel[_channel]);
    }
    else if (mSfxSound.find(_snd) != mSfxSound.end())
    {
        mSfxSound[_snd]->setMode(1);
        system->playSound(mSfxSound[_snd], nullptr, false, &mChannel[_channel]);
    }
    mChannel[_channel]->setVolume(1.5f);
    
}
/*!*****************************************************************************
\brief
    Plays a dialogue naration

\return
None.
*******************************************************************************/
void AudioManager::PlayDialogueSound(std::string _snd, int _channel)
{
    /*bool f;
    mChannel[_channel]->isPlaying(&f);*/
    if (isPlaying(_channel))
    {
        mChannel[_channel]->stop();
    }

    mChannel[_channel]->setVolume(1.0f);
    if (mDialogueSound.find(_snd) != mDialogueSound.end())
    {
        mDialogueSound[_snd]->setMode(1);
        system->playSound(mDialogueSound[_snd], nullptr, false, &mChannel[_channel]);
    }

}
/*!*****************************************************************************
\brief
    Plays a single background sound

\return
None.
*******************************************************************************/
void AudioManager::PlayBGSound(std::string _snd, int _channel)
{
    //_channel  = (int)E_AUDIO_CHANNEL::MAINBACKGROUND;
    mBgmSound[_snd]->setMode(2);
   /* bool f;
    mChannel[_channel]->isPlaying(&f);*/
    if (isPlaying(_channel))
    {
        mChannel[_channel]->stop();
    }
    //LOG_INFO("Play BG sound");
    system->playSound(mBgmSound[_snd], nullptr, false, &mChannel[_channel]);
    mChannel[_channel]->setVolume(bgmVol/2.f);
}
/*!*****************************************************************************
\brief
   Checks if a chanel is playing
\return
None.
*******************************************************************************/
bool AudioManager::isPlaying(int _channel)
{
    bool f;
    mChannel[_channel]->isPlaying(&f);
    return f;
}


/*!*****************************************************************************
\brief
    Set all volume
\param vol
    volume to set to

\return
None.
*******************************************************************************/
void AudioManager::SetALLVolume(float vol)
{
    
    songVol = 0.f;
    bgmVol = vol;
    sfxVol = vol;
    for (int i = 0; i < (int)E_AUDIO_CHANNEL::MAX; i++)
    {
        mChannel[i]->setVolume(vol/2.f);
    }
    for (int i = (int)E_AUDIO_CHANNEL::MAX; i< mChannel.size(); i++)
    {
        mChannel[i]->setVolume(vol);
    }
}
/*!*****************************************************************************
\brief
    Set all volume
\param vol
    volume to set to

\return
None.
*******************************************************************************/
void AudioManager::SetBGMVolume(float vol)
{
    bgmVol = vol;
   for (int i = 0; i<(int) E_AUDIO_CHANNEL::MAX; i++)
   {
       mChannel[i]->setVolume(bgmVol/2.f);
   }
}
/*!*****************************************************************************
\brief
    Set all volume
\param vol
    volume to set to

\return
None.
*******************************************************************************/
void AudioManager::SetSFXVolume(float vol)
{
    /*for (const Entity& e : mEntities)
    {
        e.GetComponent<Audio>().sound.volume = vol;
    }*/
    sfxVol = vol;
}
/*!*****************************************************************************
\brief
    Adds a new chanel

\return
None.
*******************************************************************************/
int AudioManager::AddChannel()
{
    //FMOD::Channel* newChannel;
    // mChannel.emplace_back(newChannel);
    mChannel.resize(mChannel.size() + 1);
    return ((int)mChannel.size() - 1);
}

/*!*****************************************************************************
\brief
    Stop a chanel sound

\return
None.
*******************************************************************************/
void AudioManager::StopSound(int _channel)
{
    mChannel[_channel]->stop();
}
void AudioManager::StopAllSound()
{
    for (int i = 0; i < mChannel.size(); i++)
        mChannel[i]->stop();
}
void AudioManager::StopBGMSound()
{
    for(int i =0; i < 20;i++)
        mChannel[i]->stop();
}
void AudioManager::StopSFXSound()
{
    for (int i = 20; i < mChannel.size(); i++)
        mChannel[i]->stop();
}
/*!*****************************************************************************
\brief
    stop a entity sound

\return
None.
*******************************************************************************/
void AudioManager::StopSound(const Entity& e)
{
    if(e.GetComponent<Audio>().sound.channel != 0)
        mChannel[e.GetComponent<Audio>().sound.channel]->stop();
}

void AudioManager::PauseChannel(int _channel)
{
    mChannel[_channel]->setPaused(true);
}

void AudioManager::PauseAllChannels()
{
	for (int i = 0; i < mChannel.size(); i++) {
		if (i == static_cast<int>(E_AUDIO_CHANNEL::EDITORSONG))
			continue;

		PauseChannel(i);
	}
}

void AudioManager::ResumeChannel(int _channel)
{
    mChannel[_channel]->setPaused(false);
}

void AudioManager::ResumeAllChannels()
{
	if (!editorManager->IsScenePaused()) {
		for (int i = 0; i < mChannel.size(); i++) {
			if (i == static_cast<int>(E_AUDIO_CHANNEL::EDITORSONG))
				continue;

			ResumeChannel(i);
		}
	}
}

/*!*****************************************************************************
\brief
    Deconstructor

\return
None.
*******************************************************************************/
void AudioManager::Unload()
{
    for (auto& i : mSfxSound)
        i.second->release();

    for (auto& i : mBgmSound)
        i.second->release();

    for (auto& i : mDialogueSound)
        i.second->release();

    mChannel.clear();

    system->release();
}
/*!*****************************************************************************
\brief
    Load all the sounds needed to FMOD

\return
None.
*******************************************************************************/
/*void AudioManager::LoadSound() //Load all the sound needed in the game
{
    mChannel.resize(20);
    FMOD::Sound* snd;
    system->createSound("..\\resources\\Audio\\SHOOT1.wav", FMOD_DEFAULT, nullptr, &snd);
    mSfxSound["SHOOT1.wav"] = snd;
    system->createSound("..\\resources\\Audio\\DAMAGE.wav", FMOD_DEFAULT, nullptr, &snd);
    mSfxSound["DAMAGE.wav"] = snd;
    system->createSound("..\\resources\\Audio\\HEALTH.wav", FMOD_DEFAULT, nullptr, &snd);
    mSfxSound["HEALTH.wav"] = snd;
    system->createSound("..\\resources\\Audio\\SHOOT5.wav", FMOD_DEFAULT, nullptr, &snd);
    mSfxSound["SHOOT5.wav"] = snd;
    system->createSound("..\\resources\\Audio\\MENUBG.wav", FMOD_DEFAULT, nullptr, &snd);
    mBgmSound["MENUBG.wav"] = snd;

    //printf("FMOD error: (%d) %s\n", result, FMOD_ErrorString(result));
//}
//////resourceManager->GetResource<FMOD::Sound*>(guid);*/