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
local spawnMelissa      -- This should be rng, might or might not spawn another melissa (50% chance)
local healthBelow50
local notBelow50Yet

local state
-- Melissa states
-- 1. TRAVEL. walk directly to player using pathfinding (change to 2. when health falls below 50% health and is intended to spawn another melissa)
-- 2. DUPLICATE. stops moving and vibrate hard. when vibrate timer is up, spawn in another melissa (change to 1. after duplicating)

-- for example you want to reference out hp variable to another script
--local hp = 100

-- function GetHP()
--     return hp
-- end

function Alive()
    math.randomseed(os.time())

    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in Melissa script!") end

    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();

    -- Initialise the state's variables
    state = "TRAVEL"

    spawnMelissa = math.random() > 0.5

    healthBelow50 = false
end

function Update()
    -- OTHER UPDATE CODES
    -- Movement

    -- STATE MACHINE
    if state == "TRAVEL" then         -- walk directly to player using pathfinding (change to 2. when duplicate timer runs out)
        direction = aiSys:GetDirection(this)
        this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, direction)
    
        Helper.Scale(direction, speed)
    
        phySys:SetVelocity(this, direction);

        if notBelow50Yet then
            if (this:GetHealthbar().health/this:GetHealthbar().maxHealth > 0.5) then 
                -- do something
            end
            -- Check for health below 50
            -- Need michelle's other function to check for health in other script. 
            -- If health below 50, set notBelow50Yet to false 
            -- Run the SpawnMelissa() function
        end
    elseif state == "DUPLICATING" then   -- stops moving and vibrate hard. when vibrate timer is up, spawn in another melissa (change to 1. after duplicating)
        phySys:SetVelocity(this, Vec3.new());
        
        -- Set vibrating animation
        -- Start melissa's screaming
        s2Timer = s2Timer + FPSManager.GetDT()
        if (s2Timer > s2TimerLimit) then
            s2Timer = 0
            -- Spawn in another melissa
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

function TRAVELInit()
    state = "TRAVEL"
end

function SpawnMelissa() -- This function should be called in 
    if not spawnMelissa then return end
    state = "DUPLICATE"
    s2Timer = 0
end



-- Helper functions
