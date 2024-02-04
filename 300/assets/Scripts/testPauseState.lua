local hoverSFX
local hoverOver
local centerscreen = Vec2.new()

local clickSFX
local clickAudioEntity

local exitSFX
local exitAudioEntity

local pauseButtonShowPos = Vec3.new()

local htp
local restart
local mainmenu
local quit

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    audioSys    = systemManager:mAudioSystem()
    inputMapSys = systemManager:mInputActionSystem()
    graphicsSys = systemManager:mGraphicsSystem()

    hoverOver = false

    clickAudioEntity = gameStateSys:GetEntity("Click")
    clickSFX = clickAudioEntity:GetAudio()

    exitAudioEntity = gameStateSys:GetEntity("Exit")
    exitSFX = exitAudioEntity:GetAudio()

    htp = gameStateSys:GetEntityByScene("HTPButton", "PauseMenu")
    restart = gameStateSys:GetEntityByScene("RestartButton", "PauseMenu")
    mainmenu = gameStateSys:GetEntityByScene("MainMenuButton", "PauseMenu")
    quit = gameStateSys:GetEntityByScene("QuitButton", "PauseMenu")
    
end

function Update()
    pauseMenuUI = Helper.GetScriptEntity(script_entity.id)
    pauseMenuUI:GetTransform().mTranslate.x = 1000;

    uirend  = pauseMenuUI:GetUIrenderer()
    button = pauseMenuUI:GetButton()
    gen = pauseMenuUI:GetGeneral()
    trans = pauseMenuUI:GetTransform()

    -- how to play button
    if(gen.name == "HTPButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = pauseMenuUI:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("HTP_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("HTP_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("HTPMenu")
        end
    end


    -- main menu button
    if(gen.name == "MainMenuButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = pauseMenuUI:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("MainMenu_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("MainMenu_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("MainMenu")
        end
    end

    -- restart level button
    if(gen.name == "RestartButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = pauseMenuUI:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Restart_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Restart_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            gameStateSys:ChangeGameState("Test")
        end
    end

    -- quit button
    if(gen.name == "QuitButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = pauseMenuUI:GetAudio()
                hoverSFX:SetPlay(0.2)
                uirend:SetTexture("Quit_P_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            uirend:SetTexture("Quit_P_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
                exitSFX:SetPlay(0.2)
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

function TriggerPause()
    pauseMenuUI = Helper.GetScriptEntity(script_entity.id)
    pauseMenuUI:GetTransform().mTranslate.x = 0;
    print("came")

    
    htp:GetTransform().mTranslate.x = pauseButtonShowPos.x;
    restart:GetTransform().mTranslate.x = pauseButtonShowPos.x;
    mainmenu:GetTransform().mTranslate.x = pauseButtonShowPos.x;
    quit:GetTransform().mTranslate.x = pauseButtonShowPos.x;

end


