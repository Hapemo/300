
local name
local this
local target

local aiSys
local scriptingSys

local runOnce = false

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in script!")
    end

    name = string.lower(this:GetGeneral().name)
    target = this:GetAISetting():GetTarget()
    aiSys = systemManager:mAISystem();
    scriptingSys = systemManager:mScriptingSystem();
end

function Update()
    if this:GetHealthbar().health <= 0 then
        --increase count of dead enemy
        _G.PreObjectivesCounter = _G.PreObjectivesCounter + 1;
        --transformed away fr now lol
        systemManager.ecs:SetDeleteEntity(this)
    end

    -- If can see player, activate the scripts accordingly
    if not runOnce and (aiSys:ConeOfSight(this, target, 70, 40)) then
        runOnce = true
        print(name)
        print("sees player")
        ActivateScript(this)
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

function ActivateScript(Entity)
    local scriptName = ""
    -- Check name of entity
    if (string.find(name, "melissa")) then
        scriptName = "..\\assets\\Scripts\\Melissa.lua"
    elseif (string.find(name, "soldier")) then 
        scriptName = "..\\assets\\Scripts\\TrojanSoldier.lua"
    elseif (string.find(name, "horse")) then 
        scriptName = "..\\assets\\Scripts\\TrojanHorse.lua"
    elseif (string.find(name, "ily")) then 
        scriptName = "..\\assets\\Scripts\\ILOVEYOU.lua"
    elseif (string.find(name, "zipbomb")) then 
        scriptName = "..\\assets\\Scripts\\ZipBomb.lua"
    end
    print(scriptName)
    -- scriptingSys:AddScript(Entity, scriptName);
end
