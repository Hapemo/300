local gameStateSys
local graphicssys
local hoverOver

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    graphicssys = systemManager:mGraphicsSystem()
    clickAudioEntity = gameStateSys:GetEntity("Click")
    clickSFX = clickAudioEntity:GetAudio()
    hoverOver = false
end

function Update()
    graphicssys:HideCursor(false)
    button = Helper.GetScriptEntity(script_entity.id)
    if (button:GetButton().mIsHover) then
        
        if(hoverOver == false) then
            hoverSFX = button:GetAudio()
            hoverSFX:SetPlay(0.2)
            hoverOver = true
        end
        
        if (button:GetGeneral().name == "WMReplayButton") then
            button:GetUIrenderer():SetTexture("Replay_Hover")
        elseif (button:GetGeneral().name == "WMMainMenuButton") then
            button:GetUIrenderer():SetTexture("MainMenu_Hover")
        elseif (button:GetGeneral().name == "WMQuitButton") then
            button:GetUIrenderer():SetTexture("Quit_P_Hover")
        end
        
    else
        hoverOver = false
        if (button:GetGeneral().name == "WMReplayButton") then
            button:GetUIrenderer():SetTexture("Replay_Default")
        elseif (button:GetGeneral().name == "WMMainMenuButton") then
            button:GetUIrenderer():SetTexture("MainMenu_Default")
        elseif (button:GetGeneral().name == "WMQuitButton") then
            button:GetUIrenderer():SetTexture("Quit_P_Default")
        end
    end

    if (button:GetButton().mActivated) then
        clickSFX:SetPlay(1.0)
        if (button:GetGeneral().name == "WMReplayButton") then
            gameStateSys:ChangeGameState("Test")
        elseif (button:GetGeneral().name == "WMMainMenuButton") then
            gameStateSys:ChangeGameState("MainMenu")
        elseif (button:GetGeneral().name == "WMQuitButton") then
            systemManager:Quit()
        end
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


