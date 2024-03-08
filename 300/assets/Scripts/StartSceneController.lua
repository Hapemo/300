local camera
local frame1
local frame2
local frame3
local frame4
local frame5
local frame6
local frame7
local frame8
local timer

--states bools
local frame1State = true
local frame2State = false
local frame3State = false
local frame4State = false
local frame5State = false
local frame6State = false
local frame7State = false
local frame8State = false

local frame3Translate = Vec3.new(0,0,0)
local frame4Translate = Vec3.new(0,0,0)
local frame4Scale = Vec3.new(0,0,0)

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    frame1 = gameStateSys:GetEntity("Frame1")
    frame2 = gameStateSys:GetEntity("Frame2")
    frame3 = gameStateSys:GetEntity("Frame3")
    frame4 = gameStateSys:GetEntity("Frame4")
    frame5 = gameStateSys:GetEntity("Frame5")
    frame6 = gameStateSys:GetEntity("Frame6")
    frame7 = gameStateSys:GetEntity("Frame7")
    frame8 = gameStateSys:GetEntity("Frame8")

    --generaltimer
    timer = 0
end

-- function TimerCondition(startTime, endTime)
--     return function(timer) return timer >= startTime and timer  endTime end
-- end 

function Update()

    frame3Translate = frame3:GetTransform().mTranslate
    frame4Translate = frame4:GetTransform().mTranslate
    frame3Scale = frame3:GetTransform().mScale
    frame4Scale = frame4:GetTransform().mScale

    timer = timer + 1 * FPSManager.GetDT()

    -- TIMER CONTROLLER --
    if (timer >= 3 and timer < 5) then
        frame2State = true
        frame1State = false
    elseif (timer >= 5 and timer < 8.5) then
        frame3State = true
        frame2State = false
    elseif (timer >= 8.5 and timer < 16) then
        frame4State = true
        frame3State = false
    elseif (timer >= 16 and timer < 18) then
        frame5State = true
        frame4State = false
    elseif (timer >= 18 and timer < 20) then
        frame6State = true
        frame5State = false
    elseif (timer >= 20 and timer < 22) then
        frame7State = true
        frame6State = false
    elseif (timer >= 22) then
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
    elseif(frame5State) then
        Frame5()
    elseif(frame6State) then
        Frame6()
    elseif(frame7State) then
        Frame7()
    elseif(frame8State) then
        Frame8()
    end
end

function Frame1()
    if (timer >= 0 and timer < 3) then
        if(frame1:GetTransform().mTranslate.x <= 2) then
            frame1:GetTransform().mTranslate.x = frame1:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame2()
    if (timer >= 3 and timer < 5) then
        if(frame2:GetTransform().mTranslate.x <= 2) then
            frame2:GetTransform().mTranslate.x = frame2:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame3()

end

function Frame4()
    if (timer >= 11 and timer < 16) then
        if(frame4:GetTransform().mTranslate.x <= 4) then
            frame4:GetTransform().mTranslate.x = frame4:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame5()
    if (timer >= 16 and timer < 18) then
        if(frame5:GetTransform().mTranslate.x <= 2) then
            frame5:GetTransform().mTranslate.x = frame5:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame6()
    if (timer >= 18 and timer < 20) then
        if(frame6:GetTransform().mTranslate.x <= 2) then
            frame6:GetTransform().mTranslate.x = frame6:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame7()
    if (timer >= 20 and timer < 22) then
        if(frame7:GetTransform().mTranslate.x <= 2) then
            fram7:GetTransform().mTranslate.x = frame7:GetTransform().mTranslate.x + 1 * FPSManager.GetDT()
        end
    end
end

function Frame8()

end

function Shake()
    if (timer >= 5 and timer < 6.5) then
        if (frame3Scale.x < 2.5 and frame3Scale.y > -2.5) then
            frame3Scale.x = frame3Scale.x + 0.2 * FPSManager.GetDT()
            frame3Scale.y = frame3Scale.y - 0.2 * FPSManager.GetDT()
        end
    end
    if (timer >= 6.5 and timer < 6.55) then
        frame3Translate.x = -0.01
        frame3Translate.y = -0.01
    elseif (timer >= 6.55 and timer < 6.6) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 6.6 and timer < 6.65) then
        frame3Translate.x = 0.01
        frame3Translate.y = 0.01
    elseif (timer >= 6.65 and timer < 6.7) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 6.7 and timer < 6.75) then
        frame3Translate.x = -0.01
        frame3Translate.y = -0.01
    elseif (timer >= 6.75 and timer < 6.8) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 6.8 and timer < 6.85) then
        frame3Translate.x = 0.01
        frame3Translate.y = 0.01
    elseif (timer >= 6.85 and timer < 6.9) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 6.9 and timer < 6.95) then
        frame3Translate.x = -0.01
        frame3Translate.y = -0.01
    elseif (timer >= 6.95 and timer < 7) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    elseif (timer >= 7 and timer < 7.5) then
        frame3Translate.x = 0.01
        frame3Translate.y = 0.01
    elseif (timer >= 7.5 and timer < 8) then
        frame3Translate.x = 0
        frame3Translate.y = 0
    end
    if (timer > 8) then
        frame3:GetTransform().mTranslate.x = 1000
    end
end

function Zoom()
    if (timer >= 8 and timer < 15) then
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


