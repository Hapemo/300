-- Variables for state
local s1Timer           = 2
local s1RoamVelocity    = Vec3.new()
local roamSpeed         = 20

local s2Timer           = 0

local s3SprintVelocity = Vec3.new()
local sprintSpeed       = 50

local s4Timer           = 0

-- Systems
local aiSys
local phySys

-- Other variables
local this


local state
-- Trojan horse states
-- 1. ROAM. roam around and passively look for player (change to 2. when sees player)
-- 2. CHARGE. saw player, eyes glow red, play some charge up noise, delay about 3 seconds before charging to player (change to 3. when delay ends)
-- 3. SPRINT. charge toward last seen player position at high speed (change to 4. when collided with something)
-- 4. REST. stops for around 0.5 seconds before moving back to 1. (change to 1. when rest timer ends)


-- for example you want to reference out hp variable to another script
--local hp = 100

-- function GetHP()
--     return hp
-- end

function Alive()
    math.randomseed(os.time())

    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in Trojan Horse script!")
    end

    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();

    -- Initialise the state's variables
    state = "ROAM"
    s1Timer           = 2
    s1RoamVelocity    = Vec3.new()
    roamSpeed         = 20
    s2Timer           = 0
    s3SprintVelocity  = Vec3.new()
    sprintSpeed       = 50
    s4Timer           = 0
end

function Update()

    -- OTHER UPDATE CODES

    -- STATE MACHINE
    if state == "ROAM" then         -- roam around and passively look for player (change to 2. when sees player)
        -- Roam around randomly
        s1Timer = s1Timer + FPSManager.GetDT()
        if s1Timer > 2 then
            s1Timer = 0
            s1RoamVelocity = RandDirectionXZ()
            s1RoamVelocity = Helper.Normalize(s1RoamVelocity)
            this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, s1RoamVelocity)
            s1RoamVelocity = Helper.Scale(s1RoamVelocity, roamSpeed)
        end
        phySys:SetVelocity(this, s1RoamVelocity)

        -- Look for player here
        if aiSys:ConeOfSight(this, this:GetAISetting():GetTarget(), 70, 40) then
            CHARGEInit()
        end

    elseif state == "CHARGE" then   -- saw player, eyes glow red, play some charge up noise, delay about 3 seconds before charging to player (change to 3. when delay ends)
        -- Play animation for eyes glowing red        

        -- Constantly make him stare at player and stand still
        local stareDirection = Vec3.new()
        stareDirection = Helper.Vec3Minus(this:GetAISetting():GetTarget():GetTransform().mTranslate, this:GetTransform().mTranslate)
        this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, stareDirection)
        phySys:SetVelocity(this, Vec3.new())

        -- Count down 3 seconds
        s2Timer = s2Timer + FPSManager.GetDT()
        if s2Timer > 3 then
            s2Timer = 0
            SPRINTInit()
            s3SprintVelocity = Helper.Scale(Helper.Normalize(stareDirection), sprintSpeed)
            s3SprintVelocity.y = 0
            -- TODO End audio and animation
        end

    elseif state == "SPRINT" then   -- charge toward last seen player position at high speed (change to 4. when collided with something)
        -- Charge towards last seen player position at high speed
        phySys:SetVelocity(this, s3SprintVelocity);

        -- Stop and change state when collided with something
        -- This part is done in OnContactEnter

    elseif state == "REST" then     -- stops for around 0.5 seconds before moving back to 1. (change to 1. when rest timer ends)
        s4Timer = s4Timer + FPSManager.GetDT()
        if s4Timer > 0.5 then
            s4Timer = 0
            ROAMInit()
        end

    end
    -- END STATE MACHINE

end

function Dead()

end

function OnTriggerEnter(Entity)
end

function OnTriggerExit(Entity)
end

function OnContactEnter(Entity)
    if (state == "SPRINT") then
        local generalComponent = Entity:GetGeneral()
        local tagid = generalComponent.tagid
        if (tagid ~= 2 and tagid ~= 3 and tagid ~= 7) then -- "BULLET", "FLOOR", "GRAPH"
            -- When collide with anything other than those tag, stop and change state
            state = "REST"
            RESTInit()
            return
        end
        

    end
end

function OnContactExit(Entity)

end


-- State initialise functions

function ROAMInit()
    --print("Start Roam")
    state = "ROAM"
    s1Timer = 2
    s1RoamVelocity = RandDirectionXZ()
end

function CHARGEInit()
    --print("Start Charge")
    state = "CHARGE"
    s2Timer = 0
end

function SPRINTInit()
    --print("Start Sprint")
    state = "SPRINT"
end

function RESTInit()
    --print("Start Rest")
    state = "REST"
    phySys:SetVelocity(this, Vec3.new())
end


-- Helper functions

function RandDirectionXZ()
    local v = Vec3.new()
    v.x = math.random()*2 -1
    v.y = 0
    v.z = math.random()*2 -1
    return v
end

