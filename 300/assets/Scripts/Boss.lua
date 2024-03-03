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
local bossPosition = Vec3.new()
local bulletSpawnPosition = Vec3.new()
local bulletProjectileType = 0                          -- Used to determine which kind of projectile the enemy uses
local fire_rate = 0.5
local fire_timer = 0
local next_fire_time = 1.0

-- Bullet Hell #1 - Circular Pulse
local angles_to_spawn_from = {0, 45 , 90 , 135 , 180}
local no_of_waves_1 = 5                                 -- used to keep track how many waves of bullets to spawn
local random_wave_generator = false                     -- boolean value to keep track if a random value has been generated.
-- keeps track of which bullethell attack has been used (so there won't be back to back usage)

-- [1] Spiral Bullets (Ground)
local bullet_attack_checker = {false, false, false}     -- keep track which attacks have been used so won't activate back to back
local attacking = false
local number_of_fire = 0
local stop_firing_at = 10

-- [2] Summon Bullet + Homing Bullets (From Boss' Mouth / Front) 
local spawn_point_ref_obj
local spawn_point_ref_trans
local spawn_point_translate = Vec3.new()



-- Boss states
local state = 0
local state_checker = {false, false , false}
local once = false
-- 1. Summon enemies
-- 2. Ground slam: Boss swings arms and slams the ground, spawning a ground slam area that damages player
-- 3. Shoot projectiles


function Alive()
    print("ALIVE")
    this = Helper.GetScriptEntity(script_entity.id)
    gameStateSys = systemManager:mGameStateSystem();
    physicsSys = systemManager:mPhysicsSystem()

    -- Testing (Bullet Spawn Position)
    bossPosition = this:GetTransform().mTranslate
    spawn_point_ref_obj = gameStateSys:GetEntityByScene("Spawn_Point_Ref" , "BossStuff")
    spawn_point_ref_trans = spawn_point_ref_obj:GetTransform().mTranslate

    -- For [1] Spiral Bullets (Ground)
    bulletSpawnPosition.x = bossPosition.x
    bulletSpawnPosition.y = bossPosition.y
    bulletSpawnPosition.z = bossPosition.z

    -- For [2]  Summon Bullet + Homing Bullets (From Boss' Mouth / Front) 
    spawn_point_translate.x = spawn_point_ref_trans.x
    spawn_point_translate.y = spawn_point_ref_trans.y
    spawn_point_translate.z = spawn_point_ref_trans.z

    -- print("1")
    print("1: " , spawn_point_translate.x)
    print("2: " , spawn_point_translate.y)
    print("3: " , spawn_point_translate.z)

end

function Update()

    -- print("CURRENT STATE: " , state)

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

    -- Bullet Types (Phases)
    -- 1. Bullet Hell (no homing) -> maybe add 2 variations to start
    -- 2. Homing Bullets (but dodgeable)

    
    -- [Bullet Hell #1 : Pulsing Circles]
    -- (a) State 1 : Normal Circles but pulsing in different directions & angles. 
    --     - Make it easier at the start 
    if state == 3 and state_checker[3] == false then
        
        fire_timer = fire_timer +  FPSManager.GetDT()

        if attacking == false then 
            -- bulletProjectileType = math.random (1, 3)
            bulletProjectileType = 2
            print("PROJECTILE ATTACKING TYPE: "  , bulletProjectileType)
            attacking = true
        end

        



        -- Check if this projectile has been recently used... 
        if(bullet_attack_checker[bulletProjectileType] == false) then 
            if(bulletProjectileType == 1) then 
                if(random_wave_generator == false) then 
                    random_wave_generator = true 
                    no_of_waves_1 = math.random(5, 8) -- can generate between 5 to 10 waves of bullets
                end

                if fire_timer > next_fire_time then 
                    if number_of_fire < stop_firing_at then 
                        fire_timer = 0 -- reset timer
                        print("NO OF WAVES: " , no_of_waves_1)
                        SpawnBulletsPattern1(no_of_waves_1)
                        number_of_fire = number_of_fire + 1
                    else
                        -- state_checker[3] = true
                        bullet_attack_checker[1] = true
                        attacking = false
                    end
                end
            end

            if(bulletProjectileType == 2) then 
                HomingSpheres(10)
                bullet_attack_checker[2] = true
                attacking = false
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
    -- print("ROTATED X: " , vector.x)
    -- print("ROTATED Z: " , vector.z)
end

-- Bullet Attack 1 - Spiraling Spheres (Ground)
function SpawnBulletsPattern1(number_of_bullets)

    local angleStep = 360 / number_of_bullets
    -- Add some randomnization
    local random_index = math.random(1,5)
    local angle = angles_to_spawn_from[random_index]
    -- print("ANGLE: " , angle)

    -- local angle = 0

    local forward = Vec3.new(0,0,1)

    -- print("NUMBER OF BULLETS TO SPAWN")

    for i = 0 , number_of_bullets do 

        bulletDirection = Vec3.new(1,0,0)
        bulletSpeed = 5
        -- Caluclate direction vector in 3D space
        RotateVectorAroundYAxis(bulletDirection, angle) -- Testing
        -- print("X: " , bulletDirection.x , " Y: " , bulletDirection.y , " Z: ", bulletDirection.z)
        sphere_bullet = systemManager.ecs:NewEntityFromPrefab("Boss_Bullet", bulletSpawnPosition)
        -- print("Spawning at: " , bulletSpawnPosition.x , ", " , bulletSpawnPosition.y , ", " , bulletSpawnPosition.z)

        -- Applying speed to vector
        bulletDirection.x = bulletDirection.x * bulletSpeed 
        bulletDirection.y = bulletDirection.y * bulletSpeed 
        bulletDirection.z = bulletDirection.z * bulletSpeed 

        physicsSys:SetVelocity(sphere_bullet, bulletDirection)
        
        angle = angle + angleStep
        
    end

end

-- Bullet Attack 2 - Homing Spheres (From eyes to player)
function HomingSpheres(number_of_bullets)   
    -- Let's start by randomnizing the starting spawn point of the projectile

    for i = 0 , number_of_bullets do 
        local attack_offset_range = 5  -- Defines a range to spread out from the central position to summon the bullets in different positions
                    
        local random_offset_x = math.random(-attack_offset_range , attack_offset_range)
        local random_offset_y = math.random(-attack_offset_range , attack_offset_range)
        local random_offset_z = math.random(-attack_offset_range , attack_offset_range)
        
        local bullet_spawn_position = Vec3.new(spawn_point_translate.x + random_offset_x , 
                                            spawn_point_translate.y + random_offset_y , 
                                            spawn_point_translate.z + random_offset_z)

        sphere_bullet = systemManager.ecs:NewEntityFromPrefab("Boss_Bullet_Homing",bullet_spawn_position)
    end

end


