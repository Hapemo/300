local LoadIn1
local LoadIn2
local ObjectiveIntro1
local ObjectiveIntro2
local TroubleIntro1
local TroubleIntro2
local TroubleIntro3
local Option1
local Option2


local LoadIn1Progress
local LoadIn2Progress
local ObjectiveIntro1Progress
local ObjectiveIntro2Progress
local TroubleIntro1Progress
local TroubleIntro2Progress
local TroubleIntro3Progress
local Option1Progress
local Option2Progress

local currState

local speed
local framespeed
local picturespeed

local frame
local picture
local angry

local frameOpened
local frameClosed
local done
local boss
local playing
local audio
--:GetUIrenderer():SetSlider(progress/objectivesComplete)

function Alive()
    gameStateSys = systemManager:mGameStateSystem()

    LoadIn1 = gameStateSys:GetEntity("DialogueLoadIn1", "Dialogue_Tutorial")
    LoadIn2 = gameStateSys:GetEntity("DialogueLoadIn2", "Dialogue_Tutorial")
    ObjectiveIntro1 = gameStateSys:GetEntity("DialogueObjectiveIntro1", "Dialogue_Tutorial")
    ObjectiveIntro2 = gameStateSys:GetEntity("DialogueObjectiveIntro2", "Dialogue_Tutorial")
    TroubleIntro1 = gameStateSys:GetEntity("DialogueTroubleIntro1", "Dialogue_Tutorial")
    TroubleIntro2 = gameStateSys:GetEntity("DialogueTroubleIntro2", "Dialogue_Tutorial")
    TroubleIntro3 = gameStateSys:GetEntity("DialogueTroubleIntro3", "Dialogue_Tutorial")
    Option1 = gameStateSys:GetEntity("DialogueOption1", "Dialogue_Tutorial")
    Option2 = gameStateSys:GetEntity("DialogueOption2", "Dialogue_Tutorial")

    frame = gameStateSys:GetEntity("DialogueFrame", "Dialogue_Tutorial")
    picture = gameStateSys:GetEntity("DialogueNormal", "Dialogue_Tutorial")
    angry = gameStateSys:GetEntity("DialogueAngry", "Dialogue_Tutorial")

    boss = gameStateSys:GetEntity("DialogueBoss", "Dialogue_Tutorial")

    LoadIn1Progress = 0.0
    LoadIn2Progress = 0.0
    ObjectiveIntro1Progress = 0.0
    ObjectiveIntro2Progress = 0.0
    TroubleIntro1Progress = 0.0
    TroubleIntro2Progress = 0.0
    TroubleIntro3Progress = 0.0
    Option1Progress = 0.0
    Option2Progress = 0.0

    currState = 0

    speed = 0.01
    framespeed = 0.25
    picturespeed = 0.03125

    frameOpened = false

    done = false;

    frameClosed = false

    playing = false
end

function Update()
    if frameOpened == false then
        OpenFrame()
    end

    if frameOpened == true and done == false then
        UpdateDialogues()
    end

    if done == true and frameClosed == false then
        CloseFrame()
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
    inputMapSys = systemManager:mInputActionSystem()

    if currState > 3 then
        done = true
    end

    if inputMapSys:GetButtonDown("skip") then 
        if currState == 0 then
            if LoadIn2Progress >= 1.0 then
                NextDialogue()
            else
                SkipAnimation()
            end
        elseif currState == 1 then
            if ObjectiveIntro2Progress >= 1.0 then
                NextDialogue()
            else
                SkipAnimation()
            end
        elseif currState == 2 then
            if TroubleIntro3Progress >= 1.0 then
                NextDialogue()
            else
                SkipAnimation()
            end
        elseif currState == 3 then
            if Option2Progress >= 1.0 then
                NextDialogue()
            else
                SkipAnimation()
            end
        end
    end

    if currState == 0 then
        if playing == false then
            audio = LoadIn1:GetAudio()
            audio:SetPlay(0.3)
            playing = true
        end
        if LoadIn1Progress < 1.0 then 
            LoadIn1Progress = speed + LoadIn1Progress
            if LoadIn1Progress > 1.0 then
                LoadIn1Progress = 1.1
                LoadIn1:GetUIrenderer():SetSlider(1.0)
            else
                LoadIn1:GetUIrenderer():SetSlider(LoadIn1Progress)
            end
        else 
            LoadIn2Progress = speed + LoadIn2Progress
            if LoadIn2Progress > 1.0 then
                LoadIn2Progress = 1.1
                LoadIn2:GetUIrenderer():SetSlider(1.0)
            else
                LoadIn2:GetUIrenderer():SetSlider(LoadIn2Progress)
            end
        end
    elseif currState == 1 then
        if playing == false then
            audio = ObjectiveIntro1:GetAudio()
            audio:SetPlay(0.3)
            playing = true
        end
        if ObjectiveIntro1Progress < 1.0 then 
            ObjectiveIntro1Progress = speed + ObjectiveIntro1Progress
            if ObjectiveIntro1Progress > 1.0 then
                ObjectiveIntro1Progress = 1.1
                ObjectiveIntro1:GetUIrenderer():SetSlider(1.0)
            else
                ObjectiveIntro1:GetUIrenderer():SetSlider(ObjectiveIntro1Progress)
            end
        else 
            ObjectiveIntro2Progress = speed + ObjectiveIntro2Progress
            if ObjectiveIntro2Progress > 1.0 then
                ObjectiveIntro2Progress = 1.1
                ObjectiveIntro2:GetUIrenderer():SetSlider(1.0)
            else
                ObjectiveIntro2:GetUIrenderer():SetSlider(ObjectiveIntro2Progress)
            end
        end
    elseif currState == 2 then
        if playing == false then
            audio = TroubleIntro1:GetAudio()
            audio:SetPlay(0.3)
            playing = true
        end
        boss:GetUIrenderer():SetSlider(1.0)
        picture:GetUIrenderer():SetSlider(0.0)
        angry:GetUIrenderer():SetSlider(1.0)

        if TroubleIntro1Progress < 1.0 then 
            TroubleIntro1Progress = speed + TroubleIntro1Progress
            if TroubleIntro1Progress > 1.0 then
                TroubleIntro1Progress = 1.1
                TroubleIntro1:GetUIrenderer():SetSlider(1.0)
            else
                TroubleIntro1:GetUIrenderer():SetSlider(TroubleIntro1Progress)
            end
        elseif TroubleIntro2Progress < 1.0 then
            TroubleIntro2Progress = speed + TroubleIntro2Progress
            if TroubleIntro2Progress > 1.0 then
                TroubleIntro2Progress = 1.1
                TroubleIntro2:GetUIrenderer():SetSlider(1.0)
            else
                TroubleIntro2:GetUIrenderer():SetSlider(TroubleIntro2Progress)
            end
        else 
            TroubleIntro3Progress = speed + TroubleIntro3Progress
            if TroubleIntro3Progress > 1.0 then
                TroubleIntro3Progress = 1.1
                TroubleIntro3:GetUIrenderer():SetSlider(1.0)
            else
                TroubleIntro3:GetUIrenderer():SetSlider(TroubleIntro3Progress)
            end
        end
    elseif currState == 3 then
        if playing == false then
            audio = Option1:GetAudio()
            audio:SetPlay(0.3)
            playing = true
        end
        if Option1Progress < 1.0 then 
            Option1Progress = speed + Option1Progress
            if Option1Progress > 1.0 then
                Option1Progress = 1.1
                Option1:GetUIrenderer():SetSlider(1.0)
            else
                Option1:GetUIrenderer():SetSlider(Option1Progress)
            end
        else 
            Option2Progress = speed + Option2Progress
            if Option2Progress > 1.0 then
                Option2Progress = 1.1
                Option2:GetUIrenderer():SetSlider(1.0)
            else
                Option2:GetUIrenderer():SetSlider(Option2Progress)
            end
        end
    end
end

function SkipAnimation()
    if currState == 0 then
        LoadIn1Progress = 1.1
        LoadIn2Progress = 1.1
        LoadIn1:GetUIrenderer():SetSlider(1.0)
        LoadIn2:GetUIrenderer():SetSlider(1.0)
    elseif currState == 1 then
        ObjectiveIntro1Progress = 1.1
        ObjectiveIntro2Progress = 1.1
        ObjectiveIntro1:GetUIrenderer():SetSlider(1.0)
        ObjectiveIntro2:GetUIrenderer():SetSlider(1.0)
    elseif currState == 2 then
        TroubleIntro1Progress = 1.1
        TroubleIntro2Progress = 1.1
        TroubleIntro3Progress = 1.1
        TroubleIntro1:GetUIrenderer():SetSlider(1.0)
        TroubleIntro2:GetUIrenderer():SetSlider(1.0)
        TroubleIntro3:GetUIrenderer():SetSlider(1.0)
    elseif currState == 3 then
        Option1Progress = 1.1
        Option2Progress = 1.1
        Option1:GetUIrenderer():SetSlider(1.0)
        Option2:GetUIrenderer():SetSlider(1.0)
    end
end

function NextDialogue()
    audio:SetStop()
    playing = false
    if currState == 0 then
        LoadIn1:GetUIrenderer():SetSlider(0.0)
        LoadIn2:GetUIrenderer():SetSlider(0.0)
        currState = currState + 1
    elseif currState == 1 then
        ObjectiveIntro1:GetUIrenderer():SetSlider(0.0)
        ObjectiveIntro2:GetUIrenderer():SetSlider(0.0)
        currState = currState + 1
    elseif currState == 2 then
        TroubleIntro1:GetUIrenderer():SetSlider(0.0)
        TroubleIntro2:GetUIrenderer():SetSlider(0.0)
        TroubleIntro3:GetUIrenderer():SetSlider(0.0)
        boss:GetUIrenderer():SetSlider(0.0)
        picture:GetUIrenderer():SetSlider(1.0)
        angry:GetUIrenderer():SetSlider(0.0)
        currState = currState + 1
    elseif currState == 3 then
        Option1:GetUIrenderer():SetSlider(0.0)
        Option2:GetUIrenderer():SetSlider(0.0)
        currState = currState + 1
    end
end

function OpenFrame()
    picture = gameStateSys:GetEntity("DialogueNormal", "Dialogue_Tutorial")
    _G.FreezePlayerControl = true
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
    picture = gameStateSys:GetEntity("DialogueNormal", "Dialogue_Tutorial")
    _G.FreezePlayerControl = false
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
