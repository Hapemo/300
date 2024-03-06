
local phySys
local this
local damage = 10

local startingVec
local lifeTime -- in seconds
local first

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end
    phySys = systemManager:mPhysicsSystem();

    startingVec = Vec3.new()
    lifeTime = 5
    first = false
end

function Update()
    -- Set bullet's velocity to consistently be some speed
    if (Helper.Vec3Len(startingVec) == 0) then
        startingVec = this:GetRigidBody().mVelocity
        --print(this)
        --print(startingVec)
    elseif first == false then
        phySys:SetVelocity(this, startingVec)
        this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, startingVec)
        first = true
    else 
        phySys:SetVelocity(this, this:GetRigidBody().mVelocity)
        this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, this:GetRigidBody().mVelocity)
    end

    -- Bullet despawn countdown
    lifeTime = lifeTime - FPSManager.GetDT()
    if lifeTime < 0 then 
        systemManager.ecs:SetDeleteEntity(this)
    end
    --print(this)
    --print(startingVec)
end

function Dead()

end

function OnTriggerEnter(Entity)
    local tagid = Entity:GetGeneral().tagid
    if (tagid == 0 and tagid == 3 and tagid == 4 and tagid == 5) then --Things the bullet can hit - "PLAYER", "FLOOR", "WALL", "TELEPORTER"
        BulletHit()
        if (tagid == 0) then 
            -- Decrease player health here, when player got hit bullet
            _G.ILYBulletHitPlayer = true
            Entity:GetHealthbar().health = Entity:GetHealthbar().health - damage
        end
    end
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