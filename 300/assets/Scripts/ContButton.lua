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
        button:GetUIrenderer():SetTexture("Continue_Hover")
    else
        hoverOver = false
        button:GetUIrenderer():SetTexture("Continue_Default")
    end

    if (button:GetButton().mActivated) then
        clickSFX:SetPlay(1.0)
        if (gameStateSys:GetPrevGSName() == "Test") then
            gameStateSys:ChangeGameState("Test2")
        elseif (gameStateSys:GetPrevGSName() == "Test2") then
            gameStateSys:ChangeGameState("Test3")
        elseif (gameStateSys:GetPrevGSName() == "Test3") then
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


