-- local portalObject
local portalLifeTime = 0
local portalDeathTime = 2.5

local this

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
end

function Update()
    portalLifeTime = portalLifeTime + FPSManager.GetDT()

    if(portalLifeTime > portalDeathTime) then 

        if this ~= nil then
            systemManager.ecs:SetDeleteEntity(this)
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


