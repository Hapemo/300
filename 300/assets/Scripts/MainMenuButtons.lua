local hoverSFX
local hoverOver
local centerscreen = Vec2.new()

local clickSFX
local clickAudioEntity

local exitSFX
local exitAudioEntity

-- mouse attributes
local mouse_move = Vec2.new()

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    audioSys    = systemManager:mAudioSystem()
    inputMapSys = systemManager:mInputActionSystem()
    graphicsSys = systemManager:mGraphicsSystem()

    hoverOver = false

    clickAudioEntity = gameStateSys:GetEntity("Click")
    clickSFX = clickAudioEntity:GetAudio()

    exitAudioEntity = gameStateSys:GetEntity("Exit")
    exitSFX = exitAudioEntity:GetAudio()

    --main menu
    startButton = gameStateSys:GetEntity("StartButton")
    menuBGButton = gameStateSys:GetEntity("MenuBackground")
    HTPButton = gameStateSys:GetEntity("HTPButton")
    settingsButton = gameStateSys:GetEntity("SettingsButton")
    creditsButton = gameStateSys:GetEntity("CreditsButton")
    quitButton = gameStateSys:GetEntity("QuitButton")

    --settings
    settingsMenuBackground = gameStateSys:GetEntity("SettingsMenuBackground")
    settingsBgmEmpty = gameStateSys:GetEntity("SettingsBgmEmpty")
    settingsBgmFull = gameStateSys:GetEntity("SettingsBgmFull")
    settingSfxFull = gameStateSys:GetEntity("SettingsSfxFull")
    settingSfxEmpty = gameStateSys:GetEntity("SettingsSfxEmpty")
    gammaFull = gameStateSys:GetEntity("gammaFull")
    gammaEmpty = gameStateSys:GetEntity("gammaEmpty")
    settingsFSButton = gameStateSys:GetEntity("SettingsFSButton")
    settingsWindowButton = gameStateSys:GetEntity("SettingsWindowButton")
    settingsBackButton = gameStateSys:GetEntity("SettingsBackButton")
    bgmDecrease = gameStateSys:GetEntity("bgmDecrease")
    bgmIncrease = gameStateSys:GetEntity("bgmIncrease")
    sfxDecrease = gameStateSys:GetEntity("sfxDecrease")
    sfxIncrease = gameStateSys:GetEntity("sfxIncrease")
    gammaDecrease = gameStateSys:GetEntity("gammaDecrease")
    gammaIncrease = gameStateSys:GetEntity("gammaIncrease")

    restartBackground = gameStateSys:GetEntity("RestartBackground")
    quitBackground = gameStateSys:GetEntity("QuitBackground")
    mmBackground = gameStateSys:GetEntity("MainMenuBackground")
    yesButton = gameStateSys:GetEntity("YesButton")
    noButton = gameStateSys:GetEntity("NoButton")
end

function Update()
    ent = Helper.GetScriptEntity(script_entity.id)

    uirend  = ent:GetUIrenderer()
    button = ent:GetButton()
    gen = ent:GetGeneral()
    trans = ent:GetTransform()

    -- start button
    if(gen.name == "StartButton") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Start_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Start_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("StartCutscene")
        end
    end

    -- quit button
    if(gen.name == "QuitButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Quit_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Quit_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
                TranslateQuitElements()
                quitBackground:GetTransform().mTranslate.x = 0
                yesButton:GetTransform().mTranslate.x = -0.2
                noButton:GetTransform().mTranslate.x = 0.2
                _G.isConfirmationMenu = true
        end

        if (_G.isYesQuit == true) then
            _G.isYesQuit = false
            exitSFX:SetPlay(0.2)
            systemManager:Quit()
        end
    end

    -- how to play button
    if(gen.name == "HTPButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("HowToPlay_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("HowToPlay_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("HTPMenu")
        end
    end

    -- settings button
    if(gen.name == "SettingsButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Settings_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Settings_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            startButton:GetTransform().mTranslate.x = 1000
            menuBGButton:GetTransform().mTranslate.x = 1000
            HTPButton:GetTransform().mTranslate.x = 1000
            settingsButton:GetTransform().mTranslate.x = 1000
            creditsButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000

            settingsMenuBackground:GetTransform().mTranslate.x = 0
            settingsBgmEmpty:GetTransform().mTranslate.x = -0.20
            settingsBgmFull:GetTransform().mTranslate.x = -0.20
            settingSfxEmpty:GetTransform().mTranslate.x = -0.20
            settingSfxFull:GetTransform().mTranslate.x = -0.20
            gammaFull:GetTransform().mTranslate.x = -0.20
            gammaEmpty:GetTransform().mTranslate.x = -0.20
            settingsFSButton:GetTransform().mTranslate.x = -0.59
            settingsWindowButton:GetTransform().mTranslate.x = -0.08
            settingsBackButton:GetTransform().mTranslate.x = 0.85
            bgmDecrease:GetTransform().mTranslate.x = -0.61
            bgmIncrease:GetTransform().mTranslate.x = 0.21
            sfxDecrease:GetTransform().mTranslate.x = -0.61
            sfxIncrease:GetTransform().mTranslate.x = 0.21
            gammaDecrease:GetTransform().mTranslate.x = -0.61
            gammaIncrease:GetTransform().mTranslate.x = 0.21
        end
    end

    -- credits button
    if(gen.name == "CreditsButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Credits_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Credits_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("CreditsMenu")
        end
    end

    -- credits next button
    if(gen.name == "CreditsNextButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(0.2)
            gameStateSys:ChangeGameState("CreditsMenu2")
        end
    end

    -- credits back button
    if(gen.name == "CreditsBackButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(0.2)
            gameStateSys:ChangeGameState("CreditsMenu")
        end
    end

    -- menu back button, used in HTP(back) and Credits(Back), can be used for settings next time
    if(gen.name == "MenuBackButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Back_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
           uirend:SetTexture("Back_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(0.2)
            gameStateSys:ChangeGameState("MainMenu")
        end
    end

    -- restart level button
    if(gen.name == "RetryLevelButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("Test")
        end
    end

    -- back to main menu button
    if(gen.name == "BackToMainMenuButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("Menu")
        end
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function TranslateQuitElements()
    startButton:GetTransform().mTranslate.x = 1000
    HTPButton:GetTransform().mTranslate.x = 1000
    settingsButton:GetTransform().mTranslate.x = 1000
    creditsButton:GetTransform().mTranslate.x = 1000
    quitButton:GetTransform().mTranslate.x = 1000
end
