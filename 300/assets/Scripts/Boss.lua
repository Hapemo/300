local this

-- Phase 1 : Enemyspawn Phase
local enemyType
local enemySpawnDirection
local enemySpawnPosition = Vec3.new()
local currentEnemySpawnResetTimer = 0
local maxEnemySpawnResetTimer = 3
local total_number_of_enemies_to_spawn = 0              -- Total number of enemies to spawn this attack phase
local summon_per_spawn_instance = 0                     -- Per Spawn Instance (2, 3, 2) -> at varying locations
local number_of_enemies_to_summon_per_round = 3          
_G.number_of_spawned_in_level_3 = 0                     -- Keeps track how many enemies have been spawned
_G.number_left_in_level_3 = 0                           -- Keeps track how many enemies is left
_G.Level3_Monsters = true                               -- Special Boolean -> interaction with spawning mechanic
local phase_1_timer = 0 
local phase_1_max_time = 10                             -- Give Players time to fight the minions

-- Phase 2 : Groundslam Phase
local groundSlamDirection
local groundSlamPosition = Vec3.new()
local currentGroundSlamResetTimer = 0
local maxGroundSlamResetTimer = 3
local groundSlamCount = 0  -- Changes to other attacking state when it slams a designated amount of time 
local groundSlamMax = 0

-- Tentative Random Boss State CHanger
local currentBossStateTimer = 0
local maxBossStateTimer = 3

-- Phase 3 : Bullethell Phase
local spiralBulletSpawnerObj
local spiralBulletSpawnPosition = Vec3.new()
local bulletSpawnPosition = Vec3.new()
local bulletTranslateRef
local bulletProjectileType = 0                          -- Used to determine which kind of projectile the enemy uses
local fire_rate = 0.5
local fire_timer = 0
local next_fire_time = 1.0

-- Bullet Hell #1 - Circular Pulse
local angles_to_spawn_from = {0, 45 , 90 , 135 , 180}
local no_of_waves_1 = 5                                 -- used to keep track how many waves of bullets to spawn
local random_wave_generator = false                     -- boolean value to keep track if a random value has been generated.
-- keeps track of which bullethell attack has been used (so there won't be back to back usage)

-- [3] Spiral Bullets (Ground)
local bullet_attack_checker = {false, false, false}     -- keep track which attacks have been used so won't activate back to back
_G.attacking = false                                    -- boolean to control when to choose state (used in [BossLaserBeamPhase.lua])
local number_of_fire = 0
local stop_firing_at = 10

-- [4] Summon Bullet + Homing Bullets (From Boss' Mouth / Front) 
--     -> Players have to shoot them to break them
local spawn_point_ref_obj
local spawn_point_ref_trans
local spawn_point_translate = Vec3.new()                         
local number_of_homing = 0
-- local number_of_homing_spawned = 0
local homing_spawned = false
local homing_spawn_timer = 0                            -- Timer that increament with DT
local homing_spawn_period = 1.0                         -- Time to spawn (in between each sphere)
local homing_spawn_counter = 0                          -- Counter to keep track how many homing bullets have been spawned
-- local homing_start = false

local homing_bullet
local next_homing_id = 0                                -- Used in [SpawnHomingSpheres] -> for ID recognition (used for deletion logic)
_G.homing_projectiles = {}                              -- Define a table to store projectile data -> used in [BossBullet.lua] also
local projectile_stay_time = 2                          -- timer for the bullet to stay still before it starts homing into the player
local initial_homing_speed = 6                          -- Starting Homing Speed


-- [5] Lazer Attack 
_G.activateLazerScript = false

-- Boss states
local state = 0
-- State Checker List -> checks if this attack has been used (if used, won't repeat again until a certain conditions hits)
-- [1] - Summon Minions -> visual : through a portal  (OK)
-- [2] - Ground Slam (AOE)                            (OK)  -> need to review
-- [3] - Projectile #1 (Pulsing Spheres)              (OK)
-- [4] - Projectile #2 (Homing Eyeballs)              (OK)  -> need to fix homing logic
-- [5] - Lazer Attack (Ground)      

_G.state_checker = {false, false , false , false , false}   -- used in [BossLaserBeamPhase.lua]
local once = false

-- Player Stuff
local player_object 
local player_position = Vec3.new()

-- 1. Summon enemies
-- 2. Ground slam: Boss swings arms and slams the ground, spawning a ground slam area that damages player
-- 3. Shoot projectiles
-- 4. Laser Attack


function Alive()
    print("ALIVE")
    this = Helper.GetScriptEntity(script_entity.id)
    gameStateSys = systemManager:mGameStateSystem();
    physicsSys = systemManager:mPhysicsSystem()

    -- Testing (Bullet Spawn Position)
    spiralBulletSpawnerObj = gameStateSys:GetEntityByScene("Spiral_Bullet_Spawn", "BossStuff")
    spiralBulletSpawnPosition = spiralBulletSpawnerObj:GetTransform().mTranslate
    spawn_point_ref_obj = gameStateSys:GetEntityByScene("Spawn_Point_Ref" , "BossStuff")
    spawn_point_ref_trans = spawn_point_ref_obj:GetTransform().mTranslate

    -- Player 
    player_object = gameStateSys:GetEntityByScene("Camera" , "test3")
    player_position = player_object:GetTransform().mTranslate
    

    -- For [1] Spiral Bullets (Ground)
    bulletSpawnPosition.x = spiralBulletSpawnPosition.x
    bulletSpawnPosition.y = spiralBulletSpawnPosition.y
    bulletSpawnPosition.z = spiralBulletSpawnPosition.z

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

    -- Tentative random switcher between boss states, replace with HP after other states implemented. 100% HP Left = Phase 1, 66% HP Left = Phase 2, 33% HP Left = Phase 3
    if _G.attacking == false then 

        -- [Recycle Attacks]
        -- Check if the current state checker has at least 4 "true" -> reset them 
        local state_true_counter = 0 

        for i = 1, #_G.state_checker do
            if _G.state_checker[i] == true then 
                state_true_counter = state_true_counter + 1
            end 
        end

        if state_true_counter >= 4 then 
            for i = 1, #_G.state_checker do
                if _G.state_checker[i] == false then 
                    state = i 
                    -- print("STATE CHOSEN: " , state)
                else 
                    _G.state_checker[i] = false -- reset it for next cycle use
                end 
            end
            PrintAttackingStates()
            
        else 
            state = math.random(1, 5)
            -- print("STATE CHOSEN: " , state)
        end


        -- currentBossStateTimer = 0
    end

    -- if currentBossStateTimer >= maxBossStateTimer then
     
    -- end

    -- Debug States
    -- state = 1 --[OK]
    -- state = 2 -- [OK] -- need to check agn after i check the other mechanics
    -- state = 3 -- [OK]
    -- state = 4 --[OK]
    -- state = 5 -- [OK]

    if state == 1 and _G.state_checker[1] == false then

        _G.attacking = true -- must include (to stop state choosing)

        -- Decide how many enemies to spawn this phase.
        if total_number_of_enemies_to_spawn == 0 then 
            total_number_of_enemies_to_spawn = math.random(4,7)
            print("NUMBER OF ENEMIES TO SPAWN: " , total_number_of_enemies_to_spawn)
        end

        -- Timer to set intervals between summons
        currentEnemySpawnResetTimer = currentEnemySpawnResetTimer + FPSManager.GetDT()
        -- print("TIMER: " , currentEnemySpawnResetTimer)

        if (currentEnemySpawnResetTimer >= maxEnemySpawnResetTimer) then

            -- Number of enemies to summon per position
            if(summon_per_spawn_instance == 0) then 
                summon_per_spawn_instance = math.random(2,3)
                print("SUMMON PER SPAWN INSTANCE: " , summon_per_spawn_instance)
            end

            print("CURRENTLY ENEMIES IN LV3: " , _G.number_of_spawned_in_level_3)
            print("TOTAL: " ,_G.number_of_spawned_in_level_3 + summon_per_spawn_instance)

            if(_G.number_of_spawned_in_level_3 +  summon_per_spawn_instance < total_number_of_enemies_to_spawn) then 
                
               print("NORMAL SUMMON")
               SummonMinions(summon_per_spawn_instance)
               currentEnemySpawnResetTimer = 0 -- Reset spawn time

            else -- if exceed the total amount. 
                        
                print("SPECIAL SUMMON")
                SummonMinions(total_number_of_enemies_to_spawn - _G.number_of_spawned_in_level_3)
                currentEnemySpawnResetTimer = 0 -- Reset spawn time
            end

            summon_per_spawn_instance   = 0 -- Reset number of enemies per spawn instance (for a new RNG) -> put here coz might exceed total number    
        end

        if(_G.number_of_spawned_in_level_3 >= total_number_of_enemies_to_spawn) then  -- Exit State (Condition)
            phase_1_timer = phase_1_timer + FPSManager.GetDT()
            print("PHASE TIMER: " , phase_1_timer)
        end

        if(phase_1_timer >= phase_1_max_time) then 
            print("SWITCHING OUT PHASE 1")
            _G.state_checker[1] = true 
            _G.attacking = false           -- attack done (exit state)
            PrintAttackingStates()
            phase_1_timer = 0              -- reset toimer
        end

    end

    if state == 2 and _G.state_checker[2] == false then

        _G.attacking = true -- must include (to stop state choosing)

        if groundSlamMax == 0 then 
          
            groundSlamMax = math.random(3, 5)
            -- print("GROUND SLAM MAX: " , groundSlamMax)
        end
        -- Timer to set intervals between ground slams
        -- print("Current Ground Slam Reset Timer: " , currentGroundSlamResetTimer)

        if groundSlamCount < groundSlamMax then 
            -- print("HI INSIDE HERE")
            currentGroundSlamResetTimer = currentGroundSlamResetTimer + FPSManager.GetDT()
            -- print("Current Ground Slam Reset Timer: " , currentGroundSlamResetTimer)

            if currentGroundSlamResetTimer >= maxGroundSlamResetTimer then

                -- Pick which direction to ground slam in 
                groundSlamDirection = math.random(1, 8)
                -- groundSlamDirection  = 2

                -- print("GROUND SLAM: " , groundSlamDirection)

                -- Ground slam (9 o'clock)
                if groundSlamDirection == 1 then
                    groundSlamPosition.x = 0
                    groundSlamPosition.y = 0
                    groundSlamPosition.z = 40
                end

                -- Ground slam (7 o'clock)
                if groundSlamDirection == 2 then
                    groundSlamPosition.x = 37   
                    groundSlamPosition.y = 0
                    groundSlamPosition.z = 37
                end

                -- Ground slam (6 o'clock)
                if groundSlamDirection == 3 then
                    groundSlamPosition.x = 50
                    groundSlamPosition.y = 0
                    groundSlamPosition.z = 8
                end

                  -- Ground slam (5 o'clock)
                if groundSlamDirection == 4 then
                    groundSlamPosition.x = 33
                    groundSlamPosition.y = 0
                    groundSlamPosition.z = -32
                end

                -- Ground slam (3 o'clock)
                if groundSlamDirection == 5 then
                    groundSlamPosition.x =  3
                    groundSlamPosition.y = 0
                    groundSlamPosition.z = -42
                end

                -- Ground slam (1 o'clock)
                if groundSlamDirection == 6 then
                    groundSlamPosition.x = -43
                    groundSlamPosition.y = 0
                    groundSlamPosition.z = -29
                end

                -- Ground slam (11o'clock)
                if groundSlamDirection == 7 then
                    groundSlamPosition.x = -43
                    groundSlamPosition.y = 0
                    groundSlamPosition.z = 39
                end

                -- Ground slam (12 o'clock)
                if groundSlamDirection == 8 then
                    groundSlamPosition.x = -60
                    groundSlamPosition.y = 0
                    groundSlamPosition.z = 2.3
                end

                -- TODO: Play arm swinging animation before spawning ground slam object
                roundSlam = systemManager.ecs:NewEntityFromPrefab("GroundSlamObject", groundSlamPosition)
                groundSlamCount = groundSlamCount + 1
                print("GROUND SLAM COUNT: ", groundSlamCount)
                currentGroundSlamResetTimer = 0 -- Reset ground slam timer
            -- else  -- Exit State (Condition)
            --     _G.state_checker[2] = true -- attack done (exit state)
            --     _G.attacking = false
            --     Print_G.attackingStates()
            end
          

        else 
            print("DONE SLAM")
            _G.state_checker[2] = true
            _G.attacking = false
            groundSlamMax = 0
        end

    end

    -- Bullet Types (Phases)
    -- 1. Bullet Hell (no homing) -> maybe add 2 variations to start
    -- 2. Homing Bullets (but dodgeable)

    
    -- [3] Bullet Hell #1 : Pulsing Spheres
    -- (a) State 1 : Normal Circles but pulsing in different directions & angles. 
    --     - Make it easier at the start 
    if state == 3 and _G.state_checker[3] == false then
        
        _G.attacking = true -- must include (to stop state choosing)

        fire_timer = fire_timer +  FPSManager.GetDT()

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
            else -- Exit State (Condition)
                _G.state_checker[3] = true
                _G.attacking = false
                PrintAttackingStates()
            end
        end
    end

    -- [4] Homing Eyeballs (From Boss' Mouth / Face)
    if state == 4 and _G.state_checker[4] == false then 
        -- print("INSIDE STATE 4")
        -- homing_spawn_counter = 0 
        _G.attacking = true -- must include (to stop state choosing)

        -- Initially -> decides the number of homing to spawn
        if number_of_homing == 0 then 
            number_of_homing = math.random(5, 8)
            -- print("NUMBER OF HOMING: " , number_of_homing)
        end

        -- Timer to spawn 1 by 1
        if homing_spawn_counter ~= number_of_homing then 
            -- print("SPAWNED ENOUGH")
            homing_spawn_timer = homing_spawn_timer + FPSManager.GetDT()
        else 
            print("SPAWNED ENOUGH")
        end

        -- print("HOMING SPAWN TIMER: " , homing_spawn_timer)
        
        if(homing_spawn_counter < number_of_homing) then 
            if(homing_spawn_timer > homing_spawn_period) then -- It's time to spawn another homing bullet
                SpawnHomingSpheres()
                homing_spawn_counter = homing_spawn_counter + 1 -- Increase the counter
                homing_spawn_timer = 0   -- Reset the counter
                -- print("SPAWNING ORB")
            end
        end

    

        if homing_spawned == true then
            UpdateHomingProjectiles()
        end
    end


    if state == 5 and _G.state_checker[5] == false then 
        -- print("LAZER ATTACK")
        _G.attacking = true -- must include (to stop state choosing)
        _G.activateLazerScript = true
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

-- local Vec3 = {}

-- Define Subtract method within Vec3 module
function Subtract(vector1, vector2)
    local result = Vec3.new()

    result.x = vector1.x - vector2.x
    result.y = vector1.y - vector2.y
    result.z = vector1.z - vector2.z

    return result
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
function SpawnHomingSpheres()
    -- Let's start by randomnizing the starting spawn point of the projectile
    bulletSpeed = 5

    -- Spawning Logic (Start)
    -- for i = 0 , number_of_bullets do 
    local attack_offset_range = 5  -- Defines a range to spread out from the central position to summon the bullets in different positions
                
    local random_offset_x = math.random(-attack_offset_range , attack_offset_range)
    local random_offset_y = math.random(-attack_offset_range , attack_offset_range)
    local random_offset_z = math.random(-attack_offset_range , attack_offset_range)
    
    local bullet_spawn_position = Vec3.new(spawn_point_translate.x + random_offset_x , 
                                        spawn_point_translate.y + random_offset_y , 
                                        spawn_point_translate.z + random_offset_z)

    -- homing_bullet = systemManager.ecs:NewEntityFromPrefab("Boss_Bullet_Homing",bullet_spawn_position)
    entity_ref = systemManager.ecs:NewEntityFromPrefab("Boss_Bullet_Homing",bullet_spawn_position)

    local homing_bullet = 
    {   
        id = next_homing_id,
        entity = entity_ref, 
        position = entity_ref:GetTransform().mTranslate,  -- Note that this is randomnized by the previous few lines
        direction = Vec3.new(0,0,0), 
        speed = 0,
        stay_time = 2.0,      -- Time to stay still before homing (in seconds)
        lock_on_time = 15.0,   -- Time to locks onto the player : Used as an internal timer (for each bullet to calculate how long to home and lock on)
        lock_on_bool = false, -- To control when to home and when not to
    }

    next_homing_id = next_homing_id + 1 -- Increment the id counter.

    -- local test_transform = homing_bullet:GetTransform().mTranslate
    -- local test_transform = projectile.entity:GetTransform()
    -- print("Transform: " , homing_bullet.entity:GetTransform().mTranslate)
    

    -- local homing_bullet = systemManager.ecs:NewEntityFromPrefab("Boss_Bullet_Homing",bullet_spawn_position)

    -- print("THIS HOMING BULLET POSITION: " , homing_bullet.position.x , " , " , homing_bullet.position.y  , " , " , homing_bullet.position.z)
    table.insert(_G.homing_projectiles, homing_bullet)

    homing_spawned = true -- to trigger "Update" Loop
    -- number_of_homing_spawned = number_of_homing_spawned + 1

    -- _G.bulletCounter = _G.bulletCounter + 1
    

        
    -- end
    -- Spawning Logic (End)

end

function UpdateHomingProjectiles()
    -- local active_projectiles = 0 -- Counter to keep track of active projectiles
    
    for i , projectile in ipairs(_G.homing_projectiles) do
        -- print("ID: " , projectile.id)
        if projectile.entity ~= nil then 
            -- counter = counter + 1
            if projectile.speed == 0 then  -- projectile speed is set to 0 at the start
                -- print("STAYING STILL")
                -- Stay still for awhile
                projectile.stay_time = projectile.stay_time - FPSManager.GetDT()  
                -- print("PROJECTILE STAY TIME: " , projectile.stay_time)
                if projectile.stay_time <= 0 then 
                    -- Starts homing towards the player (by providing speed)
                    -- print("Assigning Initial Speed - Homing")
                    projectile.speed = initial_homing_speed
                end
            else
                -- Lock onto the player after a delay
                -- print("Projectile No : " , _G.bulletCounter)
                -- print("PROJECTILE LOCK TIME: " , projectile.lock_on_time)
                projectile.lock_on_time = projectile.lock_on_time - FPSManager.GetDT()
                if projectile.lock_on_time > 0 then 
                    -- print("CALCULATING DIRECTION TO GO.")
                        
                    -- Subtract Vector 
                    local directionToPlayer = Subtract(player_position, projectile.position)
                    
                    -- print("PROJECTILE POS: " , projectile.position.x , ", " , projectile.position.y , ", " , projectile.position.z)
                    -- print("DIRECTION TO PLAYER: " , directionToPlayer.x ,  " , " , directionToPlayer.y , " , " , directionToPlayer.z)
                    directionToPlayer = Helper.Normalize(directionToPlayer)
                    -- print("NORMALIZED DIRECTION TO PLAYER: " , directionToPlayer.x ,  " , " , directionToPlayer.y , " , " , directionToPlayer.z)

                    directionToPlayer.x = directionToPlayer.x * projectile.speed
                    directionToPlayer.y = directionToPlayer.y * projectile.speed
                    directionToPlayer.z = directionToPlayer.z * projectile.speed

                    -- Save the direction 
                    projectile.direction = directionToPlayer
                    
                    -- Ensure projectile.entity is not nil before setting velocity
                    if projectile.entity ~= nil then 
                        -- print("ENTITY NAME: " , projectile.entity:GetGeneral().name)
                        -- print("Entity Exists")
                        -- print("SETTING VELOCITY")
                        physicsSys:SetVelocity(projectile.entity,  projectile.direction)
                        -- physicsSys:SetVelocity(projectile.entity, random_vec3)
                    else
                        print("Projectile entity is nil!")
                    end
                    -- print("SETTING VELOCITY")
                    -- print("DIRECTION TO PLAYER: " , directionToPlayer.x ,  " , " , directionToPlayer.y , " , " , directionToPlayer.z)
                    -- physicsSys:SetVelocity(projectile.entity, directionToPlayer)
                end
            end
        end
    end

    if #_G.homing_projectiles == 0 then 
        print("EMPTY LIAO")
        _G.state_checker[4] = true
        homing_spawned = false
        homing_spawn_timer = 0    -- need to reset 
        homing_spawn_counter = 0  -- Reset number of homing spawned
        number_of_homing = 0      -- Reset number of homing required
        _G.attacking = false           -- important to toggle state choose again
        PrintAttackingStates()
    end
end 

function SummonMinions(summon_per_spawn_instance) 
    print("Number of Enemies to Summon: " , summon_per_spawn_instance)

    -- Pick which direction to ground slam in 
    enemySpawnDirection = math.random(1, 4)
    enemySpawnDirection = 1
    print("ENEMY SPAWN DIRECTION: " , enemySpawnDirection)

    -- Summon Area (In Front of Boss)
    if enemySpawnDirection == 1 then
        groundSlamPosition.x = 43.2
        groundSlamPosition.y = 0.697
        groundSlamPosition.z = 7
    end

    -- Summon Area (Left of Boss)
    if enemySpawnDirection == 2 then
        groundSlamPosition.x = 0
        groundSlamPosition.y = 0
        groundSlamPosition.z = 40

    end

    -- Summon Area (Right of Boss)
    if enemySpawnDirection == 3 then
        groundSlamPosition.x = 5.6  
        groundSlamPosition.y = 0
        groundSlamPosition.z = -35
    end

    -- Summon Area (Behind Boss)
    if enemySpawnDirection == 4 then
        groundSlamPosition.x = -33.7
        groundSlamPosition.y = 0
        groundSlamPosition.z = -0.09
    end


    -- [3/6] - Set to summon multiple enemies in 1 area 
    for i = 1 , summon_per_spawn_instance  do 
        enemyType = math.random(1, 4)
        print("ENEMY TYPE: " , enemyType)
        print("SUMMON # " , _G.number_of_spawned_in_level_3)
        _G.number_of_spawned_in_level_3 = _G.number_of_spawned_in_level_3 + 1
        _G.number_left_in_level_3 = _G.number_left_in_level_3 + 1

        if enemyType == 1 then
            enemySpawn = systemManager.ecs:NewEntityFromPrefab("Melissa", groundSlamPosition)
        end

        if enemyType == 2 then
            enemySpawn = systemManager.ecs:NewEntityFromPrefab("ILOVEYOU", groundSlamPosition)
        end

        if enemyType == 3 then
            enemySpawn = systemManager.ecs:NewEntityFromPrefab("ZipBomb", groundSlamPosition)
        end

        if enemyType == 4 then
            enemySpawn = systemManager.ecs:NewEntityFromPrefab("TrojanHorse", groundSlamPosition)
        end

    end
end


function PrintAttackingStates()
    for i = 1, #_G.state_checker do
        print("Phase " .. i .. ": " .. tostring(_G.state_checker[i]))
    end

    print("ATTACKING: " , _G.attacking)
end

-- function UpdateHomingProjectiles()
--     for i , projectiles in

-- end

