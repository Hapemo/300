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

    button = ent:GetButton()
    gen = ent:GetGeneral()
    trans = ent:GetTransform()

    -- print("mouse pos x: ")
    -- print(mouse_move.x)
    -- print("mouse pos y: ")
    -- print(mouse_move.y)

    -- print("width: ")
    -- print(graphicsSys.m_WindowWidth)

    -- print("height: ")
    -- print(graphicsSys.m_WindowHeight)

    -- print("")

    -- check if cursor is within the button width & height

    -- start button
    if(gen.name == "StartButton") then
        if (button.mIsHover) then
            hoverSFX = ent:GetAudio()
            hoverSFX:SetPlay(0.2)
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("Tutorial")
        end
    end

    -- 435, 290
    -- quit button
    if(gen.name == "QuitButton") then
        if (button.mIsHover) then
            hoverSFX = ent:GetAudio()
            hoverSFX:SetPlay(0.2)
        end
        if (button.mActivated) then
                exitSFX:SetPlay(0.2)
                systemManager:Quit()
        end
    end

    -- 960, 360
    -- 435, 290
    -- credits button
    if(gen.name == "CreditsButton") then
        if (button.mIsHover) then
            hoverSFX = ent:GetAudio()
            hoverSFX:SetPlay(0.2)
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("CreditsMenu")
        end
    end

    -- credits (back) button
    if(gen.name == "CreditsBackButton") then
        if (button.mIsHover) then
            hoverSFX = ent:GetAudio()
            hoverSFX:SetPlay(0.2)
        end
        if (button.mActivated) then
            clickSFX:SetPlay(0.2)
            gameStateSys:ChangeGameState("MainMenu")
        end
    end

    -- restart level button
    if(gen.name == "RetryLevelButton") then
        if (button.mIsHover) then
            hoverSFX = ent:GetAudio()
            hoverSFX:SetPlay(0.2)
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("Test")
        end
    end

    -- back to main menu button
    if(gen.name == "BackToMainMenuButton") then
        if (button.mIsHover) then
            hoverSFX = ent:GetAudio()
            hoverSFX:SetPlay(0.2)
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
