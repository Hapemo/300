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

local currState

local scared
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
    DestroyedAV1 = gameStateSys:GetEntity("DialogueEnemyDestroysAV1", "Dialogue_Level1")
    DestroyedAV2 = gameStateSys:GetEntity("DialogueEnemyDestroysAV2", "Dialogue_Level1")

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

    speed = 0.01
    framespeed = 0.25
    picturespeed = 0.03125

    counter = 0

    picture = normal
end

function Update()
    
    if currState ~= "" then
        if currState == "loadin" then
            picture = angry
        elseif currState == "obj1" then
            picture = excited
        elseif currState == "obj2" then
            picture = scared
        elseif currState == "obj3" then
            picture = angry
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
    elseif currState == "loadin" then
        if inputMapSys:GetButtonDown("skip") then 
            if counter == 0 then
                if LoadInProgress >= 1.0 then
                    EndDialogue()
                else
                    SkipAnimation()
                end
            elseif counter == 1 then
                if ObjectiveBrief1_2Progress >= 1.0 then
                    EndDialogue()
                else
                    SkipAnimation()
                end
            elseif counter == 2 then
                if ObjectiveBrief2_2Progress >= 1.0 then
                    EndDialogue()
                    finish = true
                else
                    SkipAnimation()
                end
            end
        end
        if counter == 0 then
            if LoadInProgress < 1.0 then 
                LoadInProgress = speed + LoadInProgress
                if LoadInProgress > 1.0 then
                    LoadInProgress = 1.1
                    LoadIn:GetUIrenderer():SetSlider(1.0)
                else
                    LoadIn:GetUIrenderer():SetSlider(LoadInProgress)
                end
            end
        elseif counter == 1 then
            if ObjectiveBrief1_1Progress < 1.0 then 
                ObjectiveBrief1_1Progress = speed + ObjectiveBrief1_1Progress
                if ObjectiveBrief1_1Progress > 1.0 then
                    ObjectiveBrief1_1Progress = 1.1
                    ObjectiveBrief1_1:GetUIrenderer():SetSlider(1.0)
                else
                    ObjectiveBrief1_1:GetUIrenderer():SetSlider(ObjectiveBrief1_1Progress)
                end
            elseif ObjectiveBrief1_2Progress < 1.0 then
                ObjectiveBrief1_2Progress = speed + ObjectiveBrief1_2Progress
                if ObjectiveBrief1_2Progress > 1.0 then
                    ObjectiveBrief1_2Progress = 1.1
                    ObjectiveBrief1_2:GetUIrenderer():SetSlider(1.0)
                else
                    ObjectiveBrief1_2:GetUIrenderer():SetSlider(ObjectiveBrief1_2Progress)
                end
            end
        elseif counter == 2 then
            if ObjectiveBrief2_1Progress < 1.0 then 
                ObjectiveBrief2_1Progress = speed + ObjectiveBrief2_1Progress
                if ObjectiveBrief2_1Progress > 1.0 then
                    ObjectiveBrief2_1Progress = 1.1
                    ObjectiveBrief2_1:GetUIrenderer():SetSlider(1.0)
                else
                    ObjectiveBrief2_1:GetUIrenderer():SetSlider(ObjectiveBrief2_1Progress)
                end
            elseif ObjectiveBrief2_2Progress < 1.0 then
                ObjectiveBrief2_2Progress = speed + ObjectiveBrief2_2Progress
                if ObjectiveBrief2_2Progress > 1.0 then
                    ObjectiveBrief2_2Progress = 1.1
                    ObjectiveBrief2_2:GetUIrenderer():SetSlider(1.0)
                else
                    ObjectiveBrief2_2:GetUIrenderer():SetSlider(ObjectiveBrief2_2Progress)
                end
            end
        end
    elseif currState == "obj1" then
        if inputMapSys:GetButtonDown("skip") then 
            if CompleteObjective1Progress >= 1.0 then
                EndDialogue()
                finish = true
            else
                SkipAnimation()
            end
        end
        if CompleteObjective1Progress < 1.0 then
            CompleteObjective1Progress = speed + CompleteObjective1Progress
            if CompleteObjective1Progress > 1.0 then
                CompleteObjective1Progress = 1.1
                CompleteObjective1:GetUIrenderer():SetSlider(1.0)
            else
                CompleteObjective1:GetUIrenderer():SetSlider(CompleteObjective1Progress)
            end
        end
    elseif currState == "obj2" then
        if inputMapSys:GetButtonDown("skip") then 
            if CompleteObjective2Progress >= 1.0 then
                EndDialogue()
                finish = true
            else
                SkipAnimation()
            end
        end
        if CompleteObjective2Progress < 1.0 then
            CompleteObjective2Progress = speed + CompleteObjective2Progress
            if CompleteObjective2Progress > 1.0 then
                CompleteObjective2Progress = 1.1
                CompleteObjective2:GetUIrenderer():SetSlider(1.0)
            else
                CompleteObjective2:GetUIrenderer():SetSlider(CompleteObjective2Progress)
            end
        end
    elseif currState == "obj3" then
        if inputMapSys:GetButtonDown("skip") then 
            if counter == 0 then
                if CompleteObjective3_2Progress >= 1.0 then
                    EndDialogue()
                else
                    SkipAnimation()
                end
            elseif counter == 1 then
                if DestroyedAV2Progress >= 1.0 then
                    EndDialogue()
                    finish = true
                else
                    SkipAnimation()
                end
            end
        end
        if counter == 0 then
            if CompleteObjective3_1Progress < 1.0 then 
                CompleteObjective3_1Progress = speed + CompleteObjective3_1Progress
                if CompleteObjective3_1Progress > 1.0 then
                    CompleteObjective3_1Progress = 1.1
                    CompleteObjective3_1:GetUIrenderer():SetSlider(1.0)
                else
                    CompleteObjective3_1:GetUIrenderer():SetSlider(CompleteObjective3_1Progress)
                end
            elseif CompleteObjective3_2Progress < 1.0 then
                CompleteObjective3_2Progress = speed + CompleteObjective3_2Progress
                if CompleteObjective3_2Progress > 1.0 then
                    CompleteObjective3_2Progress = 1.1
                    CompleteObjective3_2:GetUIrenderer():SetSlider(1.0)
                else
                    CompleteObjective3_2:GetUIrenderer():SetSlider(CompleteObjective3_2Progress)
                end
            end
        elseif counter == 1 then
            if DestroyedAV1Progress < 1.0 then 
                DestroyedAV1Progress = speed + DestroyedAV1Progress
                if DestroyedAV1Progress > 1.0 then
                    DestroyedAV1Progress = 1.1
                    DestroyedAV1:GetUIrenderer():SetSlider(1.0)
                else
                    DestroyedAV1:GetUIrenderer():SetSlider(DestroyedAV1Progress)
                end
            elseif DestroyedAV2Progress < 1.0 then
                DestroyedAV2Progress = speed + DestroyedAV2Progress
                if DestroyedAV2Progress > 1.0 then
                    DestroyedAV2Progress = 1.1
                    DestroyedAV2:GetUIrenderer():SetSlider(1.0)
                else
                    DestroyedAV2:GetUIrenderer():SetSlider(DestroyedAV2Progress)
                end
            end
        end
    end
end

function OpenFrame()
    _G.FreezePlayerControl = true
    -- print("IM IN OPEN FRAME: " , _G.)

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
    _G.FreezePlayerControl = false

    if currState == "obj3" then 
        picture = normal
    end
    if picture:GetTransform().mScale.x >= 0.01 then
        picture:GetTransform().mScale.x = picture:GetTransform().mScale.x - picturespeed
        if picture:GetTransform().mScale.x < 0.0 then
            picture:GetTransform().mScale.x = 0.0
        end
    elseif frame:GetTransform().mScale.x >= 0.01 then
        frame:GetTransform().mScale.x = frame:GetTransform().mScale.x - framespeed
        if frame:GetTransform().mScale.x < 0.01 then
            frame:GetTransform().mScale.x = 0.0

            if currState =="obj3" then
                gameStateSys = systemManager:mGameStateSystem()
                
                x = gameStateSys:GetEntity("TransitionHelper", "Transition") 
                y = x:GetScripts()
                z = y:GetScript("../assets/Scripts/Transition.lua")
                if z ~= nil then
                    z:SetNextGameStateHelper("SetNextGameState", "Test2")
                    --z:RunFunctionWithParam("SetNextGameState", "Test2")
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
    elseif currState == "loadin" then
        if counter == 0 then
            LoadInProgress = 1.1
            LoadIn:GetUIrenderer():SetSlider(1.0)
        elseif counter == 1 then
            ObjectiveBrief1_1Progress = 1.1
            ObjectiveBrief1_2Progress = 1.1
            ObjectiveBrief1_1:GetUIrenderer():SetSlider(1.0)
            ObjectiveBrief1_2:GetUIrenderer():SetSlider(1.0)
        elseif counter == 2 then
            ObjectiveBrief2_1Progress = 1.1
            ObjectiveBrief2_2Progress = 1.1
            ObjectiveBrief2_1:GetUIrenderer():SetSlider(1.0)
            ObjectiveBrief2_2:GetUIrenderer():SetSlider(1.0)
        end
    elseif currState == "obj1" then
        CompleteObjective1Progress = 1.1
        CompleteObjective1:GetUIrenderer():SetSlider(1.0)
    elseif currState == "obj2" then
        CompleteObjective2Progress = 1.1
        CompleteObjective2:GetUIrenderer():SetSlider(1.0)
    elseif currState == "obj3" then
        if counter == 0 then
            CompleteObjective3_1Progress = 1.1
            CompleteObjective3_2Progress = 1.1
            CompleteObjective3_1:GetUIrenderer():SetSlider(1.0)
            CompleteObjective3_2:GetUIrenderer():SetSlider(1.0)
        elseif counter == 1 then
            DestroyedAV1Progress = 1.1
            DestroyedAV2Progress = 1.1
            DestroyedAV1:GetUIrenderer():SetSlider(1.0)
            DestroyedAV2:GetUIrenderer():SetSlider(1.0)
        end
    end
end

function EndDialogue()
    if currState == "intro" then
        Intro1:GetUIrenderer():SetSlider(0.0)
        Intro2:GetUIrenderer():SetSlider(0.0)
    elseif currState == "loadin" then
        if counter == 0 then
            LoadIn:GetUIrenderer():SetSlider(0.0)
            counter = 1
            ChangePicture(angry, normal)
        elseif counter == 1 then
            ObjectiveBrief1_1:GetUIrenderer():SetSlider(0.0)
            ObjectiveBrief1_2:GetUIrenderer():SetSlider(0.0)
            counter = 2
            ChangePicture(normal, angry)
        elseif counter == 2 then
            ObjectiveBrief2_1:GetUIrenderer():SetSlider(0.0)
            ObjectiveBrief2_2:GetUIrenderer():SetSlider(0.0)
            counter = 3
        end
    elseif currState == "obj1" then
        CompleteObjective1:GetUIrenderer():SetSlider(0.0)
    elseif currState == "obj2" then
        CompleteObjective2:GetUIrenderer():SetSlider(0.0)
    elseif currState == "obj3" then
        if counter == 0 then
            CompleteObjective3_1:GetUIrenderer():SetSlider(0.0)
            CompleteObjective3_2:GetUIrenderer():SetSlider(0.0)
            counter = 1
            ChangePicture(angry, normal)
        elseif counter == 1 then
            DestroyedAV1:GetUIrenderer():SetSlider(0.0)
            DestroyedAV2:GetUIrenderer():SetSlider(0.0)
            counter = 2
        end
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

function ChangePicture(Entity1, Entity2) 
    Entity1:GetTransform().mScale.x = 0
    Entity2:GetTransform().mScale.x = 0.25
    picture = Entity2
end