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

    centerscreen = Input:GetCursorCenter()
    mouse_move.x = Input.CursorPos().x - centerscreen.x
    mouse_move.y = Input.CursorPos().y - centerscreen.y

    print("mouse pos x: ")
    print(mouse_move.x)
    print("mouse pos y: ")
    print(mouse_move.y)

    print("width: ")
    print(graphicsSys.m_WindowWidth)

    print("height: ")
    print(graphicsSys.m_WindowHeight)

    -- print("")

    -- check if cursor is within the button width & height

    -- start button
    if(gen.name == "StartButton") then
        if((mouse_move.x < -(graphicsSys.m_WindowWidth * 0.1875)) and (mouse_move.x > -(graphicsSys.m_WindowWidth * 0.5)) and
            (mouse_move.y < (graphicsSys.m_WindowHeight * 0.2170)) and (mouse_move.y > (graphicsSys.m_WindowHeight * 0.0707))) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                clickSFX:SetPlay(1.0)
                gameStateSys:ChangeGameState("Tutorial")
            end
          
        else
            hoverOver = false
        end
    end

    -- 435, 290
    -- quit button
    if(gen.name == "QuitButton") then
        if((mouse_move.x < -(graphicsSys.m_WindowWidth * 0.1875)) and (mouse_move.x > -(graphicsSys.m_WindowWidth * 0.5)) and
            (mouse_move.y < (graphicsSys.m_WindowHeight * 0.4390)) and (mouse_move.y > (graphicsSys.m_WindowHeight * 0.2926))) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                exitSFX:SetPlay(0.2)
                systemManager:Quit()
            end
        else
            hoverOver = false
        end
    end

    -- 960, 360
    -- 435, 290
    -- credits button
    if(gen.name == "CreditsButton") then
        if((mouse_move.x < (graphicsSys.m_WindowWidth * 0.5)) and (mouse_move.x > (graphicsSys.m_WindowWidth * 0.1875)) and
            (mouse_move.y < (graphicsSys.m_WindowHeight * 0.4390)) and (mouse_move.y > (graphicsSys.m_WindowHeight * 0.2926))) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                clickSFX:SetPlay(1.0)
                gameStateSys:ChangeGameState("CreditsMenu")
            end
        else
            hoverOver = false
        end
    end

    -- credits (back) button
    if(gen.name == "CreditsBackButton") then
        if((mouse_move.x < (graphicsSys.m_WindowWidth * 0.5)) and (mouse_move.x > (graphicsSys.m_WindowWidth * 0.1875)) and
            (mouse_move.y < (graphicsSys.m_WindowHeight * 0.4390)) and (mouse_move.y > (graphicsSys.m_WindowHeight * 0.2926))) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                clickSFX:SetPlay(0.2)
                gameStateSys:ChangeGameState("MainMenu")
            end
        else
            hoverOver = false
        end
    end

    -- restart level button
    if(gen.name == "RetryLevelButton") then
        if((mouse_move.x < (graphicsSys.m_WindowWidth * 0.15625)) and (mouse_move.x > -(graphicsSys.m_WindowWidth * 0.15625)) and
            (mouse_move.y < (graphicsSys.m_WindowHeight * 0.0353)) and (mouse_move.y > -(graphicsSys.m_WindowHeight * 0.0959))) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                clickSFX:SetPlay(1.0)
                gameStateSys:ChangeGameState("Test")
            end
          
        else
            hoverOver = false
        end
    end

    -- back to main menu button
    if(gen.name == "BackToMainMenuButton") then
        if((mouse_move.x < (graphicsSys.m_WindowWidth * 0.15625)) and (mouse_move.x > -(graphicsSys.m_WindowWidth * 0.15625)) and
            (mouse_move.y < (graphicsSys.m_WindowHeight * 0.2371)) and (mouse_move.y > -(graphicsSys.m_WindowHeight * 0.1060))) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                clickSFX:SetPlay(1.0)
                gameStateSys:ChangeGameState("MainMenu")
            end
          
        else
            hoverOver = false
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
