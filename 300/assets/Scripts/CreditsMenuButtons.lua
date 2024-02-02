local hoverSFX
local hoverOver
local centerscreen = Vec2.new()

local clickSFX
local clickAudioEntity

local exitSFX
local exitAudioEntity

local nextButtonShowPos = Vec3.new()
local backButtonShowPos = Vec3.new()

local buttonHidePos = Vec3.new()

-- mouse attributes
local mouse_move = Vec2.new()

local next1
local next2
local next3
local next4
local next5

local back2
local back3
local back4
local back5
local back6

local creditsBG

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

    nextButtonShowPos.x = 0.175;
    backButtonShowPos.x = -0.88;
    buttonHidePos.x = 3;

    next1 = gameStateSys:GetEntityByScene("Next1Button", "CreditsMenuScene")
    next2 = gameStateSys:GetEntityByScene("Next2Button", "CreditsMenuScene")
    next3 = gameStateSys:GetEntityByScene("Next3Button", "CreditsMenuScene")
    next4 = gameStateSys:GetEntityByScene("Next4Button", "CreditsMenuScene")
    next5 = gameStateSys:GetEntityByScene("Next5Button", "CreditsMenuScene")

    back2 = gameStateSys:GetEntityByScene("Back2Button", "CreditsMenuScene")
    back3 = gameStateSys:GetEntityByScene("Back3Button", "CreditsMenuScene")
    back4 = gameStateSys:GetEntityByScene("Back4Button", "CreditsMenuScene")
    back5 = gameStateSys:GetEntityByScene("Back5Button", "CreditsMenuScene")
    back6 = gameStateSys:GetEntityByScene("Back6Button", "CreditsMenuScene")
    
    creditsBG = gameStateSys:GetEntity("CreditsBG", "CreditsMenuScene")
end

function Update()
    ent = Helper.GetScriptEntity(script_entity.id)

    uirend  = ent:GetUIrenderer()
    button = ent:GetButton()
    gen = ent:GetGeneral()
    trans = ent:GetTransform()

    creditsUIrend = creditsBG:GetUIrenderer()

    -- next (1) button
    if(gen.name == "Next1Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("NextButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            -- hide this button in hidden position
            next1:GetTransform().mTranslate.x = buttonHidePos.x;
            -- show next page's buttons on screen
            next2:GetTransform().mTranslate.x = nextButtonShowPos.x;
            back2:GetTransform().mTranslate.x = backButtonShowPos.x;
            -- change texture to next page
            creditsUIrend:SetTexture("Credits Menu_Programmers_Pg2")
        end
    end

    -- next (2) button
    if(gen.name == "Next2Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
               -- uirend:SetTexture("NextButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            next2:GetTransform().mTranslate.x = buttonHidePos.x;
            back2:GetTransform().mTranslate.x = buttonHidePos.x;
            next3:GetTransform().mTranslate.x = nextButtonShowPos.x;
            back3:GetTransform().mTranslate.x = backButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_Designers_Pg3")
        end
    end

    -- next (3) button
    if(gen.name == "Next3Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("NextButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            next3:GetTransform().mTranslate.x = buttonHidePos.x;
            back3:GetTransform().mTranslate.x = buttonHidePos.x;
            next4:GetTransform().mTranslate.x = nextButtonShowPos.x;
            back4:GetTransform().mTranslate.x = backButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_FacultiesAdvisors__Pg4")
        end
    end

    -- next (4) button
    if(gen.name == "Next4Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("NextButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            next4:GetTransform().mTranslate.x = buttonHidePos.x;
            back4:GetTransform().mTranslate.x = buttonHidePos.x;
            next5:GetTransform().mTranslate.x = nextButtonShowPos.x;
            back5:GetTransform().mTranslate.x = backButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_SpecialThanks__Pg5")
        end
    end

    -- next (5) button
    if(gen.name == "Next5Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("NextButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
           -- uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            next5:GetTransform().mTranslate.x = buttonHidePos.x;
            back5:GetTransform().mTranslate.x = buttonHidePos.x;
            back6:GetTransform().mTranslate.x = backButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_CreatedAt__Pg6")
        end
    end

    -- back (2) button
    if(gen.name == "Back2Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("BackButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("BackButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            next2:GetTransform().mTranslate.x = buttonHidePos.x;
            back2:GetTransform().mTranslate.x = buttonHidePos.x;
            next1:GetTransform().mTranslate.x = nextButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_Programmers_Pg1")
        end
    end

    -- back (3) button
    if(gen.name == "Back3Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("BackButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("BackButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            next3:GetTransform().mTranslate.x = buttonHidePos.x;
            back3:GetTransform().mTranslate.x = buttonHidePos.x;
            next2:GetTransform().mTranslate.x = nextButtonShowPos.x;
            back2:GetTransform().mTranslate.x = backButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_Programmers_Pg2")
        end
    end

    -- back (4) button
    if(gen.name == "Back4Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("BackButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("BackButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            next4:GetTransform().mTranslate.x = buttonHidePos.x;
            back4:GetTransform().mTranslate.x = buttonHidePos.x;
            next3:GetTransform().mTranslate.x = nextButtonShowPos.x;
            back3:GetTransform().mTranslate.x = backButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_Designers_Pg3")
        end
    end

    -- back (5) button
    if(gen.name == "Back5Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("BackButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("BackButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            next5:GetTransform().mTranslate.x = buttonHidePos.x;
            back5:GetTransform().mTranslate.x = buttonHidePos.x;
            next4:GetTransform().mTranslate.x = nextButtonShowPos.x;
            back4:GetTransform().mTranslate.x = backButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_FacultiesAdvisors__Pg4")
        end
    end

    -- back (6) button
    if(gen.name == "Back6Button") then
        -- when the mouse is hovered over the button,
        -- do whatever here (click sound + image change, set the boolean)
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("BackButton_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("BackButton_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            back6:GetTransform().mTranslate.x = buttonHidePos.x;
            next5:GetTransform().mTranslate.x = nextButtonShowPos.x;
            back5:GetTransform().mTranslate.x = backButtonShowPos.x;
            creditsUIrend:SetTexture("Credits Menu_SpecialThanks__Pg5")
        end
    end

    -- menu back button, used in HTP(back) and Credits(Back), can be used for settings next time
    if(gen.name == "MenuBackButton") then
        if (button.mIsHover) then
            if (hoverOver == false) then
                hoverSFX = ent:GetAudio()
                hoverSFX:SetPlay(0.2)
                --uirend:SetTexture("Back_Hover")
                hoverOver = true;
            end
        end
        if (button.mIsHover == false) then
            --uirend:SetTexture("Back_Default")
            hoverOver = false;
        end
        if (button.mActivated) then
            clickSFX:SetPlay(0.2)
            gameStateSys:ChangeGameState("MainMenu")
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
