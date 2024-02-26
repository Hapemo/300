local gameStateSys
local graphicssys
local hoverOver

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    graphicssys = systemManager:mGraphicsSystem()
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
        
        if (button:GetGeneral().name == "YesButton") then
            button:GetUIrenderer():SetTexture("Yes_Hover")
        elseif (button:GetGeneral().name == "NoButton") then
            button:GetUIrenderer():SetTexture("No_Hover")
        end
    else
        hoverOver = false
        if (button:GetGeneral().name == "YesButton") then
            button:GetUIrenderer():SetTexture("Yes_Default")
        elseif (button:GetGeneral().name == "NoButton") then
            button:GetUIrenderer():SetTexture("No_Default")
        end
    end

    if (button:GetButton().mActivated) then
        if (button:GetGeneral().name == "YesButton") then
            if (gameStateSys:GetPrevGSName() == "Test") then
                gameStateSys:ChangeGameState("Test")
            elseif (gameStateSys:GetPrevGSName() == "Test2") then
                gameStateSys:ChangeGameState("Test2")
            elseif (gameStateSys:GetPrevGSName() == "Test3") then
                gameStateSys:ChangeGameState("Test3")
            end
        elseif (button:GetGeneral().name == "NoButton") then
            gameStateSys:ChangeGameState("MainMenu")
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


