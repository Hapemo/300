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

local VO_good_job
local VO_good_job_done = false

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    frame1 = gameStateSys:GetEntity("Frame1E")
    frame2 = gameStateSys:GetEntity("Frame2E")
    frame3 = gameStateSys:GetEntity("Frame3E")
    frame4 = gameStateSys:GetEntity("Frame4E")
    frame5 = gameStateSys:GetEntity("Frame5E")
    frame6 = gameStateSys:GetEntity("Frame6E")

    skipButton = gameStateSys:GetEntity("SkipButtonE")
    clickAudioEntity = gameStateSys:GetEntity("Click")

    hoverOver = false

    VO_good_job = gameStateSys:GetEntity("VO_Good_Job_Soldier")

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
    -- TIMER CONTROLLER --
    if (timer >= 3 and timer < 8) then
        frame2State = true
        frame1State = false
    elseif (timer >= 8 and timer < 13) then
        frame3State = true
        frame2State = false
    elseif (timer >= 13 and timer < 18) then
        frame4State = true
        frame3State = false
    elseif (timer >= 18 and timer < 23) then
        frame5State = true
        frame4State = false
    elseif (timer >= 23 and timer < 28) then
        frame6State = true
        frame5State = false
    elseif (timer >= 28) then
        gameStateSys:ChangeGameState("WinMenu")
    end

    -- WHAT IS DONE IN EACH FRAME
    if(frame1State) then
        Frame1()
        if VO_good_job_done == false then 
            VO_good_job:GetAudio():SetPlay(1.0)
            VO_good_job_done = true
        end
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
    if (timer >= 13 and timer < 18) then
        if(frame4:GetTransform().mTranslate.x <= 2) then
            frame4:GetTransform().mTranslate.x = frame4:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame5()
    if (timer >= 18 and timer < 23) then
        if(frame5:GetTransform().mTranslate.x <= 2) then
            frame5:GetTransform().mTranslate.x = frame5:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame6()
    if (timer >= 23 and timer < 28) then
        if (timer >= 23.5) then
            if (frame6Scale.x < 2.5 and frame6Scale.y > -2.5) then
                frame6Scale.x = frame6Scale.x + 0.2 * FPSManager.GetDT()
                frame6Scale.y = frame6Scale.y - 0.2 * FPSManager.GetDT()
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



