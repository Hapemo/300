-- Variables for state
local speed             = 3
local damage            = 15

local s2AttackingTimer          = 1.875 -- This is the attack animation length
local s2AttackingTimerCount     = 0

local s3ScreamingTimer           = 0
local s3ScreamingTimerLimit      = 2.7 -- This should depend on the animation time period

-- Systems
local aiSys
local phySys
local gameStateSys

-- Other variables
local this
local target
local thisPos
local targetPos
local direction    = Vec3.new()
local spawnMelissa = false      -- This should be rng, might or might not spawn another melissa (50% chance)
local notBelow50Yet
local AttackOnce = false

local deathTimer = 1.45
local deathTimerCount = 0

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
    gameStateSys = systemManager:mGameStateSystem();

    -- Initialise the state's variables
    state = "TRAVEL"
    this:GetMeshRenderer():SetMesh("Melissa_Moving", this)

    local num = math.random()
    spawnMelissa = num > 0.5
    target = this:GetAISetting():GetTarget()

    thisPos = this:GetTransform().mTranslate
    targetPos = target:GetTransform().mTranslate

    notBelow50Yet = true
    deathTimerCount = 0

    -- Change pathfinding graph to match that of the gamestate
    if (gameStateSys:GetCurrentGameState().mName == "Test") then
        this:GetAISetting().mGraphDataName = "GroundPath"
    elseif (gameStateSys:GetCurrentGameState().mName == "Test2") then
        this:GetAISetting().mGraphDataName = "GroundPath2"
    elseif (gameStateSys:GetCurrentGameState().mName == "Test3") then
        this:GetAISetting().mGraphDataName = "GroundPath3"
    end
end

function Update()

    -- if systemManager:mInputActionSystem():GetButtonDown("Test1") then
    --     this:GetHealthbar().health = this:GetHealthbar().health - 10
    -- end

    Helper.SetRealRotate(this, Vec3.new(0, Helper.DirectionToAngle(this, direction), 0))
    -- STATE MACHINE
    if state == "TRAVEL" then         -- walk directly to player using pathfinding (change to 2. when duplicate timer runs out)
        direction = aiSys:GetDirection(this)
        direction.y = direction.y + 0.5
        Helper.Scale(direction, speed)
    
        phySys:SetVelocity(this, direction);

        if (InMeleeRange(1.5)) then
            AttackInit()
        end

        if notBelow50Yet then
            -- Check for health below 50%
            if (this:GetHealthbar().health/this:GetHealthbar().maxHealth < 0.5) then 
                notBelow50Yet = false
                SpawnMelissa()
            end
        end
    elseif state == "ATTACKING" then -- damaging when at half the duration
        phySys:SetVelocity(this, Vec3.new());
        s2AttackingTimerCount = s2AttackingTimerCount + FPSManager.GetDT()
        if (s2AttackingTimerCount > s2AttackingTimer) then
            s2AttackingTimerCount = 0
            AttackOnce = false
            TRAVELInit()
        elseif (s2AttackingTimerCount > s2AttackingTimer/2) and not AttackOnce then -- (this run once only per attack state entrance)
            AttackOnce = true
            -- Play attack SOUND
            -- decrease player health
            if InMeleeRange(2) then
                target:GetHealthbar().health = target:GetHealthbar().health - damage
            end
        end
    elseif state == "DUPLICATING" then   -- stops moving and vibrate hard. when vibrate timer is up, spawn in another melissa (change to 1. after duplicating)
        phySys:SetVelocity(this, Vec3.new());
        
        -- Set vibrating animation
        -- Start melissa's screaming
        s3ScreamingTimer = s3ScreamingTimer + FPSManager.GetDT()
        if (s3ScreamingTimer > s3ScreamingTimerLimit) then
            s3ScreamingTimer = 0
            -- Spawn in another melissa
            local spawnPos = this:GetTransform().mTranslate
            local scale = this:GetTransform().mScale
            spawnPos.x = spawnPos.x + scale.x
            spawnPos.z = spawnPos.z + scale.z
            systemManager.ecs:NewEntityFromPrefab("Melissa", spawnPos)
            TRAVELInit()
        end
    elseif state == "DEATH" then
        deathTimerCount = deathTimerCount + FPSManager.GetDT()
        if deathTimerCount > deathTimer then systemManager.ecs:SetDeleteEntity(this) end
        return
    end
    -- END STATE MACHINE

    -- Health logic
    if this:GetHealthbar().health <= 0 then
        StartDeath() 
        if _G.Level3_Monsters ~= nil then 
            if _G.Level3_Monsters == true then 
                _G.number_left_in_level_3 = _G.number_left_in_level_3 - 1
            end
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
function AttackInit()
    -- Start attack animation
    --print("Start melissa attack")
    this:GetMeshRenderer():SetMesh("Melissa_attack", this, 1)
    AttackOnce = false
    s2AttackingTimerCount = 0
    state = "ATTACKING"
end

function TRAVELInit()
    state = "TRAVEL"

    --print("Start melissa travel")
    this:GetMeshRenderer():SetMesh("Melissa_Moving", this) -- Change back to moving animation 

end

function SpawnMelissa() -- This function should be called in 
    if not spawnMelissa then return end
    state = "DUPLICATING"
    this:GetMeshRenderer():SetMesh("Melissa_Split", this) -- Change to death animation 
    s3ScreamingTimer = 0
    this:GetAudio():SetPlay()
end

-- this function is ran when health just reached 0
function StartDeath()
    -- Start death animation
    -- Start death sound
    state = "DEATH"
    gameStateSys:GetEntity("EnemyDeath"):GetAudio():SetPlay()
    this:GetMeshRenderer():SetMesh("Melissa_Death", this) -- Change to death animation 
    -- this:GetAudio():SetStop()
end

-- Helper functions

function InMeleeRange(range)
    local distance = Helper.Vec3Len(Helper.Vec3Minus(targetPos, thisPos))
    if distance < range then return true end
    return false
end





