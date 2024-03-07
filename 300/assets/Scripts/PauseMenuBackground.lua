_G.isPausePauseMenu = false
runOnce = false
runOnce2 = false

local gameStateSys
local inputMapSys
local mainMenuButton
local quitButton
local resumeButton
local htpButton
local restartButton
local menuBackground
local graphicssys

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    inputMapSys = systemManager:mInputActionSystem()
    graphicssys = systemManager:mGraphicsSystem()
    mainMenuButton = gameStateSys:GetEntity("PauseMainMenuButton")
    quitButton = gameStateSys:GetEntity("PauseQuitButton")
    resumeButton = gameStateSys:GetEntity("PauseResumeButton")
    htpButton = gameStateSys:GetEntity("PauseHTPButton")
    restartButton = gameStateSys:GetEntity("PauseRestartButton")
    settingsButton = gameStateSys:GetEntity("PauseSettingsButton")
    menuBackground = gameStateSys:GetEntity("PauseMenuBackground")
    _G.isPausePauseMenu = false
end

function Update()
    if (runOnce2) then
        runOnce2 = false
        return
    end

    if (inputMapSys:GetButtonDown("pause")) then
        runOnce = true
        if (not _G.isPausePauseMenu) then
            systemManager.mIsInGamePause = true
            _G.isPausePauseMenu = true
        end
    end
    if (not _G.isPausePauseMenu) then
        mainMenuButton:GetTransform().mTranslate.x = 1000
        quitButton:GetTransform().mTranslate.x = 1000
        resumeButton:GetTransform().mTranslate.x = 1000
        htpButton:GetTransform().mTranslate.x = 1000
        menuBackground:GetTransform().mTranslate.x = 1000
        restartButton:GetTransform().mTranslate.x = 1000
        settingsButton:GetTransform().mTranslate.x = 1000
    end
end

function PauseUpdate()
    if (runOnce) then
        runOnce = false
        return
    end

    inputMapSys = systemManager:mInputActionSystem()
    if (inputMapSys:GetButtonDown("pause")) then
        if (_G.isPausePauseMenu and not _G.isHTPMenu and not _G.isSettingsMenu) then
            graphicssys:HideCursor(true)
            systemManager.mIsInGamePause = false
            _G.isPausePauseMenu = false
            runOnce2 = true
        end
    end

    if not _G.isPausePauseMenu then return end
        graphicssys:HideCursor(false)
        Input.SetCursorCenter()
    --print(_G.isSettingsMenu)
    if (not _G.isHTPMenu and not _G.isSettingsMenu) then
        mainMenuButton:GetTransform().mTranslate.x = 0.02
        quitButton:GetTransform().mTranslate.x = 0.02
        resumeButton:GetTransform().mTranslate.x = 0.023
        htpButton:GetTransform().mTranslate.x = 0.02
        menuBackground:GetTransform().mTranslate.x = 0
        restartButton:GetTransform().mTranslate.x = 0.02
        settingsButton:GetTransform().mTranslate.x = 0.02
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


