local vec = Vec3.new()

local deathAudioSource
local aiSys
local phySys
local this

local bobbleAngle

-- for example you want to reference out hp variable to another script
--local hp = 100

-- function GetHP()
--     return hp
-- end

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();
end

function Update()
    vec = aiSys:GetDirection(this)
    this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, vec)

    Helper.Scale(vec, 20)
    phySys:SetVelocity(this, vec);
    
    if (this:GetTransform().mScale.x < 2.0) then
        deathAudioSource:Play()
        deathAudioSource:SetVolume(0.2)
        systemManager.ecs:SetDeleteEntity(this)
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end
