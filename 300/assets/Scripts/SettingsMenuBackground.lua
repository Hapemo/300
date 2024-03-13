function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    inputMapSys = systemManager:mInputActionSystem()
    graphicssys = systemManager:mGraphicsSystem()
    audioSys = systemManager:mAudioSystem()
    settingsMenuBackground = gameStateSys:GetEntity("SettingsMenuBackground")
    settingsBgmFull = gameStateSys:GetEntity("SettingsBgmFull")
    settingSfxFull = gameStateSys:GetEntity("SettingsSfxFull")
    settingsBgmEmpty = gameStateSys:GetEntity("SettingsBgmEmpty")
    settingSfxEmpty = gameStateSys:GetEntity("SettingsSfxEmpty")
    bgmDecrease = gameStateSys:GetEntity("bgmDecrease")
    bgmIncrease = gameStateSys:GetEntity("bgmIncrease")
    sfxDecrease = gameStateSys:GetEntity("sfxDecrease")
    sfxIncrease = gameStateSys:GetEntity("sfxIncrease")

    -- button
    settingsFSButton = gameStateSys:GetEntity("SettingsFSButton")
    settingsWindowButton = gameStateSys:GetEntity("SettingsWindowButton")
    settingsBackButton = gameStateSys:GetEntity("SettingsBackButton")

    clickAudioEntity = gameStateSys:GetEntity("Click")
    clickSFX = clickAudioEntity:GetAudio()
end

function Update()

end

function PauseUpdate()
    graphicssys = systemManager:mGraphicsSystem()
    settingsBgmFull:GetUIrenderer():SetSlider(audioSys:GetBGMVolume())
    settingSfxFull:GetUIrenderer():SetSlider(audioSys:GetSFXVolume())

    if (bgmIncrease:GetButton().mActivated) then
        clickSFX:SetPlay(1.0)
        if (audioSys:GetBGMVolume() <= 0.9) then
            local roundedNewVol = tonumber(string.format("%.1f", audioSys:GetBGMVolume() + 0.1))
            settingsBgmFull:GetUIrenderer():SetSlider(roundedNewVol);
            audioSys:SetAllBGMVolume(roundedNewVol);
            --print("new vol", roundedNewVol)
            --print(audioSys:GetBGMVolume())
        end
    end

    if (bgmDecrease:GetButton().mActivated) then
        clickSFX:SetPlay(1.0)
        if (audioSys:GetBGMVolume() >= 0.1) then
            local roundedNewVol = tonumber(string.format("%.1f", audioSys:GetBGMVolume() - 0.1))
            settingsBgmFull:GetUIrenderer():SetSlider(roundedNewVol);
            audioSys:SetAllBGMVolume(roundedNewVol);
            --print("new vol", roundedNewVol)
            --print(audioSys:GetBGMVolume())
        end
    end

    if (sfxIncrease:GetButton().mActivated) then
        clickSFX:SetPlay(1.0)
        if (audioSys:GetSFXVolume() <= 0.9) then
            local roundedNewVol = tonumber(string.format("%.1f", audioSys:GetSFXVolume() + 0.1))
            settingSfxFull:GetUIrenderer():SetSlider(roundedNewVol);
            audioSys:SetAllSFXVolume(roundedNewVol);
            --print("new vol", roundedNewVol)
            --print(audioSys:GetSFXVolume())
        end
    end

    if (sfxDecrease:GetButton().mActivated) then
        clickSFX:SetPlay(1.0)
        if (audioSys:GetSFXVolume() >= 0.1) then
            local roundedNewVol = tonumber(string.format("%.1f", audioSys:GetSFXVolume() - 0.1))
            settingSfxFull:GetUIrenderer():SetSlider(roundedNewVol);
            audioSys:SetAllSFXVolume(roundedNewVol);
            --print("new vol", roundedNewVol)
            --print(audioSys:GetSFXVolume())
        end
    end

    -- if back button pressed
    if (settingsBackButton:GetButton().mActivated) then
        clickSFX:SetPlay(1.0)
        settingsMenuBackground:GetTransform().mTranslate.x = 1000
        settingsBgmEmpty:GetTransform().mTranslate.x = 1000
        settingsBgmFull:GetTransform().mTranslate.x = 1000
        settingSfxEmpty:GetTransform().mTranslate.x = 1000        settingSfxFull:GetTransform().mTranslate.x = 1000
        settingsFSButton:GetTransform().mTranslate.x = 1000
        settingsWindowButton:GetTransform().mTranslate.x = 1000
        settingsBackButton:GetTransform().mTranslate.x = 1000
        bgmDecrease:GetTransform().mTranslate.x = 1000
        bgmIncrease:GetTransform().mTranslate.x = 1000
        sfxDecrease:GetTransform().mTranslate.x = 1000
        sfxIncrease:GetTransform().mTranslate.x = 1000
        _G.isSettingsMenu = false
    end

    if (settingsFSButton:GetButton().mActivated) then
        graphicssys:SetFullscreenFlag(true)
    end

    if (settingsWindowButton:GetButton().mActivated) then
        graphicssys:SetFullscreenFlag(false)
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


