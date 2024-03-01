local this

-- Ground slam variables
local groundSlamDirection
local groundSlamPosition = Vec3.new()
local currentGroundSlamResetTimer = 0
local maxGroundSlamResetTimer = 3
local groundSlamCount = 0  -- Changes to other attacking state when it slams a designated amount of time 

-- Tentative Random Boss State CHanger
local currentBossStateTimer = 0
local maxBossStateTimer = 3

-- Phase 3 : Bullethell Phase
local bulletSpawnPosition = Vec3.new()

-- Boss states
local state = 0
local state_checker = {false, false ,false}
local once = false
-- 1. Summon enemies
-- 2. Ground slam: Boss swings arms and slams the ground, spawning a ground slam area that damages player
-- 3. Shoot projectiles

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    -- Testing (Bullet Spawn Position)
    bulletSpawnPosition = this:GetTransform().mTranslate
    physicsSys = systemManager:mPhysicsSystem()
end

function Update()

    print("CURRENT STATE: " , state)

    -- print("PHASE 1 BOOL: " , state_checker[1])
    
    -- print("PHASE 2 BOOL: " , state_checker[2])
    
    -- print("PHASE 3 BOOL: " , state_checker[3])
    -- for i = 0 , 2 do
    --     print("PHASE BOOL: " , state_checker[i][1])
    -- end

    -- Tentative random switcher between boss states, replace with HP after other states implemented. 100% HP Left = Phase 1, 66% HP Left = Phase 2, 33% HP Left = Phase 3
    currentBossStateTimer = currentBossStateTimer + FPSManager.GetDT()
    if currentBossStateTimer >= maxBossStateTimer then
        state = math.random(1, 3)
        currentBossStateTimer = 0
    end

    -- state = 2 -- For testing only to force state 2, delete afterwards
    state = 3

    if state == 1 and state_checker[1] == false then
    end

    if state == 2 and state_checker[2] == false then

        -- Timer to set intervals between ground slams
        currentGroundSlamResetTimer = currentGroundSlamResetTimer + FPSManager.GetDT()

        if currentGroundSlamResetTimer >= maxGroundSlamResetTimer then

            -- Pick which direction to ground slam in 
            groundSlamDirection = math.random(1, 3)

            -- Ground slam right (from boss perspective)
            if groundSlamDirection == 1 then
                groundSlamPosition.x = -15
                groundSlamPosition.y = -6
                groundSlamPosition.z = 25
            end

            -- Ground slam front (from boss perspective)
            if groundSlamDirection == 2 then
                groundSlamPosition.x = 0
                groundSlamPosition.y = -6
                groundSlamPosition.z = 25
            end

            -- Ground slam left (from boss perspective)
            if groundSlamDirection == 3 then
                groundSlamPosition.x = 15
                groundSlamPosition.y = -6
                groundSlamPosition.z = 25
            end

            -- TODO: Play arm swinging animation before spawning ground slam object
            roundSlam = systemManager.ecs:NewEntityFromPrefab("GroundSlamObject", groundSlamPosition)
            currentGroundSlamResetTimer = 0 -- Reset ground slam timer
        else 
            state_checker[2] = true
        end

    end

    if state == 3 and state_checker[3] == false then
        -- Bullet Types (Phases)
        -- 1. Bullet Hell (no homing) -> maybe add 2 variations to start
        -- 2. Homing Bullets (but dodgeable)

        -- if once == false then 
        --  sphere_bullet = systemManager.ecs:NewEntityFromPrefab("Boss_Bullet", bulletSpawnPosition)
        --  once = true
        -- end

        SpawnBulletsPattern1(1)

        -- original_scale =  sphere_bullet :GetTransform().mScale 
        -- sphere_bullet:GetTransform().mScale.x = original_scale.x / 3
        -- sphere_bullet:GetTransform().mScale.y = original_scale.y / 3
        -- sphere_bullet:GetTransform().mScale.z = original_scale.z / 3
        print("SPAWNING BULLETS")
    end

end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end

function randomnizeState() 
   local random_state = math.random(1, 3)


end 

-- Function to rotate a vector around the Y-axis
-- angle is in degrees
function RotateVectorAroundYAxis(vector, angle)
    local angleInRadians = math.rad(angle)
    local rotatedX = vector.x * math.cos(angleInRadians) - vector.z * math.sin(angleInRadians)
    local rotatedZ = vector.x * math.sin(angleInRadians) + vector.z * math.cos(angleInRadians)
    -- return { x = rotatedX, y = vector.y, z = rotatedZ }

    vector.x = rotatedX 
    vector.z = rotatedZ
end


function SpawnBulletsPattern1(number_of_bullets)
    local angleStep = 360 / number_of_bullets
    local angle = 0

    local forward = Vec3.new(0,0,1)

    print("NUMBER OF BULLETS TO SPAWN")

    for i = 0 , number_of_bullets do 

        bulletDirection = Vec3.new()
        -- Caluclate direction vector in 3D space
        bulletDirection = RotateVectorAroundYAxis(forward, angle) -- Testing
        print("X: " , bulletDirection.x , " Y: " , bulletDirection.y , " Z: ", bulletDirection.z)
        sphere_bullet = systemManager.ecs:NewEntityFromPrefab("Boss_Bullet", bulletSpawnPosition)
        
        
        -- physicsSys:SetVelocity(sphere_bullet, bulletDirection)
        
    end

end