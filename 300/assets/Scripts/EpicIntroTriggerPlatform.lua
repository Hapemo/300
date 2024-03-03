-- This script controls the activation of enemy script based on EpicIntroTriggerPlatform.lua
-- Will only run once when triggered

_G.gameStateSys = systemManager:mGameStateSystem()

local name
local this
local targetPlayer
local triggerOnce
local moveTime = 0

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end

    name = string.lower(this:GetGeneral().name)

    targetPlayer = _G.gameStateSys:GetEntity("Camera")
    triggerOnce = false
end

function Update()
 if not triggerOnce then
    -- keep spawning arrow pointing down
    -- "this" is the script entity, can get position from here, the x,y,z
    RandomSpawnArrowIndicator()
 end
end

function Dead()
    
end

function OnTriggerEnter(Entity)
    if triggerOnce then return end
    if Entity == targetPlayer then --player id
        print("Epic platform OnTriggerEnter")
        triggerOnce = true

        if (string.find(name, "melissa")) then
            _G.activateEpicM = true
        elseif (string.find(name, "soldier")) then 
            _G.activateEpicTS = true
        elseif (string.find(name, "horse")) then 
            _G.activateEpicTrojanHorse = true
        elseif (string.find(name, "ily")) then 
            _G.activateEpicILY = true
        elseif (string.find(name, "zipbomb")) then 
            _G.activateEpicZB = true
        end
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


function RandomSpawnArrowIndicator()
    moveTime = moveTime + FPSManager.GetDT()

    if(moveTime > 0.4) then
        transform = this:GetTransform()
        local spawndataPos = Vec3.new()
        spawndataPos.x = transform.mTranslate.x + math.random(-300,300)/100
        spawndataPos.y = transform.mTranslate.y + 100
        spawndataPos.z = transform.mTranslate.z + math.random(-300,300)/100

        systemManager.ecs:NewEntityFromPrefab("TriggerIndicator", spawndataPos)
        moveTime = 0.0
    end
end