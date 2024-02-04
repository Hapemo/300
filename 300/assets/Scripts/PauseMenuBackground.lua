_G.isPausePauseMenu = false

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    inputMapSys = systemManager:mInputActionSystem()
    mainMenuButton = gameStateSys:GetEntityByScene("MainMenuButton", "PauseMenuScene")
    quitButton = gameStateSys:GetEntityByScene("QuitButton", "PauseMenuScene")
    resumeButton = gameStateSys:GetEntityByScene("ResumeButton", "PauseMenuScene")
    htpButton = gameStateSys:GetEntityByScene("HTPButton", "PauseMenuScene")
    restartButton = gameStateSys:GetEntityByScene("RestartButton", "PauseMenuScene")
end

function Update()
    --print("Update")
    menuBackground = Helper.GetScriptEntity(script_entity.id)
    if (inputMapSys:GetButtonDown("pause")) then
        print("PAUSEEEEEEEEEEE")
        if (not _G.isPausePauseMenu) then
            systemManager:Pause()
            _G.isPausePauseMenu = true
        else
            systemManager:Play()
            _G.isPausePauseMenu = false
        end
    end
    if (not _G.isPausePauseMenu) then
        mainMenuButton:GetTransform().mTranslate.x = 1000
        quitButton:GetTransform().mTranslate.x = 1000
        resumeButton:GetTransform().mTranslate.x = 1000
        htpButton:GetTransform().mTranslate.x = 1000
        menuBackground:GetTransform().mTranslate.x = 1000
        restartButton:GetTransform().mTranslate.x = 1000
    end
end

function PauseUpdate()
    if (inputMapSys:GetKeyDown("pause")) then
        print("PAUSEEEEEEEEEEE")
        if (not _G.isPausePauseMenu) then
            systemManager:Pause()
            _G.isPausePauseMenu = true
        else
            systemManager:Play()
            _G.isPausePauseMenu = false
        end
    end
    menuBackground = Helper.GetScriptEntity(script_entity.id)
    mainMenuButton:GetTransform().mTranslate.x = 0.02
    quitButton:GetTransform().mTranslate.x = 0.02
    resumeButton:GetTransform().mTranslate.x = 0.023
    htpButton:GetTransform().mTranslate.x = 0.02
    menuBackground:GetTransform().mTranslate.x = 0
    restartButton:GetTransform().mTranslate.x = 0.02
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


