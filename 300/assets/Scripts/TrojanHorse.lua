local vec = Vec3.new()
local firstvec = Vec3.new()
local secondvec = Vec3.new()
local originalScaleX

local deathEntity
local deathComp
local deathAudioSource

-- Variables for state
local s1Timer           = 2
local s1RoamVelocity    = Vec3.new()
local roamSpeed         = 20

local s2Timer           = 0

local s3SprintVelocity = Vec3.new()
local sprintSpeed       = 50

local s4Timer           = 0

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
    gameStateSys = systemManager:mGameStateSystem()

    deathEntity = gameStateSys:GetEntity("Death" , "testSerialization")
    deathComp   = deathEntity:GetAudio()
    deathAudioSource = Helper.CreateAudioSource(deathEntity)

    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in Trojan Horse script!")
    end

    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();

    -- Initialise the state's variables
    state = "ROAM"
end

function Update()

    if state == "ROAM" then         -- roam around and passively look for player (change to 2. when sees player)
        -- Roam around randomly
        s1Timer = s1Timer + FPSManager.GetDT()
        if s1Timer > 2 then
            s1Timer = 0
            s1RoamVelocity = RandDirectionXZ()
            s1RoamVelocity = Helper.Normalize(s1RoamVelocity)
            s1RoamVelocity = Helper.Scale(s1RoamVelocity, roamSpeed)
        end
        phySys:SetVelocity(entity, s1RoamVelocity)

        -- Look for player here
        if aiSys:ConeOfSight(entity, entity:GetAISetting().GetTarget(), 70, 40) then
            state = "CHARGE"
            CHARGEInit()
        end

    elseif state == "CHARGE" then   -- saw player, eyes glow red, play some charge up noise, delay about 3 seconds before charging to player (change to 3. when delay ends)
        -- Play animation for eyes glowing red        

        -- Constantly make him stare at player
        local stareDirection = Vec3.new()
        stareDirection = Helper.Vec3Minus(entity:GetAISetting().GetTarget():GetTransform().mTranslate, entity:GetTransform().mTranslate)
        entity:GetTransform().mRotate.y = Helper.DirectionToAngle(stareDirection)

        -- Count down 3 seconds
        s2Timer = s2Timer + FPSManager.GetDT()
        if s2Timer > 3 then
            s2Timer = 0
            state = "SPRINT"
            SPRINTInit()
            s3SprintVelocity = Helper.Scale(Helper.Normalize(stareDirection), sprintSpeed)
            -- TODO End audio and animation
        end

    elseif state == "SPRINT" then   -- charge toward last seen player position at high speed (change to 4. when collided with something)
        -- Charge towards last seen player position at high speed
        phySys:SetVelocity(entity, s3SprintVelocity);

        -- Stop and change state when collided with something
        entity:GetRigidBody().mVelocity

    elseif state == "REST" then     -- stops for around 0.5 seconds before moving back to 1. (change to 1. when rest timer ends)
        s4Timer = s4Timer + FPSManager.GetDT()
        if s4Timer > 0.5 then
            s4Timer = 0
            state = "ROAM"
            ROAMInit()
        end

    end



    -- vec = aiSys:GetDirection(entity)
    -- entity:GetTransform().mRotate.y = Helper.DirectionToAngle(vec)

    -- vec.x = vec.x * 20;
    -- vec.y = entity:GetRigidBody().mVelocity.y;
    -- vec.z = vec.z * 20;
    -- phySys:SetVelocity(entity, vec);
    
    -- if (entity:GetTransform().mScale.x < 2.0) then
    --     deathAudioSource:Play()
    --     deathAudioSource:SetVolume(0.2)
    --     systemManager.ecs:SetDeleteEntity(entity)
    -- end
        
    -- AI functions
    -- aiSys:SetPredictiveVelocity(entity, entity, 0.5)
    -- aiSys:PredictiveShootPlayer(entity, 0.5, 2, 4)

    --print("Jiayou Jazzi")
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


-- State initialise functions

function ROAMInit()
    s1Timer = 2
    s1RoamVelocity = RandDirectionXZ()
end

function CHARGEInit()
    s2Timer = 0
end

function SPRINTInit()

end

function RESTInit()

end


-- Helper functions

function RandDirectionXZ()
    local v = Vec3.new()
    v.x = math.random()*2 -1
    v.y = 0
    v.z = math.random()*2 -1
    return v
end

