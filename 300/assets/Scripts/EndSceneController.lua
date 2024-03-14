local camera
local frame1
local frame2
local frame3
local frame4
local frame5
local frame6
local endCredits
local timer

--states bools
local frame1State = true
local frame2State = false
local frame3State = false
local frame4State = false
local frame5State = false
local frame6State = false
local creditsState = false

local frame3Translate = Vec3.new(0,0,0)
local frame4Translate = Vec3.new(0,0,0)
local frame4Scale = Vec3.new(0,0,0)

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    frame1 = gameStateSys:GetEntity("Frame1E")
    frame2 = gameStateSys:GetEntity("Frame2E")
    frame3 = gameStateSys:GetEntity("Frame3E")
    frame4 = gameStateSys:GetEntity("Frame4E")
    frame5 = gameStateSys:GetEntity("Frame5E")
    frame6 = gameStateSys:GetEntity("Frame6E")
    endCredits = gameStateSys:GetEntity("EndCredits")

    skipButton = gameStateSys:GetEntity("SkipButtonE")
    clickAudioEntity = gameStateSys:GetEntity("Click")

    hoverOver = false

    --generaltimer
    timer = 0
end

-- function TimerCondition(startTime, endTime)
--     return function(timer) return timer >= startTime and timer  endTime end
-- end 

function Update()
    frame3Translate = frame3:GetTransform().mTranslate
    frame4Translate = frame4:GetTransform().mTranslate
    frame4Scale = frame4:GetTransform().mScale
    frame6Scale = frame6:GetTransform().mScale
    
    timer = timer + 1 * FPSManager.GetDT()
    --print(timer)
    -- TIMER CONTROLLER --
    if (timer >= 3 and timer < 8) then
        frame2State = true
        frame1State = false
    elseif (timer >= 8 and timer < 13) then
        frame3State = true
        frame2State = false
    elseif (timer >= 13 and timer < 16) then
        frame4State = true
        frame3State = false
    elseif (timer >= 16 and timer < 21) then
        frame5State = true
        frame4State = false
    elseif (timer >= 21 and timer < 26) then
        frame6State = true
        frame5State = false
    elseif (timer >= 26 and timer < 67) then
        creditsState = true
        frame6State = false
    elseif (timer >= 67) then
        gameStateSys:ChangeGameState("WinMenu")
    end

    -- WHAT IS DONE IN EACH FRAME
    if(frame1State) then
        Frame1()
    elseif(frame2State) then
        Frame2()
    elseif(frame3State) then
        Frame3()
    elseif(frame4State) then
        Frame4()
    elseif(frame5State) then
        Frame5()
    elseif(frame6State) then
        Frame6()
    elseif(creditsState) then
        Credits()
    end

    if (skipButton:GetButton().mIsHover) then
        if(hoverOver == false) then
            hoverSFX = skipButton:GetAudio()
            hoverSFX:SetPlay(0.2)
            hoverOver = true
        end
        skipButton:GetUIrenderer():SetTexture("Skip_Hover")
    else
        hoverOver = false
        skipButton:GetUIrenderer():SetTexture("Skip_Default")
    end

    if (skipButton:GetButton().mActivated) then
        clickSFX = clickAudioEntity:GetAudio()
        clickSFX:SetPlay(1.0)
        gameStateSys:ChangeGameState("WinMenu")
    end
end

function Frame1()

end

function Frame2()
    if (timer >= 3 and timer < 8) then
        if(frame1:GetUIrenderer().mColor.w > 0) then
            frame1:GetUIrenderer().mColor.w = frame1:GetUIrenderer().mColor.w - 0.3 * FPSManager.GetDT()
        end
        if (timer >= 6) then
            frame2:GetUIrenderer().mColor.w = 0
        end
    end
end

function Frame3()
    if (timer >= 8 and timer < 13) then
        if(frame3:GetTransform().mTranslate.x <= 2) then
            frame3:GetTransform().mTranslate.x = frame3:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame4()
    if (timer >= 13 and timer < 16) then
        if(frame4:GetTransform().mTranslate.x <= 2) then
            frame4:GetTransform().mTranslate.x = frame4:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame5()
    if (timer >= 16 and timer < 21) then
        if (timer >= 18) then
            if(frame5:GetUIrenderer().mColor.w > 0) then
                frame5:GetUIrenderer().mColor.w = frame5:GetUIrenderer().mColor.w - 0.2 * FPSManager.GetDT()
            end
        end
    end
end

function Frame6()
    if (timer >= 21 and timer < 26) then
        if (timer >= 21.5 and timer < 24) then
            if (frame6Scale.x < 2.5 and frame6Scale.y > -2.5) then
                frame6Scale.x = frame6Scale.x + 0.25 * FPSManager.GetDT()
                frame6Scale.y = frame6Scale.y - 0.25 * FPSManager.GetDT()
            end
        end
        if (timer >= 23) then
            if(frame6:GetUIrenderer().mColor.w > 0) then
                frame6:GetUIrenderer().mColor.w = frame6:GetUIrenderer().mColor.w - 0.2 * FPSManager.GetDT()
            end
        end
    end
end

function Credits()
    if (timer >= 26 and timer < 67) then
        if (timer >= 26 and timer < 65) then
            if(endCredits:GetTransform().mTranslate.y < 5.5) then
                endCredits:GetTransform().mTranslate.y = endCredits:GetTransform().mTranslate.y + 0.15 * FPSManager.GetDT()
            end
        else
            if(endCredits:GetUIrenderer().mColor.w > 0) then
                endCredits:GetUIrenderer().mColor.w = endCredits:GetUIrenderer().mColor.w - 0.2 * FPSManager.GetDT()
            end
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



