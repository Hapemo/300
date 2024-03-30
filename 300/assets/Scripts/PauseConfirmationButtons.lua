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

    restartBackground = gameStateSys:GetEntity("RestartBackground")
    quitBackground = gameStateSys:GetEntity("QuitBackground")
    mmBackground = gameStateSys:GetEntity("MainMenuBackground")
    yesButton = gameStateSys:GetEntity("YesButton")
    noButton = gameStateSys:GetEntity("NoButton")

    mainMenuButton = gameStateSys:GetEntity("PauseMainMenuButton")
    quitButton = gameStateSys:GetEntity("PauseQuitButton")
    resumeButton = gameStateSys:GetEntity("PauseResumeButton")
    htpButton = gameStateSys:GetEntity("PauseHTPButton")
    restartButton = gameStateSys:GetEntity("PauseRestartButton")
    settingsButton = gameStateSys:GetEntity("PauseSettingsButton")
    menuBackground = gameStateSys:GetEntity("PauseMenuBackground")
end

function Update()

end

function PauseUpdate()
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
            if (restartBackground:GetTransform().mTranslate.x == 0) then
                _G.isYesRestart = true
                TranslateConfirmationElements()
            elseif (quitBackground:GetTransform().mTranslate.x == 0) then
                _G.isYesQuit = true
                TranslateConfirmationElements()
            elseif (mmBackground:GetTransform().mTranslate.x == 0) then
                _G.isYesMainMenu = true
                --TranslateConfirmationElements()
            end
        elseif (button:GetGeneral().name == "NoButton") then
            TranslateBackPauseElements()
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
    restartBackground:GetTransform().mTranslate.x = 1000
    quitBackground:GetTransform().mTranslate.x = 1000
    mmBackground:GetTransform().mTranslate.x = 1000
    yesButton:GetTransform().mTranslate.x = 1000
    noButton:GetTransform().mTranslate.x = 1000
    _G.isConfirmationMenu = false
end

function TranslateBackPauseElements()
    mainMenuButton:GetTransform().mTranslate.x = 0.02
    quitButton:GetTransform().mTranslate.x = 0.02
    resumeButton:GetTransform().mTranslate.x = 0.023
    htpButton:GetTransform().mTranslate.x = 0.02
    menuBackground:GetTransform().mTranslate.x = 0
    restartButton:GetTransform().mTranslate.x = 0.02
    settingsButton:GetTransform().mTranslate.x = 0.02
end
