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
            --graphicssys:HideCursor(false)
            --print("PAUSE MENU, CURSOR SHOWN")
            --systemManager:Pause()
            systemManager.mIsInGamePause = true
            _G.isPausePauseMenu = true
        end
    end
    if (not _G.isPausePauseMenu) then
        --print("bring AWAYYY menu")
            --graphicssys:HideCursor(true)
            --print("PAUSE MENU, CURSOR HIDDEN")
        mainMenuButton:GetTransform().mTranslate.x = 1000
        quitButton:GetTransform().mTranslate.x = 1000
        resumeButton:GetTransform().mTranslate.x = 1000
        htpButton:GetTransform().mTranslate.x = 1000
        menuBackground:GetTransform().mTranslate.x = 1000
        restartButton:GetTransform().mTranslate.x = 1000
    end
end

function PauseUpdate()
    if (runOnce) then
        runOnce = false
        return
    end

    inputMapSys = systemManager:mInputActionSystem()
    if (inputMapSys:GetButtonDown("pause")) then
        if (_G.isPausePauseMenu) then
            graphicssys:HideCursor(true)
            --print("PAUSE MENU, CURSOR HIDDEN")
            --systemManager:Play()
            systemManager.mIsInGamePause = false
            _G.isPausePauseMenu = false
            runOnce2 = true
        end
    end

    if not _G.isPausePauseMenu then return end

    --print("bring back menu7")
        graphicssys:HideCursor(false)
        Input.SetCursorCenter()
        --print("PAUSE MENU, CURSOR SHOWN")
    if (_G.isHTPMenu == false) then
        mainMenuButton:GetTransform().mTranslate.x = 0.02
        quitButton:GetTransform().mTranslate.x = 0.02
        resumeButton:GetTransform().mTranslate.x = 0.023
        htpButton:GetTransform().mTranslate.x = 0.02
        menuBackground:GetTransform().mTranslate.x = 0
        restartButton:GetTransform().mTranslate.x = 0.02
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


