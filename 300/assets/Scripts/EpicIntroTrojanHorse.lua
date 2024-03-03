-- This script is same as trojan horse but specially for epic intro only
-- Variables for state
local s1Timer           = 2
local s1RoamVelocity    = Vec3.new()
local roamSpeed         = 2
local damage            = 20

local s2ChargeCount     = 0
local s2Charge          = 1.5

local s3SprintVelocity  = Vec3.new()
local sprintSpeed       = 15
local stareDirection    = Vec3.new()

local dashStopTimerCount = 0

local s4RestTimer       = 2
local s4RestTimerCount  = 0

-- Systems
local aiSys
local phySys
local gameStateSys

-- Other variables
local this
local target

local deathTimer = 1.25
local deathTimerCount

local spawnSoldierFrequency = 0.6
local spawnSoldierCounter = 0

local state
-- Trojan horse states
-- 1. ROAM. roam around and passively look for player (change to 2. when sees player)
-- 2. CHARGE. saw player, eyes glow red, play some charge up noise, delay about 3 seconds before charging to player (change to 3. when delay ends)
-- 3. SPRINT. charge toward last seen player position at high speed (change to 4. when collided with something)
-- 4. REST. stops for around 0.5 seconds before moving back to 1. (change to 1. when rest timer ends)

function Alive()
    math.randomseed(os.time())

    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in Trojan Horse script!")
    end
    
    phySys = systemManager:mPhysicsSystem();
    gameStateSys = systemManager:mGameStateSystem();

    -- Initialise the state's variables
    state = "ROAM"
    -- this:GetAudio():SetPlay()
    s1Timer           = 2
    s1RoamVelocity    = Vec3.new()
    s2Timer           = 0
    s3SprintVelocity  = Vec3.new()

    deathTimerCount   = 0
    target = this:GetAISetting():GetTarget()

    CHARGEInit()
end

local timer = 0
function Update()
    if state == "CHARGE" then   -- saw player, eyes glow red, play some charge up noise, delay about 3 seconds before charging to player (change to 3. when delay ends)
        -- Play animation for eyes glowing red        
        
        -- make trojan horse dash towards z = 1
        stareDirection = Vec3.new()
        stareDirection.z = 1
        Helper.SetRealRotate(this, Vec3.new(0, Helper.DirectionToAngle(this, stareDirection), 0))
        phySys:SetVelocity(this, Vec3.new())
        
        -- if audio_played == false then 
        --     this:GetAudio():SetPlay()
        -- end

        -- Count down 3 seconds
        s2ChargeCount = s2ChargeCount + FPSManager.GetDT()
        if s2ChargeCount > s2Charge then
            s2ChargeCount = 0
            SPRINTInit()

            s3SprintVelocity = Helper.Scale(Helper.Normalize(stareDirection), sprintSpeed)
            s3SprintVelocity.y = -2
            -- TODO End audio and animation
        end

    elseif state == "SPRINT" then   -- charge toward last seen player position at high speed (change to 4. when collided with something)
        -- Charge towards last seen player position at high speed
        phySys:SetVelocity(this, s3SprintVelocity);
        Helper.SetRealRotate(this, Vec3.new(0, Helper.DirectionToAngle(this, stareDirection), 0))
        -- Stop and change state when collided with something
        -- This part is done in OnContactEnter
        dashStopTimerCount = dashStopTimerCount + FPSManager.GetDT()
        if dashStopTimerCount > _G.THdashStopTimer then
            dashStopTimerCount = 0
            
            EndEpicIntro()
        end

    elseif state == "REST" then     -- stops for some time before moving back to 1. (change to 1. when rest timer ends)
        Helper.SetRealRotate(this, Vec3.new(0, Helper.DirectionToAngle(this, stareDirection), 0))
        s4RestTimerCount = s4RestTimerCount + FPSManager.GetDT()
        if s4RestTimerCount > s4RestTimer then
            s4RestTimerCount = 0
            EndEpicIntro()
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
            if Entity == target then CrashIntoPlayer() end

            state = "REST"
            RESTInit()
            return
        end
    elseif state == "ROAM" then
        MoveRandDir()
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
    this:GetAudio():SetResume()
    this:GetMeshRenderer():SetMesh("Trojan_Movement", this)
end

function CHARGEInit()
    -- play animation
    this:GetMeshRenderer():SetMesh("Trojan_charging", this)

    --print("Start Charge")
    -- this:GetAudio():SetPause()

    -- [1/31] ML - changed to 3D system (for spatial cues) -> audio file is now attached w the enemy.
    this:GetAudio():SetPlay()
    -- gameStateSys:GetEntity("TrojanHorseChargeAudio"):GetAudio():SetPlay()
    state = "CHARGE"
    s2ChargeCount = 0
end

function SPRINTInit()
    --print("Start Sprint")
    state = "SPRINT"
    this:GetAudio():SetResume()
end

function RESTInit()
    --print("Start Rest")
    state = "REST"
    this:GetMeshRenderer():SetMesh("Trojan_Idle", this) -- change animation back

    -- this:GetAudio():SetPause()
    phySys:SetVelocity(this, Vec3.new())
end

function EndEpicIntro()
    this:GetScripts():AddScript(this, "../assets/Scripts/TrojanHorse.lua")
    this:GetScripts():DeleteScript("..\\assets\\Scripts\\EpicIntroTrojanHorse.lua");
end


-- Helper functions

function RandDirectionXZ()
    local v = Vec3.new()
    v.x = math.random()*2 -1
    v.y = 0
    v.z = math.random()*2 -1
    return v
end

function MoveRandDir()
    s1RoamVelocity = RandDirectionXZ()
    s1RoamVelocity = Helper.Normalize(s1RoamVelocity)
        Helper.SetRealRotate(this, Vec3.new(0, Helper.DirectionToAngle(this, s1RoamVelocity), 0))
    s1RoamVelocity = Helper.Scale(s1RoamVelocity, roamSpeed)
end

-- this function is ran when health just reached 0
function StartDeath()
    -- Start death animation
    -- Start death sound
    state = "DEATH"
    this:GetMeshRenderer():SetMesh("Trojan_Death", this)
    gameStateSys:GetEntity("EnemyDeath"):GetAudio():SetPlay()
end

function Die()
    systemManager.ecs:SetDeleteEntity(this)
end

function SpawnSoldier()
    local position = Helper.Vec3Add(this:GetTransform().mTranslate, this:GetBoxCollider().mTranslateOffset)
    local velocity = Helper.Scale(RandDirectionXZ(), 15)
    velocity.y = 24
    
    local soldier = systemManager.ecs:NewEntityFromPrefab("TrojanSoldier", position)
    phySys:SetVelocity(soldier, velocity)
end

function CrashIntoPlayer()
    -- decrease player health
    target:GetHealthbar().health = target:GetHealthbar().health - damage

    -- make player fly back
    local velocity = Helper.Scale(Helper.Normalize(this:GetRigidBody().mVelocity), 100)
    velocity.y = 20
    phySys:SetVelocity(target, velocity)
end
