-- This script controls the activation of enemy script based on EpicIntroTriggerPlatform.lua

_G.gameStateSys = systemManager:mGameStateSystem()

local name
local this
local targetEnemy
local targetPlayer
local triggerOnce

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end

    name = string.lower(this:GetGeneral().name)

    if (string.find(name, "melissa")) then
        targetEnemy = _G.gameStateSys:GetEntity("EpicIntroMelissa")
    elseif (string.find(name, "soldier")) then 
        targetEnemy = _G.gameStateSys:GetEntity("EpicIntroTrojanSoldier")
    elseif (string.find(name, "horse")) then 
        targetEnemy = _G.gameStateSys:GetEntity("EpicIntroTrojanHorse")
    elseif (string.find(name, "ily")) then 
        targetEnemy = _G.gameStateSys:GetEntity("EpicIntroILY")
    elseif (string.find(name, "zipbomb")) then 
        targetEnemy = _G.gameStateSys:GetEntity("EpicIntroZipBomb")
    end

    targetPlayer = _G.gameStateSys:GetEntity("Camera")
    triggerOnce = false

    print(targetEnemy)
    print(targetPlayer)
end

function Update()

end

function Dead()
    
end

function OnTriggerEnter(Entity)
    if triggerOnce then return end
    if Entity == targetPlayer then --player id
        triggerOnce = true
        print("OnTriggerEnter")
        _G.TrojanHorseEpicIntroState = 1
        --newpos = this:GetTransform().mTranslate
        --newpos.y = 1000
        --Helper.SetTranslate(this, newpos)
    end
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
        scriptName = "..\\assets\\Scripts\\EpicIntroMelissa.lua"
    elseif (string.find(name, "soldier")) then 
        scriptName = "..\\assets\\Scripts\\EpicIntroTrojanSoldier.lua"
    elseif (string.find(name, "horse")) then 
        scriptName = "..\\assets\\Scripts\\EpicIntroTrojanHorse.lua"
    elseif (string.find(name, "ily")) then 
        scriptName = "..\\assets\\Scripts\\EpicIntroILOVEYOU.lua"
    elseif (string.find(name, "zipbomb")) then 
        scriptName = "..\\assets\\Scripts\\EpicIntroZipBomb.lua"
    end
    -- print(scriptName)
    Entity:GetScripts():AddScript(Entity, scriptName)
end
