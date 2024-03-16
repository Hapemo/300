local camera
local frame1
local frame2
local frame3
local frame4
local frame5
local frame6
local frame7
local timer

--states bools
local frame1State = true
local frame2State = false
local frame3State = false
local frame4State = false
local frame5State = false
local frame6State = false
local frame7State = false

local frame3Translate = Vec3.new(0,0,0)
local frame4Translate = Vec3.new(0,0,0)
local frame4Scale = Vec3.new(0,0,0)

local VO_obj
local VO_obj2
local VO_played = false
local VO_played2 = false
local customized_delay = 1.0

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    frame1 = gameStateSys:GetEntity("Frame1")
    frame2 = gameStateSys:GetEntity("Frame2")
    frame3 = gameStateSys:GetEntity("Frame3")
    frame4 = gameStateSys:GetEntity("Frame4")
    frame5 = gameStateSys:GetEntity("Frame5")
    frame6 = gameStateSys:GetEntity("Frame6")
    frame7 = gameStateSys:GetEntity("Frame7")
    skipButton = gameStateSys:GetEntity("SkipButton")
    clickAudioEntity = gameStateSys:GetEntity("Click")

    VO_obj = gameStateSys:GetEntity("VO_ItCantBe")
    VO_obj2 = gameStateSys:GetEntity("VO_ShesHere")

    hoverOver = false

    --generaltimer
    timer = 0
end

-- function TimerCondition(startTime, endTime)
--     return function(timer) return timer >= startTime and timer  endTime end
-- end 

function Update()
    -- customized_delay = customized_delay + FPSManager.GetDT()
    -- print("CUSTOMIZED DELAY" , customized_delay)

    frame3Translate = frame3:GetTransform().mTranslate
    frame4Translate = frame4:GetTransform().mTranslate
    frame3Scale = frame3:GetTransform().mScale
    frame4Scale = frame4:GetTransform().mScale

    timer = timer + 1 * FPSManager.GetDT()
    -- TIMER CONTROLLER --
    if (timer >= 5 and timer < 10) then
        frame2State = true
        frame1State = false
    elseif (timer >= 10 and timer < 15) then
        frame3State = true
        frame2State = false
    elseif (timer >= 15 and timer < 20) then
        frame4State = true
        frame3State = false
    elseif (timer >= 20 and timer < 25) then
        frame5State = true
        frame4State = false
    elseif (timer >= 25 and timer < 30) then
        frame6State = true
        frame5State = false
    elseif (timer >= 30 and timer < 35) then
        frame7State = true
        frame6State = false
    elseif (timer >= 38) then
        gameStateSys:ChangeGameState("Tutorial")
    end

    -- WHAT IS DONE IN EACH FRAME
    if(frame1State) then
        Frame1()
    elseif(frame2State) then
        Frame2()
    elseif(frame3State) then
        Shake()
    elseif(frame4State) then
        Frame4()
        Zoom()
        customized_delay = customized_delay + FPSManager.GetDT()
        if customized_delay >= 1 then
            if VO_played == false then
                VO_obj:GetAudio():SetPlay(1.0)
                VO_played = true
            end
        end
    elseif(frame5State) then
        Frame5()
        if VO_played2 == false then
            VO_obj2:GetAudio():SetPlay(1.0)
            VO_played2 = true
        end
    elseif(frame6State) then
        Frame6()
    elseif(frame7State) then
        Frame7()
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
        gameStateSys:ChangeGameState("Tutorial")
    end
end

function Frame1()
    if (timer >= 0 and timer < 5) then
        if (timer > 3) then
            if(frame1:GetTransform().mTranslate.x <= 2) then
                frame1:GetTransform().mTranslate.x = frame1:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
            end
        end
    end
end

function Frame2()
    if (timer >= 5 and timer < 10) then
        if (timer > 8) then
            if(frame2:GetTransform().mTranslate.x <= 2) then
                frame2:GetTransform().mTranslate.x = frame2:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
            end
        end
    end
end

function Frame3()

end

function Frame4()
    if (timer >= 15 and timer < 20) then
        if (timer > 18) then
            if(frame4:GetTransform().mTranslate.x <= 4) then
                frame4:GetTransform().mTranslate.x = frame4:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
            end
        end
    end
end

function Frame5()
    if (timer >= 20 and timer < 25) then
        if (timer > 23) then
            if(frame5:GetTransform().mTranslate.x <= 2) then
                frame5:GetTransform().mTranslate.x = frame5:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
            end
        end
    end
end

function Frame6()
    if (timer >= 25 and timer < 30) then
        if (timer > 28) then
            if(frame6:GetTransform().mTranslate.x <= 2) then
                frame6:GetTransform().mTranslate.x = frame6:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
            end
        end
    end
end

function Frame7()
    if (timer >= 30 and timer < 35) then
        if (timer > 33) then
            if(frame7:GetUIrenderer().mColor.w > 0) then
                print("came")
                frame7:GetUIrenderer().mColor.w = frame7:GetUIrenderer().mColor.w - 0.3 * FPSManager.GetDT()
            end
        end
    end
end

function Shake()
    if (timer >= 10 and timer < 10.5) then
        if (frame3Scale.x < 2.5 and frame3Scale.y > -2.5) then
            frame3Scale.x = frame3Scale.x + 0.2 * FPSManager.GetDT()
            frame3Scale.y = frame3Scale.y - 0.2 * FPSManager.GetDT()
        end
    end
    if (timer >= 10.5 and timer < 10.55) then
        frame3Translate.x = -0.01
        frame3Translate.y = -0.01
    elseif (timer >= 10.55 and timer < 10.6) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 10.6 and timer < 10.65) then
        frame3Translate.x = 0.01
        frame3Translate.y = 0.01
    elseif (timer >= 10.65 and timer < 10.7) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 10.7 and timer < 10.75) then
        frame3Translate.x = -0.01
        frame3Translate.y = -0.01
    elseif (timer >= 10.75 and timer < 10.8) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 10.8 and timer < 10.85) then
        frame3Translate.x = 0.01
        frame3Translate.y = 0.01
    elseif (timer >= 10.85 and timer < 10.9) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 10.9 and timer < 10.95) then
        frame3Translate.x = -0.01
        frame3Translate.y = -0.01
    elseif (timer >= 10.95 and timer < 11) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 11 and timer < 11.5) then
        frame3Translate.x = 0.01
        frame3Translate.y = 0.01
    end
    if (timer > 14) then
        frame3:GetTransform().mTranslate.x = 1000
    end
end

function Zoom()
    if (timer >= 15 and timer < 20) then
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


