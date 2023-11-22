-- Variables for state
local speed             = 20

local s2Timer           = 0
local s2TimerLimit      = 2 -- This should depend on the animation time period

-- Systems
local aiSys
local phySys

-- Other variables
local this
local direction
local spawnMelissa = false      -- This should be rng, might or might not spawn another melissa (50% chance)
local notBelow50Yet

local deathTimer = 2
local deathTimerCount

local state
-- Melissa states
-- 1. TRAVEL. walk directly to player using pathfinding (change to 2. when health falls below 50% health and is intended to spawn another melissa)
-- 2. DUPLICATE. stops moving and vibrate hard. when vibrate timer is up, spawn in another melissa (change to 1. after duplicating)

function Alive()
    math.randomseed(os.time())

    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in Melissa script!") end

    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();

    -- Initialise the state's variables
    state = "TRAVEL"

    local num = math.random()
    spawnMelissa = num > 0.5
    print(num)
    print(spawnMelissa)

    notBelow50Yet = true
    deathTimerCount = 0
end

function Update()

    if systemManager:mInputActionSystem():GetButtonDown("Test1") then
        this:GetHealthbar().health = this:GetHealthbar().health - 10
    end

    -- Health logic
    if this:GetHealthbar().health <= 0 then StartDeath() end

    -- STATE MACHINE
    if state == "TRAVEL" then         -- walk directly to player using pathfinding (change to 2. when duplicate timer runs out)
        direction = aiSys:GetDirection(this)
        this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, direction)
    
        Helper.Scale(direction, speed)
    
        phySys:SetVelocity(this, direction);

        if notBelow50Yet then
            -- Check for health below 50%
            if (this:GetHealthbar().health/this:GetHealthbar().maxHealth < 0.5) then 
                notBelow50Yet = false
                SpawnMelissa()
            end
        end
    elseif state == "DUPLICATING" then   -- stops moving and vibrate hard. when vibrate timer is up, spawn in another melissa (change to 1. after duplicating)
        phySys:SetVelocity(this, Vec3.new());
        
        -- Set vibrating animation
        -- Start melissa's screaming
        s2Timer = s2Timer + FPSManager.GetDT()
        if (s2Timer > s2TimerLimit) then
            s2Timer = 0
            -- Spawn in another melissa
            local spawnPos = this:GetTransform().mTranslate
            local scale = this:GetTransform().mScale
            spawnPos.x = spawnPos.x + scale.x
            spawnPos.z = spawnPos.z + scale.z
            local melissa2 = systemManager.ecs:NewEntityFromPrefab("Melissa", spawnPos)
            TRAVELInit()
        end
    elseif state == "DEATH" then
        deathTimerCount = deathTimerCount + FPSManager.GetDT()
        if deathTimerCount > deathTimer then systemManager.ecs:SetDeleteEntity(this) end
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

function TRAVELInit()
    state = "TRAVEL"
end

function SpawnMelissa() -- This function should be called in 
    if not spawnMelissa then return end
    state = "DUPLICATING"
    s2Timer = 0
end

-- this function is ran when health just reached 0
function StartDeath()
    -- Start death animation
    -- Start death sound
    state = "DEATH"
end

-- Helper functions
