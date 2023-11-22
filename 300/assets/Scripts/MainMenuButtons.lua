local hoverSFX
local hoverOver
local centerscreen = Vec2.new()

-- mouse attributes
local mouse_move = Vec2.new()

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    audioSys    = systemManager:mAudioSystem()
    inputMapSys = systemManager:mInputActionSystem()

    hoverOver = false
end

function Update()
    ent = Helper.GetScriptEntity(script_entity.id)

    button = ent:GetButton()
    gen = ent:GetGeneral()
    trans = ent:GetTransform()

    centerscreen = Input:GetCursorCenter()
    mouse_move.x = Input.CursorPos().x - centerscreen.x
    mouse_move.y = Input.CursorPos().y - centerscreen.y

    -- print("mouse pos x: ")
    -- print(mouse_move.x)
    -- print("mouse pos y: ")
    -- print(mouse_move.y)

    -- check if cursor is within the button width & height
    -- lol hardcoded (for char's laptop)

    -- start button
    if(gen.name == "StartButton") then
        if((mouse_move.x < -360) and (mouse_move.x > -960) and
            (mouse_move.y < 215) and (mouse_move.y > 70)) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.3)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                gameStateSys:ChangeGameState("Tutorial")
            end
        else
            hoverOver = false
        end
    end

    -- quit button
    if(gen.name == "QuitButton") then
        if((mouse_move.x < -360) and (mouse_move.x > -960) and
            (mouse_move.y < 435) and (mouse_move.y > 290)) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.3)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                systemManager:Quit()
            end
        else
            hoverOver = false
        end
    end

    -- credits button
    if(gen.name == "CreditsButton") then
        if((mouse_move.x < 960) and (mouse_move.x > 360) and
            (mouse_move.y < 435) and (mouse_move.y > 290)) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.3)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
                gameStateSys:ChangeGameState("CreditsMenu")
            end
        else
            hoverOver = false
        end
    end

    -- credits (back) button
    if(gen.name == "CreditsBackButton") then
        if((mouse_move.x < 960) and (mouse_move.x > 360) and
            (mouse_move.y < 435) and (mouse_move.y > 290)) then
            if not hoverOver then
                hoverOver = true
                -- Play the sound effect
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.3)
            end

            if(inputMapSys:GetButtonDown("Shoot")) then
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
