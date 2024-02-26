-- This script controls the activation of enemy script based on EpicIntroTriggerPlatform.lua

local name
local this

local activated = false

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in script!")
    end

    name = string.lower(this:GetGeneral().name)
end

function Update()
    if not activated then

        if (string.find(name, "melissa")) then
            if _G.activateMelissa then activated = true end
        elseif (string.find(name, "soldier")) then 
            if _G.activateTrojanSoldier then activated = true end
        elseif (string.find(name, "horse")) then 
            if _G.activateTrojanHorse then activated = true end
        elseif (string.find(name, "ily")) then 
            if _G.activateILY then activated = true end
        elseif (string.find(name, "zipbomb")) then 
            if _G.activateZipBomb then activated = true end
        end
        
        if activated then ActivateScript(this) end

    end

end

function Dead()
    _G.PreObjectivesCounter = _G.PreObjectivesCounter + 1;
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
    -- print(scriptName)
    Entity:GetScripts():AddScript(Entity, scriptName)
end
