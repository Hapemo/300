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

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    -- audioSys    = systemManager:mAudioSystem()
    -- inputMapSys = systemManager:mInputActionSystem()
    -- graphicsSys = systemManager:mGraphicsSystem()

    -- hoverOver = false

    clickAudioEntity = gameStateSys:GetEntity("Click")
    clickSFX = clickAudioEntity:GetAudio()

    -- exitAudioEntity = gameStateSys:GetEntity("Exit")
    -- exitSFX = exitAudioEntity:GetAudio()

    mainMenuButton = gameStateSys:GetEntityByScene("MainMenuButton", "PauseMenuScene")
    quitButton = gameStateSys:GetEntityByScene("QuitButton", "PauseMenuScene")
    resumeButton = gameStateSys:GetEntityByScene("ResumeButton", "PauseMenuScene")
    htpButton = gameStateSys:GetEntityByScene("HTPButton", "PauseMenuScene")
    restartButton = gameStateSys:GetEntityByScene("RestartButton", "PauseMenuScene")
    HTPMenu = gameStateSys:GetEntityByScene("HTPMenu", "PauseMenuScene")
    backButton = gameStateSys:GetEntityByScene("BackButton", "PauseMenuScene")
    menuBackground = gameStateSys:GetEntityByScene("MenuBackground", "PauseMenuScene")

end

function Update()

end

function PauseUpdate()
    ----print("INUPDATEEEEEEEEEEEEEEE")
    button = Helper.GetScriptEntity(script_entity.id)
    if (button:GetButton().mIsHover) then
        ----print("HOVERRRRRRRRRRRRRRRRRRRRRRR")
        hoverSFX = button:GetAudio()
        hoverSFX:SetPlay(0.2)
        if (button:GetGeneral().name == "ResumeButton") then
            button:GetUIrenderer():SetTexture("Resume_Hover")
        elseif (button:GetGeneral().name == "HTPButton") then
            button:GetUIrenderer():SetTexture("HTP_Hover")
        elseif (button:GetGeneral().name == "RestartButton") then
            button:GetUIrenderer():SetTexture("Restart_Hover")
        elseif (button:GetGeneral().name == "QuitButton") then
            button:GetUIrenderer():SetTexture("Quit_P_Hover")
        elseif (button:GetGeneral().name == "MainMenuButton") then
            button:GetUIrenderer():SetTexture("MainMenu_Hover")
        end
    else
        if (button:GetGeneral().name == "ResumeButton") then
            button:GetUIrenderer():SetTexture("Resume_Default")
        elseif (button:GetGeneral().name == "HTPButton") then
            button:GetUIrenderer():SetTexture("HTP_Default")
        elseif (button:GetGeneral().name == "RestartButton") then
            button:GetUIrenderer():SetTexture("Restart_Default")
        elseif (button:GetGeneral().name == "QuitButton") then
            button:GetUIrenderer():SetTexture("Quit_P_Default")
        elseif (button:GetGeneral().name == "MainMenuButton") then
            button:GetUIrenderer():SetTexture("MainMenu_Default")
        end
    end
    if (button:GetButton().mActivated) then
        clickSFX:SetPlay(1.0)
        if (button:GetGeneral().name == "ResumeButton") then
        --print("bring AWAYYY menu1")
        mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000
            systemManager:SetIsPause(false)
            _G.isPausePauseMenu = false
            systemManager:Play()
        elseif (button:GetGeneral().name == "HTPButton") then
        --print("bring AWAYYY menu2")
        mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000
            HTPMenu:GetTransform().mTranslate.x = 0
            backButton:GetTransform().mTranslate.x = 0.65
        elseif (button:GetGeneral().name == "BackButton") then
        --print("bring AWAYYY menu3")
        HTPMenu:GetTransform().mTranslate.x = 1000
            backButton:GetTransform().mTranslate.x = 1000
        elseif (button:GetGeneral().name == "RestartButton") then
        --print("bring AWAYYY menu4")
        mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000 
            _G.isPausePauseMenu = false
            systemManager:SetIsPause(false)
            gameStateSys:ChangeGameState("Test")
        elseif (button:GetGeneral().name == "MainMenuButton") then
            mainMenuButton:GetTransform().mTranslate.x = 1000
        --print("bring AWAYYY menu5")
        quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000
            _G.isPausePauseMenu = false
            systemManager:SetIsPause(false)
            gameStateSys:ChangeGameState("MainMenu")
        elseif (button:GetGeneral().name == "QuitButton") then
        --print("bring AWAYYY menu6")
        mainMenuButton:GetTransform().mTranslate.x = 1000
            quitButton:GetTransform().mTranslate.x = 1000
            resumeButton:GetTransform().mTranslate.x = 1000
            htpButton:GetTransform().mTranslate.x = 1000
            menuBackground:GetTransform().mTranslate.x = 1000
            restartButton:GetTransform().mTranslate.x = 1000
            _G.isPausePauseMenu = false
            systemManager:SetIsPause(false)
            --print("About to Exit")
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
