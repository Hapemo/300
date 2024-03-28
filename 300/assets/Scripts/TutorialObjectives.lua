
local this
local progress
local maxProgress
local playerOnObjective
local bar
local complete
local inputMapSys
function Alive()
    progress = 0
    maxProgress = 360
    playerOnObjective = false
    this = Helper.GetScriptEntity(script_entity.id)
    
    complete = false
    inputMapSys = systemManager:mInputActionSystem();
end

function Update()
    gameStateSys = systemManager:mGameStateSystem()
    bar = gameStateSys:GetEntity("ProgressionBar", "JumpAndDash")
    if complete == false then
        if (playerOnObjective) then
            progress = progress + 1
        else 
            progress = progress - 1
            if progress < 0 then
                progress = 0
            end
        end
        if progress == maxProgress then
            complete = true
        end
    end
    bar:GetUIrenderer():SetSlider(progress/maxProgress)
    if complete == true then
        gameStateSys = systemManager:mGameStateSystem()
        inputMapSys = systemManager:mInputActionSystem()
        
            x = gameStateSys:GetEntity("TransitionHelper", "Transition") 
            y = x:GetScripts()
            z = y:GetScript("../assets/Scripts/Transition.lua")

            if z ~= nil then
                z:SetNextGameStateHelper("SetNextGameState", "Test")
                --z:RunFunctionWithParam("SetNextGameState", "Test2")
                z:RunFunction("StartTransition")
            end
    end

    if(_G.Tutorial_Boss_Defeated == true) then 
        EnemyIsKilled()
    end
end

function Dead()
    
end

function OnTriggerEnter(Entity)
    print("start progress")
    general = Entity:GetGeneral()
    if (general.tagid == 0) then
        playerOnObjective = true
    end
end


function OnTriggerExit(Entity) 
    print("end progress")

    general = Entity:GetGeneral()
    if (general.tagid == 0) then
        playerOnObjective = false
    end
end

function EnemyIsKilled()
    this = Helper.GetScriptEntity(script_entity.id)
    this:GetTransform().mTranslate.y = 6.775
    Helper.SetTranslate(this, this:GetTransform().mTranslate)
end