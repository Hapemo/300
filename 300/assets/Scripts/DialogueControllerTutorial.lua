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
end

function Update()
    UpdateDialogues()
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
        return
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
        elseif currState == 3
            if Option2Progress >= 1.0 then
                NextDialogue()
            else
                SkipAnimation()
            end
        end
    end

    if currState == 0 then
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
                LoadIn2:GetUIrenderer():SetSlider(LoadIn2progress)
            end
        end
    elseif currState == 1 then
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
                ObjectiveIntro1:GetUIrenderer():SetSlider(1.0)
            else
                ObjectiveIntro1:GetUIrenderer():SetSlider(ObjectiveIntro2Progress)
            end
        end
    elseif currState == 2 then
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
    elseif currState == 3
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
    elseif currState == 3
        Option1Progress = 1.1
        Option2Progress = 1.1
        Option1:GetUIrenderer():SetSlider(1.0)
        Option2:GetUIrenderer():SetSlider(1.0)
    end
end

function NextDialogue()
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
        currState = currState + 1
    elseif currState == 3
        Option1:GetUIrenderer():SetSlider(0.0)
        Option2:GetUIrenderer():SetSlider(0.0)
        currState = currState + 1
    end
end