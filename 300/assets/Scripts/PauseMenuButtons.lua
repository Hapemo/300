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

    -- Resume button
    if(gen.name == "ResumeButton") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Resume_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Resume_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("Test")
        end
    end

    -- how to play button
    if(gen.name == "HTPButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("HTP_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("HTP_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("HTPMenu")
        end
    end

    -- restart level button
    if(gen.name == "RestartButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Restart_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Restart_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("Test")
        end
    end

    -- back to main menu button
    if(gen.name == "MainMenuButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("MainMenu_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("MainMenu_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("MainMenu")
        end
    end

     -- quit button
     if(gen.name == "QuitButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Quit_P_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Quit_P_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
                exitSFX:SetPlay(0.2)
                systemManager:Quit()
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
