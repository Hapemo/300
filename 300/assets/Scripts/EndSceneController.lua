local camera
local frame1
local frame2
local frame3
local frame4
local frame5
local frame6
local timer

--states bools
local frame1State = true
local frame2State = false
local frame3State = false
local frame4State = false
local frame5State = false
local frame6State = false

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

    skipButton = gameStateSys:GetEntity("SkipButton")
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

    timer = timer + 1 * FPSManager.GetDT()

    -- TIMER CONTROLLER --
    if (timer >= 3 and timer < 8) then
        frame2State = true
        frame1State = false
    elseif (timer >= 8 and timer < 11) then
        frame3State = true
        frame2State = false
    elseif (timer >= 11 and timer < 14) then
        frame4State = true
        frame3State = false
    elseif (timer >= 14 and timer < 17) then
        frame5State = true
        frame4State = false
    elseif (timer >= 17 and timer < 20) then
        frame6State = true
        frame5State = false
    elseif (timer >= 20) then
        gameStateSys:ChangeGameState("WinMenu")
    end

    -- WHAT IS DONE IN EACH FRAME
    if(frame1State) then
        Frame1()
    elseif(frame2State) then
        Frame2()
    elseif(frame3State) then
        Frame3()
        --Shake()
    elseif(frame4State) then
        Frame4()
        --Zoom()
    elseif(frame5State) then
        Frame5()
    elseif(frame6State) then
        Frame6()
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
    if(frame1:GetUIrenderer().mColor.w > 0) then
        frame1:GetUIrenderer().mColor.w = frame1:GetUIrenderer().mColor.w - 0.3 * FPSManager.GetDT()
    end
end

function Frame3()
    frame3:GetUIrenderer().mColor.w = 1
end

function Frame4()
    frame4:GetUIrenderer().mColor.w = 1
end

function Frame5()
    frame5:GetUIrenderer().mColor.w = 1
end

function Frame6()
    frame6:GetUIrenderer().mColor.w = 1
end

function Shake()
    if (timer >= 5 and timer < 5.1) then
        frame3Translate.x = -0.02
        frame3Translate.y = -0.02
    elseif (timer >= 5.1 and timer < 5.15) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 5.15 and timer < 5.2) then
        frame3Translate.x = 0.02
        frame3Translate.y = 0.02
    elseif (timer >= 5.2 and timer < 5.25) then
        frame3Translatex = 0
        frame3Translatey = 0
    elseif (timer >= 5.25 and timer < 5.3) then
        frame3Translate.x = -0.02
        frame3Translate.y = -0.02
    elseif (timer >= 5.3 and timer < 5.35) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 5.35 and timer < 5.4) then
        frame3Translate.x = 0.02
        frame3Translate.y = 0.02
    elseif (timer >= 5.4 and timer < 5.45) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    end
end

function Zoom()
    if (timer >= 8 and timer < 13) then
        if (frame4Scale.x < 3.2 and frame4Scale.y > -3.2) then
            frame4Scale.x = frame4Scale.x + 0.3 * FPSManager.GetDT()
            frame4Scale.y = frame4Scale.y - 0.3 * FPSManager.GetDT()
        end
        if (frame4Translate.y > -0.4) then
            frame4Translate.y = frame4Translate.y - 0.1 * FPSManager.GetDT()
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



