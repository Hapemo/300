
local phySys
local this

local startingVec
local lifeTime -- in seconds

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end
    phySys = systemManager:mPhysicsSystem();

    startingVec = Vec3.new()
    lifeTime = 5
end

function Update()
    -- Set bullet's velocity to consistently be some speed
    if (Helper.Vec3Len(startingVec) == 0) then
        startingVec = this:GetRigidBody().mVelocity
    else
        phySys:SetVelocity(this, startingVec)
    end

    -- Bullet despawn countdown
    lifeTime = lifeTime - FPSManager.GetDT()
    if lifeTime < 0 then 
        systemManager.ecs:SetDeleteEntity(this)
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    local tagid = Entity:GetGeneral().tagid
    if (tagid == 0 and tagid == 3 and tagid == 4 and tagid == 5) then --Things the bullet can hit - "PLAYER", "FLOOR", "WALL", "TELEPORTER"
        BulletHit()
        if (tagid == 0) then 
            -- TODO, Decrease player health here, when player got hit by bomb
            print("player hit by enemy bullet!")
        end
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

function BulletHit()

    -- Might want bullet to explode some particles out
    -- and bullet hit sound

    systemManager.ecs:SetDeleteEntity(this)
end