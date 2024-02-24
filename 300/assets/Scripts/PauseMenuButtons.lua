-- local hoverSFX
-- local hoverOver
-- local centerscreen = Vec2.new()

-- local clickSFX
-- local clickAudioEntity

-- local exitSFX
-- local exitAudioEntity

-- -- mouse attributes
-- local mouse_move = Vec2.new()
local menuBackground
local mainMenuButton
local quitButton
local resumeButton
local htpButton
local restartButton
local HTPMenu
local backButton

gameState = "Test"

_G.isHTPMenu = false

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    -- audioSys    = systemManager:mAudioSystem()
    -- inputMapSys = systemManager:mInputActionSystem()
    graphicssys = systemManager:mGraphicsSystem()

    hoverOver = false

    clickAudioEntity = gameStateSys:GetEntity("Click")
    clickSFX = clickAudioEntity:GetAudio()

    -- exitAudioEntity = gameStateSys:GetEntity("Exit")
    -- exitSFX = exitAudioEntity:GetAudio()

    mainMenuButton = gameStateSys:GetEntity("PauseMainMenuButton")
    quitButton = gameStateSys:GetEntity("PauseQuitButton")
    resumeButton = gameStateSys:GetEntity("PauseResumeButton")
    htpButton = gameStateSys:GetEntity("PauseHTPButton")
    restartButton = gameStateSys:GetEntity("PauseRestartButton")
    HTPMenu = gameStateSys:GetEntity("PauseHTPMenu")
    backButton = gameStateSys:GetEntity("PauseBackButton")
    menuBackground = gameStateSys:GetEntity("PauseMenuBackground")

    _G.isPausePauseMenu = false
    _G.isHTPMenu = false

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
        
        if (button:GetGeneral().name == "PauseResumeButton") then
            button:GetUIrenderer():SetTexture("Resume_Hover")
        elseif (button:GetGeneral().name == "PauseHTPButton") then
            button:GetUIrenderer():SetTexture("HTP_Hover")
        elseif (button:GetGeneral().name == "PauseRestartButton") then
            button:GetUIrenderer():SetTexture("Restart_Hover")
        elseif (button:GetGeneral().name == "PauseQuitButton") then
            button:GetUIrenderer():SetTexture("Quit_P_Hover")
        elseif (button:GetGeneral().name == "PauseMainMenuButton") then
            button:GetUIrenderer():SetTexture("MainMenu_Hover")
        end
    else
        hoverOver = false
        if (button:GetGeneral().name == "PauseResumeButton") then
            button:GetUIrenderer():SetTexture("Resume_Default")
        elseif (button:GetGeneral().name == "PauseHTPButton") then
            button:GetUIrenderer():SetTexture("HTP_Default")
        elseif (button:GetGeneral().name == "PauseRestartButton") then
            button:GetUIrenderer():SetTexture("Restart_Default")
        elseif (button:GetGeneral().name == "PauseQuitButton") then
            button:GetUIrenderer():SetTexture("Quit_P_Default")
        elseif (button:GetGeneral().name == "PauseMainMenuButton") then
            button:GetUIrenderer():SetTexture("MainMenu_Default")
        end
    end
    if (button:GetButton().mActivated) then
        --if (m_EditorSceneHovered == false) then
            clickSFX:SetPlay(1.0)
        --end
        if (button:GetGeneral().name == "PauseResumeButton") then
            _G.mouse_on = true
            graphicssys:HideCursor(true)
            mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000
            systemManager:SetIsPause(false)
            _G.isPausePauseMenu = false
        elseif (button:GetGeneral().name == "PauseHTPButton") then
        mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000
            HTPMenu:GetTransform().mTranslate.x = 0
            backButton:GetTransform().mTranslate.x = 0.65
            _G.isHTPMenu = true
        elseif (button:GetGeneral().name == "PauseBackButton") then
            HTPMenu:GetTransform().mTranslate.x = 1000
            backButton:GetTransform().mTranslate.x = 1000
            _G.isHTPMenu = false
        elseif (button:GetGeneral().name == "PauseRestartButton") then
        mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000 
            _G.isPausePauseMenu = false
            systemManager:SetIsPause(false)
            gameStateSys:ChangeGameState(gameState)
        elseif (button:GetGeneral().name == "PauseMainMenuButton") then
            mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000
            _G.isPausePauseMenu = false
            systemManager:SetIsPause(false)
            gameStateSys:ChangeGameState("MainMenu")
        elseif (button:GetGeneral().name == "PauseQuitButton") then
            mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000
            _G.isPausePauseMenu = false
            systemManager:SetIsPause(false)
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
