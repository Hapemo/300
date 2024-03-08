local Intro1
local Intro2
local Boss

local Intro1Progress
local Intro2Progress
local BossProgress

local currState

local excited
local angry

local frame

local frameOpened
local frameClosed
local finished

local framespeed
local picturespeed

local counter
local picture

local playing
local audio

--neeae
function Alive()
    Intro1 = gameStateSys:GetEntity("DialogueLoadIn1", "Dialogue_Level3")
    Intro2 = gameStateSys:GetEntity("DialogueLoadIn2", "Dialogue_Level3")
    Obj1 = gameStateSys:GetEntity("DialogueCompleteObj1", "Dialogue_Level3")
    Boss = gameStateSys:GetEntity("DialogueBoss", "Dialogue_Level3")
    
    Intro1Progress = 0.0
    Intro2Progress = 0.0
    BossProgress = 0.0

    currState = ""

    excited = gameStateSys:GetEntity("DialogueExcited", "Dialogue_Level3")
    angry = gameStateSys:GetEntity("DialogueAngry", "Dialogue_Level3")
    frame = gameStateSys:GetEntity("DialogueFrame", "Dialogue_Level3")

    frameOpened = false
    frameClosed = false
    finish = false

    speed = 0.01
    framespeed = 0.25
    picturespeed = 0.03125

    counter = 0

    picture = angry

    playing = false
end

function Update()
    if currState ~= "" then
        systemManager.mIsDialogue = true
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

function DialogueUpdate()
        if currState == "intro" then
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
            if currState == "boss" then
                print("going to next gamestate")
                gameStateSys = systemManager:mGameStateSystem()
            
                x = gameStateSys:GetEntity("TransitionHelper", "Transition") 
                y = x:GetScripts()
                z = y:GetScript("../assets/Scripts/Transition.lua")

                if z ~= nil then
                    z:SetNextGameStateHelper("SetNextGameState", "EndCutscene")
                    z:RunFunction("StartTransition")
                end
            end
            frameOpened = false
            frameClosed = false
            finish = false
            currState = ""
            counter = 0
            systemManager.mIsDialogue = false
            
        end
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
            if playing == false then
                audio = Intro1:GetAudio()
                audio:SetPlay(0.3)
                playing = true
            end
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
            if playing == false then
                audio = Boss:GetAudio()
                audio:SetPlay(0.3)
                playing = true
            end
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
        if frame:GetTransform().mScale.x < 0.0 then
            frame:GetTransform().mScale.x = 0.0
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
    elseif currState == "boss" then
        BossProgress = 1.1
        Boss:GetUIrenderer():SetSlider(1.0)
    end
end

function EndDialogue()
    audio:SetStop()
    playing = false
    if currState == "intro" then
        Intro1:GetUIrenderer():SetSlider(0.0)
        Intro2:GetUIrenderer():SetSlider(0.0)
    elseif currState == "boss" then
        Boss:GetUIrenderer():SetSlider(0.0)
    end
end

function StartIntro()
    currState = "intro" --angry
end

function FinishBoss()
    currState = "boss" --yellow
end

function ChangePicture(Entity1, Entity2) 
    Entity1:GetTransform().mScale.x = 0
    Entity2:GetTransform().mScale.x = 0.25
    picture = Entity2
end