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
    timer = timer + 1 * FPSManager.GetDT()

    -- TIMER CONTROLLER --
    if (timer >= 3 and timer < 5) then
        frame2State = true
        frame1State = false
    elseif (timer >= 5 and timer < 10) then
        frame3State = true
        frame2State = false
    elseif (timer >= 10 and timer < 12) then
        frame4State = true
        frame3State = false
    elseif (timer >= 12 and timer < 14) then
        frame5State = true
        frame4State = false
    elseif (timer >= 14 and timer < 16) then
        frame6State = true
        frame5State = false
    elseif (timer >= 16 and timer < 18) then
        frame7State = true
        frame6State = false
    elseif (timer >= 18 and timer < 20) then
        frame8State = true
        frame7State = false
    elseif (timer >= 20) then
        gameStateSys:ChangeGameState("Tutorial")
    end

    -- WHAT IS DONE IN EACH FRAME
    if(frame1State) then
        Frame1()
    elseif(frame2State) then
        Frame2()
    elseif(frame3State) then
        Frame3()
        Shake()
    elseif(frame4State) then
        Frame4()
    elseif(frame5State) then
        Frame5()
    elseif(frame6State) then
        Frame6()
    elseif(frame7State) then
        Frame7()
    elseif(frame8State) then
        Frame8()
    end

    print(timer)
end

function Frame1()
    
end

function Frame2()
    frame2:GetUIrenderer().mColor.w = 1
    print(frame2:GetUIrenderer().mColor.w)
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

function Frame7()
    frame7:GetUIrenderer().mColor.w = 1
end

function Frame8()
    frame8:GetUIrenderer().mColor.w = 1
end

function Shake()
    if (timer >= 5 and timer < 5.1) then
        frame3:GetTransform().mTranslate.x = -0.03
    elseif (timer >= 5.1 and timer < 5.15) then
        frame3:GetTransform().mTranslate.x = 0
    elseif (timer >= 5.15 and timer < 5.2) then
        frame3:GetTransform().mTranslate.x = 0.03
    elseif (timer >= 5.2 and timer < 5.25) then
        frame3:GetTransform().mTranslate.x = 0
    elseif (timer >= 5.25 and timer < 5.3) then
        frame3:GetTransform().mTranslate.x = -0.03
    elseif (timer >= 5.3 and timer < 5.35) then
        frame3:GetTransform().mTranslate.x = 0
    elseif (timer >= 5.35 and timer < 5.4) then
        frame3:GetTransform().mTranslate.x = 0.03
    elseif (timer >= 5.4 and timer < 5.45) then
        frame3:GetTransform().mTranslate.x = 0
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


