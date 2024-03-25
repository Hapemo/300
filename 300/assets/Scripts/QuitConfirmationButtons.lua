local restartBackground
local quitBackground
local mmBackground
local yesButton
local noButton

gameState = "Test"

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    graphicssys = systemManager:mGraphicsSystem()

    hoverOver = false

    clickAudioEntity = gameStateSys:GetEntity("Click")
    clickSFX = clickAudioEntity:GetAudio()

    startButton = gameStateSys:GetEntity("StartButton")
    menuBGButton = gameStateSys:GetEntity("MenuBackground")
    HTPButton = gameStateSys:GetEntity("HTPButton")
    settingsButton = gameStateSys:GetEntity("SettingsButton")
    creditsButton = gameStateSys:GetEntity("CreditsButton")
    quitButton = gameStateSys:GetEntity("QuitButton")

    restartBackground = gameStateSys:GetEntity("RestartBackground")
    quitBackground = gameStateSys:GetEntity("QuitBackground")
    mmBackground = gameStateSys:GetEntity("MainMenuBackground")
    yesButton = gameStateSys:GetEntity("YesButton")
    noButton = gameStateSys:GetEntity("NoButton")


end

function Update()
    --print("INUPDATEEEEEEEEEEEEEEE")
    button = Helper.GetScriptEntity(script_entity.id)
    if (button:GetButton().mIsHover) then
        --print("HOVERRRRRRRRRRRRRRRRRRRRRRR")

        if(hoverOver == false) then
            hoverSFX = button:GetAudio()
            --if (m_EditorSceneHovered == false) then
                hoverSFX:SetPlay(0.2)
            --end
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
        --if (m_EditorSceneHovered == false) then
            clickSFX:SetPlay(1.0)
        --end
        if (button:GetGeneral().name == "YesButton") then
            if (quitBackground:GetTransform().mTranslate.x == 0) then
                _G.isYesQuit = true
            end
        elseif (button:GetGeneral().name == "NoButton") then
            startButton:GetTransform().mTranslate.x = -0.447
            menuBGButton:GetTransform().mTranslate.x = 0
            HTPButton:GetTransform().mTranslate.x = -0.45
            settingsButton:GetTransform().mTranslate.x = -0.46
            creditsButton:GetTransform().mTranslate.x = -0.452
            quitButton:GetTransform().mTranslate.x = -0.45
            TranslateConfirmationElements()
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

function TranslateConfirmationElements()
    quitBackground:GetTransform().mTranslate.x = 1000
    yesButton:GetTransform().mTranslate.x = 1000
    noButton:GetTransform().mTranslate.x = 1000
    _G.isConfirmationMenu = false
end
