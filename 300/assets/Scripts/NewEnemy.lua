
local name
local this
local target

local aiSys
local scriptingSys

local color = Vec4.new(0,0,0,1)
local flipColor = true
local colorRate = 1

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
        -- _G.PreObjectivesCounter = _G.PreObjectivesCounter + 1;
        --transformed away fr now lol
        systemManager.ecs:SetDeleteEntity(this)
    end

    -- If can see player, activate the scripts accordingly
    if not runOnce and (aiSys:ConeOfSight(this, target, 70, 40)) then
        runOnce = true
        -- print(name)
        -- print("sees player")
        ActivateScript(this)
    end

    if (flipColor) then
        if (color.x < 1) then
            color.x = color.x + FPSManager.GetDT()*colorRate
            if color.x > 1 then color.x = 1 end
        elseif (color.y < 1) then
            color.y = color.y + FPSManager.GetDT()*colorRate
            if color.y > 1 then color.x = 1 end
        elseif (color.z < 1) then
            color.z = color.z + FPSManager.GetDT()*colorRate
            if color.z > 1 then 
                color.z = 1 
                flipColor = false
            end
        end
    else 
        if (color.x > 0) then
            color.x = color.x - FPSManager.GetDT()*colorRate
            if color.x < 0 then color.x = 0 end
        elseif (color.y > 0) then
            color.y = color.y - FPSManager.GetDT()*colorRate
            if color.y < 0 then color.y = 0 end
        elseif (color.z > 0) then
            color.z = color.z - FPSManager.GetDT()*colorRate
            if color.z < 0 then 
                color.z = 0 
                flipColor = true
            end
        end
    end
    this:GetMeshRenderer():SetColor(color)
    

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
