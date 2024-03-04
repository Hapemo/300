local Intro1
local Intro2
local LoadIn
local ObjectiveBrief1_1
local ObjectiveBrief1_2
local ObjectiveBrief2_1
local ObjectiveBrief2_2
local CompleteObjective1
local CompleteObjective2
local CompleteObjective3_1
local CompleteObjective3_2
local DestroyedAV1
local DestroyedAV2

local Intro1Progress
local Intro2Progress
local LoadInProgress
local ObjectiveBrief1_1Progress
local ObjectiveBrief1_2Progress
local ObjectiveBrief2_1Progress
local ObjectiveBrief2_2Progress
local CompleteObjective1Progress
local CompleteObjective2Progress
local CompleteObjective3_1Progress
local CompleteObjective3_2Progress
local DestroyedAV1Progress
local DestroyedAV2Progress

local currSubState

local currState

local scared
local excited
local normal
local angry

local frame

local frameOpened
local frameClosed
local finished

function Alive()
    Intro1 = gameStateSys:GetEntity("DialogueIntro1", "Dialogue_Level1")
    Intro2 = gameStateSys:GetEntity("DialogueIntro2", "Dialogue_Level1")
    LoadIn = gameStateSys:GetEntity("DialogueLoadIn", "Dialogue_Level1")
    ObjectiveBrief1_1 = gameStateSys:GetEntity("DialogueObjectiveBrief1_1", "Dialogue_Level1")
    ObjectiveBrief1_2 = gameStateSys:GetEntity("DialogueObjectiveBrief1_2", "Dialogue_Level1")
    ObjectiveBrief2_1 = gameStateSys:GetEntity("DialogueObjectiveBrief2_1", "Dialogue_Level1")
    ObjectiveBrief2_2 = gameStateSys:GetEntity("DialogueObjectiveBrief2_2", "Dialogue_Level1")
    CompleteObjective1 = gameStateSys:GetEntity("DialogueCompleteObjective1", "Dialogue_Level1")
    CompleteObjective2 = gameStateSys:GetEntity("DialogueCompleteObjective2", "Dialogue_Level1")
    CompleteObjective3_1 = gameStateSys:GetEntity("DialogueCompleteObjective3_1", "Dialogue_Level1")
    CompleteObjective3_2 = gameStateSys:GetEntity("DialogueCompleteObjective3_2", "Dialogue_Level1")
    EnemyDestroysAV1 = gameStateSys:GetEntity("DialogueEnemyDestroysAV1", "Dialogue_Level1")
    EnemyDestroysAV2 = gameStateSys:GetEntity("DialogueEnemyDestroysAV2", "Dialogue_Level1")

    Intro1Progress = 0.0
    Intro2Progress = 0.0
    LoadInProgress = 0.0
    ObjectiveBrief1_1Progress = 0.0
    ObjectiveBrief1_2Progress = 0.0
    ObjectiveBrief2_1Progress = 0.0
    ObjectiveBrief2_2Progress = 0.0
    CompleteObjective1Progress = 0.0
    CompleteObjective2Progress = 0.0
    CompleteObjective3_1Progress = 0.0
    CompleteObjective3_2Progress = 0.0
    DestroyedAV1Progress = 0.0
    DestroyedAV2Progress = 0.0

    currSubState = 0
    currState = "intro"

    scared = gameStateSys:GetEntity("DialogueScared", "Dialogue_Level1")
    excited = gameStateSys:GetEntity("DialogueExcited", "Dialogue_Level1")
    normal = gameStateSys:GetEntity("DialogueNormal", "Dialogue_Level1")
    angry = gameStateSys:GetEntity("DialogueAngry", "Dialogue_Level1")

    frame = gameStateSys:GetEntity("DialogueFrame", "Dialogue_Level1")

    frameOpened = false
    frameClosed = false
    finish = false
end

function Update()
    if currState ~= "" then
        if frameOpened == false then
            OpenFrame()
        end

        if frameOpened == true and finish == false then
            UpdateDialogues()
        end

        if finish == true and frameClosed == false then
            CloseFrame()
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

function UpdateDialogues()
    if currState == "intro" then
        if inputMapSys:GetButtonDown("skip") then 
            if Intro1Progress >= 1.0 then
            else
            end
        end
    elseif currState == "loadin" then
        
    elseif currState == "obj1" then
        
    elseif currState == "obj2" then
        
    elseif currState == "obj3" then
        
    end
end

function OpenFrame()
    if frame:GetTransform().mScale.x <= 1.99 then
        frame:GetTransform().mScale.x = frame:GetTransform().mScale.x + framespeed
        if frame:GetTransform().mScale.x > 1.98 then
            frame:GetTransform().mScale.x = 2.0
        end
    elseif normal:GetTransform().mScale.x <= 0.24 then
        normal:GetTransform().mScale.x = normal:GetTransform().mScale.x + picturespeed
        if normal:GetTransform().mScale.x > 0.23 then
            normal:GetTransform().mScale.x = 0.25
        end
    else
        frameOpened = true
    end
end

function CloseFrame()
    if normal:GetTransform().mScale.x >= 0.01 then
        normal:GetTransform().mScale.x = normal:GetTransform().mScale.x - picturespeed
        if normal:GetTransform().mScale.x < 0.0 then
            normal:GetTransform().mScale.x = 0.0
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


function FinishedCutscenes()
    currState = "loadin" --yellow
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