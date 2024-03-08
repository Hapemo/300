local Intro1
local Intro2
local Obj1
local Obj2
local Obj3
local Boss

local Intro1Progress
local Intro2Progress
local Obj1Progress
local Obj2Progress
local Obj3Progress
local BossProgress

local currState

local excited
local normal
local angry

local frame

local frameOpened
local frameClosed
local finished

local framespeed
local picturespeed

local counter
local picture

--neeae
function Alive()
    Intro1 = gameStateSys:GetEntity("DialogueLoadIn1", "Dialogue_Level2")
    Intro2 = gameStateSys:GetEntity("DialogueLoadIn2", "Dialogue_Level2")
    Obj1 = gameStateSys:GetEntity("DialogueCompleteObj1", "Dialogue_Level2")
    Obj2 = gameStateSys:GetEntity("DialogueCompleteObj2", "Dialogue_Level2")
    Obj3 = gameStateSys:GetEntity("DialogueCompleteObj3", "Dialogue_Level2")
    Boss = gameStateSys:GetEntity("DialogueCompleteBoss", "Dialogue_Level2")
    
    Intro1Progress = 0.0
    Intro2Progress = 0.0
    Obj1Progress = 0.0
    Obj2Progress = 0.0
    Obj3Progress = 0.0
    BossProgress = 0.0

    currState = "intro"

    excited = gameStateSys:GetEntity("DialogueExcited", "Dialogue_Level2")
    normal = gameStateSys:GetEntity("DialogueNormal", "Dialogue_Level2")
    angry = gameStateSys:GetEntity("DialogueAngry", "Dialogue_Level2")
    frame = gameStateSys:GetEntity("DialogueFrame", "Dialogue_Level2")

    frameOpened = false
    frameClosed = false
    finish = false

    speed = 0.01
    framespeed = 0.25
    picturespeed = 0.03125

    counter = 0

    picture = normal
end

function Update()
    if currState ~= "" then
        systemManager.mIsDialogue = true
    end
end

function DialogueUpdate()
    if currState == "obj1" then
        picture = excited
    elseif currState == "obj2" then
        picture = excited
    elseif currState == "obj3" then
        picture = angry
    elseif currState == "boss" then
        picture = excited
    end

    if frameOpened == false then
        OpenFrame()
    end

    if frameOpened == true and finish == false then
        UpdateDialogues()
    end

    if finish == true and frameClosed == false then
        CloseFrame()
    end

    if finish == true and frameClosed == true then
        frameOpened = false
        frameClosed = false
        finish = false
        currState = ""
        counter = 0
        systemManager.mIsDialogue = false
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

function UpdateDialogues()
    if currState == "intro" then
        if inputMapSys:GetButtonDown("skip") then 
            if Intro2Progress >= 1.0 then
                EndDialogue()
                finish = true
            else
                SkipAnimation()
            end
        end

        if Intro1Progress < 1.0 then 
            Intro1Progress = speed + Intro1Progress
            if Intro1Progress > 1.0 then
                Intro1Progress = 1.1
                Intro1:GetUIrenderer():SetSlider(1.0)
            else
                Intro1:GetUIrenderer():SetSlider(Intro1Progress)
            end
        elseif Intro2Progress < 1.0 then
            Intro2Progress = speed + Intro2Progress
            if Intro2Progress > 1.0 then
                Intro2Progress = 1.1
                Intro2:GetUIrenderer():SetSlider(1.0)
            else
                Intro2:GetUIrenderer():SetSlider(Intro2Progress)
            end
        end
    elseif currState == "obj1" then
        if inputMapSys:GetButtonDown("skip") then 
            if Obj1Progress >= 1.0 then
                EndDialogue()
                finish = true
            else
                SkipAnimation()
            end
        end

        if Obj1Progress < 1.0 then 
            Obj1Progress = speed + Obj1Progress
            if Obj1Progress > 1.0 then
                Obj1Progress = 1.1
                Obj1:GetUIrenderer():SetSlider(1.0)
            else
                Obj1:GetUIrenderer():SetSlider(Obj1Progress)
            end
        end
    elseif currState == "obj2" then
        if inputMapSys:GetButtonDown("skip") then 
            if Obj2Progress >= 1.0 then
                EndDialogue()
                finish = true
            else
                SkipAnimation()
            end
        end

        if Obj2Progress < 1.0 then 
            Obj2Progress = speed + Obj2Progress
            if Obj2Progress > 1.0 then
                Obj2Progress = 1.1
                Obj2:GetUIrenderer():SetSlider(1.0)
            else
                Obj2:GetUIrenderer():SetSlider(Obj2Progress)
            end
        end
    elseif currState == "obj3" then
        if inputMapSys:GetButtonDown("skip") then 
            if Obj3Progress >= 1.0 then
                EndDialogue()
                finish = true
            else
                SkipAnimation()
            end
        end

        if Obj3Progress < 1.0 then 
            Obj3Progress = speed + Obj3Progress
            if Obj3Progress > 1.0 then
                Obj3Progress = 1.1
                Obj3:GetUIrenderer():SetSlider(1.0)
            else
                Obj3:GetUIrenderer():SetSlider(Obj3Progress)
            end
        end
    elseif currState == "boss" then
        if inputMapSys:GetButtonDown("skip") then 
            if BossProgress >= 1.0 then
                EndDialogue()
                finish = true
            else
                SkipAnimation()
            end
        end

        if BossProgress < 1.0 then 
            BossProgress = speed + BossProgress
            if BossProgress > 1.0 then
                BossProgress = 1.1
                Boss:GetUIrenderer():SetSlider(1.0)
            else
                Boss:GetUIrenderer():SetSlider(BossProgress)
            end
        end
    end
end

function OpenFrame()
    if frame:GetTransform().mScale.x <= 1.99 then
        frame:GetTransform().mScale.x = frame:GetTransform().mScale.x + framespeed
        if frame:GetTransform().mScale.x > 1.98 then
            frame:GetTransform().mScale.x = 2.0
        end
    elseif picture:GetTransform().mScale.x <= 0.24 then
        picture:GetTransform().mScale.x = picture:GetTransform().mScale.x + picturespeed
        if picture:GetTransform().mScale.x > 0.23 then
            picture:GetTransform().mScale.x = 0.25
        end
    else
        frameOpened = true
    end
end

function CloseFrame()
    if picture:GetTransform().mScale.x >= 0.01 then
        picture:GetTransform().mScale.x = picture:GetTransform().mScale.x - picturespeed
        if picture:GetTransform().mScale.x < 0.0 then
            picture:GetTransform().mScale.x = 0.0
        end
    elseif frame:GetTransform().mScale.x >= 0.01 then
        frame:GetTransform().mScale.x = frame:GetTransform().mScale.x - framespeed
        if frame:GetTransform().mScale.x < 0.01 then
            frame:GetTransform().mScale.x = 0.0
            if currState == "boss" then
                gameStateSys = systemManager:mGameStateSystem()
            
                x = gameStateSys:GetEntity("TransitionHelper", "Transition") 
                y = x:GetScripts()
                z = y:GetScript("../assets/Scripts/Transition.lua")

                if z ~= nil then
                    z:SetNextGameStateHelper("SetNextGameState", "Test3")
                    z:RunFunction("StartTransition")
                end
            end
        end
    else
        frameClosed = true
    end
end

function SkipAnimation()
    if currState == "intro" then
        Intro1Progress = 1.1
        Intro2Progress = 1.1
        Intro1:GetUIrenderer():SetSlider(1.0)
        Intro2:GetUIrenderer():SetSlider(1.0)
    elseif currState == "obj1" then
        Obj1Progress = 1.1
        Obj1:GetUIrenderer():SetSlider(1.0)
    elseif currState == "obj2" then
        Obj2Progress = 1.1
        Obj2:GetUIrenderer():SetSlider(1.0)
    elseif currState == "obj3" then
        Obj3Progress = 1.1
        Obj3:GetUIrenderer():SetSlider(1.0)
    elseif currState == "boss" then
        BossProgress = 1.1
        Boss:GetUIrenderer():SetSlider(1.0)
    end
end

function EndDialogue()
    if currState == "intro" then
        Intro1:GetUIrenderer():SetSlider(0.0)
        Intro2:GetUIrenderer():SetSlider(0.0)
    elseif currState == "obj1" then
        Obj1:GetUIrenderer():SetSlider(0.0)
    elseif currState == "obj2" then
        Obj2:GetUIrenderer():SetSlider(0.0)
    elseif currState == "obj3" then
        Obj3:GetUIrenderer():SetSlider(0.0)
    elseif currState == "boss" then
        Boss:GetUIrenderer():SetSlider(0.0)
    end
end

function FinishObjective1()
    currState = "obj1" --cyan
end

function FinishObjective2()
    currState = "obj2" --blue
end

function FinishObjective3()
    currState = "obj3" --light cyan
end

function FinishBoss()
    currState = "boss" --yellow
end

function ChangePicture(Entity1, Entity2) 
    Entity1:GetTransform().mScale.x = 0
    Entity2:GetTransform().mScale.x = 0.25
    picture = Entity2
end