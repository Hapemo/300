-- Variables for state

-- Systems
local aiSys
local phySys

-- Other variables
local this

local deathTimer = 0.1
local deathTimerCount

local state

local aiSetting

local rotate = Vec3.new()

-- Trojan horse states
-- 1. ROAM. roam around and passively look for player (change to 2. when sees player)
-- 2. CHARGE. saw player, eyes glow red, play some charge up noise, delay about 3 seconds before charging to player (change to 3. when delay ends)
-- 3. SPRINT. charge toward last seen player position at high speed (change to 4. when collided with something)
-- 4. REST. stops for around 0.5 seconds before moving back to 1. (change to 1. when rest timer ends)

function Alive()
    math.randomseed(os.time())

    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end

    phySys = systemManager:mPhysicsSystem();
    aiSetting = this:GetAISetting()
    -- Initialise the state's variables
    state = ""

    deathTimerCount   = 0
end

function Update()

    if systemManager:mInputActionSystem():GetButtonDown("Test5") then
        this:GetHealthbar().health = this:GetHealthbar().health - 10
    end

    if state == "DEATH" then
        deathTimerCount = deathTimerCount + FPSManager.GetDT()
        if deathTimerCount > deathTimer then systemManager.ecs:SetDeleteEntity(this) end
        return
    end

    -- Attack portion is in oncontact
    -- Movement
    this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, this:GetRigidBody().mVelocity)
    
    if Helper.Vec3Len(this:GetRigidBody().mVelocity) < 0.1 then
        print(this:GetRigidBody().mVelocity)
        local upVec = Vec3.new()
        upVec.y = 8
        phySys:SetVelocity(this, upVec);
    end

    if this:GetHealthbar().health <= 0 then StartDeath() end


end

function Dead()

end

function OnTriggerEnter(Entity)
end

function OnTriggerExit(Entity)
end

function OnContactEnter(Entity)
    if state == "DEATH" then return end
    
    local tagID = Entity:GetGeneral().tagid
    local velocity = Helper.Normalize(Helper.Vec3Minus(aiSetting:GetTarget():GetTransform().mTranslate, this:GetTransform().mTranslate))
    velocity = Helper.Scale(velocity, 3)
    velocity.y = 8
    if tagID == 3 then -- "FLOOR"
        -- Movement
    elseif Entity == aiSetting:GetTarget() then
        -- Attack
        velocity.x = -velocity.x * 5
        velocity.y = velocity.y * 3
        velocity.z = -velocity.z * 5
        -- Make player take damage here
        print("player damaged by trojan soldier")
    end
    phySys:SetVelocity(this, velocity);
end

function OnContactExit(Entity)

end


-- this function is ran when health just reached 0
function StartDeath()
    -- Start death animation
    -- Start death sound
    state = "DEATH"
end