
local phySys
local this

local startingVec

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end
    phySys = systemManager:mPhysicsSystem();

    startingVec = Vec3.new()
end

function Update()
    if (Helper.Vec3Len(startingVec) == 0) then
        -- print("len is 0")
        startingVec = this:GetRigidBody().mVelocity
    else
        -- print("len is NOT 0")
        phySys:SetVelocity(this, startingVec)
    end
    -- print(Helper.Vec3Len(startingVec))
end

function Dead()

end

function OnTriggerEnter(Entity)
    local tagid = Entity:GetGeneral().tagid
    if (tagid == 0) then --player id
        -- TODO, Decrease player health here, when player got hit by bomb
        print("player hit by enemy bullet!")
    end
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end

function BulletDespawn()

    -- Might want bullet to explode some particles out
    -- and bullet hit sound

    systemManager.ecs:SetDeleteEntity(this)
end