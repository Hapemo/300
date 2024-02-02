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
            gameStateSys:ChangeGameState("Tutorial")
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
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
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
            gameStateSys:ChangeGameState("MainMenu")
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
