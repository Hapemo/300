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

-- Phase 2 : Groundslam Phase
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

-- [a] Spiral Bullets (Ground)
local bullet_attack_checker = {false, false, false}     -- keep track which attacks have been used so won't activate back to back
local attacking = false
local number_of_fire = 0
local stop_firing_at = 10

-- [b] Summon Bullet + Homing Bullets (From Boss' Mouth / Front) 
--     -> Players have to shoot them to break them
local spawn_point_ref_obj
local spawn_point_ref_trans
local spawn_point_translate = Vec3.new()
local number_of_homing
local homing_spawned = false
local homing_spawn_timer = 0                            -- Timer that increament with DT
local homing_spawn_period = 1.0                         -- Time to spawn (in between each sphere)
local homing_spawn_counter = 0                          -- Counter to keep track how many homing bullets have been spawned
local homing_start = false

local homing_bullet
local homing_projectiles = {}                           -- Define a table to store projectile data
local projectile_stay_time = 2                          -- timer for the bullet to stay still before it starts homing into the player
local initial_homing_speed = 6                          -- Starting Homing Speed


-- [4] Lazer Attack 
_G.activateLazerScript = false

-- Boss states
local state = 0
local state_checker = {false, false , false , false}
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
    bossPosition = this:GetTransform().mTranslate
    spawn_point_ref_obj = gameStateSys:GetEntityByScene("Spawn_Point_Ref" , "BossStuff")
    spawn_point_ref_trans = spawn_point_ref_obj:GetTransform().mTranslate

    -- Player 
    player_object = gameStateSys:GetEntityByScene("Camera" , "test3")
    player_position = player_object:GetTransform().mTranslate
    

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
    if currentBossStateTimer >= maxBossStateTimer and attacking == false then
        state = math.random(1, 4)
        print("STATE CHOSEN: " , state)
        currentBossStateTimer = 0
    end

    -- state = 2 -- For testing only to force state 2, delete afterwards
    -- state = 3
    state = 1

    if state == 1 and state_checker[1] == false then

        attacking = true

        -- Decide how many enemies to spawn this phase.
        if total_number_of_enemies_to_spawn == 0 then 
            total_number_of_enemies_to_spawn = math.random(6,10)  
            print("NUMBER OF ENEMIES TO SPAWN: " , total_number_of_enemies_to_spawn)
        end

        -- Timer to set intervals between summons
        currentEnemySpawnResetTimer = currentEnemySpawnResetTimer + FPSManager.GetDT()
        -- print("TIMER: " , currentEnemySpawnResetTimer)

        if (currentEnemySpawnResetTimer >= maxEnemySpawnResetTimer) then

            -- Number of enemies to summon per position
            if(summon_per_spawn_instance == 0) then 
                summon_per_spawn_instance = math.random(2,3)
            end

            print("CURRENTLY ENEMIES IN LV3: " , _G.number_of_spawned_in_level_3)
            print("TOTAL: " ,_G.number_of_spawned_in_level_3 + summon_per_spawn_instance)

            if(_G.number_of_spawned_in_level_3 +  summon_per_spawn_instance < total_number_of_enemies_to_spawn) then 
                
               SummonMinions(summon_per_spawn_instance)
               currentEnemySpawnResetTimer = 0 -- Reset spawn time

            else -- if exceed the total amount. 
                SummonMinions(total_number_of_enemies_to_spawn - _G.number_of_spawned_in_level_3)
                currentEnemySpawnResetTimer = 0 -- Reset spawn time
            end

            summon_per_spawn_instance   = 0 -- Reset number of enemies per spawn instance (for a new RNG) -> put here coz might exceed total number
        end




    end

    if state == 2 and state_checker[2] == false then

        -- Timer to set intervals between ground slams
        currentGroundSlamResetTimer = currentGroundSlamResetTimer + FPSManager.GetDT()

        if currentGroundSlamResetTimer >= maxGroundSlamResetTimer then

            -- Pick which direction to ground slam in 
            groundSlamDirection = math.random(1, 3)

            -- Ground slam front (from boss perspective)
            if groundSlamDirection == 1 then
                groundSlamPosition.x = 0
                groundSlamPosition.y = -6
                groundSlamPosition.z = 25
            end

            -- Ground slam right (from boss perspective)
            if groundSlamDirection == 2 then
                groundSlamPosition.x = -15
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
            bulletProjectileType = math.random (1, 2)
            -- bulletProjectileType = 1 -- temporary
            -- print("PROJECTILE ATTACKING TYPE: "  , bulletProjectileType)
            attacking = true
        end

        -- Check if this projectile has been recently used... 
        if(bullet_attack_checker[bulletProjectileType] == false) then 

            -- [1] Spiral Bullets (Ground)
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

            -- [2] Summon Bullet + Homing Bullets (From Boss' Mouth / Front) 
            if(bulletProjectileType == 2) then 
                number_of_homing = math.random(5, 8)
                -- print("NUMBER OF HOMING: " , number_of_homing)

                if homing_spawned == false then 
                    -- TODO : Set Timer to spawn 1 by 1 
                    homing_spawn_timer = homing_spawn_timer + FPSManager.GetDT()

                    -- print("HOMING SPAWN TIMER: " , homing_spawn_timer)
                    
                    if(homing_spawn_counter < number_of_homing) then 
                        if(homing_spawn_timer > homing_spawn_period) then -- It's time to spawn another homing bullet
                            SpawnHomingSpheres()
                            homing_spawn_counter = homing_spawn_counter + 1 -- Increase the counter
                            homing_spawn_timer = 0   -- Reset the counter
                            -- print("SPAWNING ORB")
                        end
                     
                    end
                    
                    -- for i = 0 , number_of_homing do
                    --     SpawnHomingSpheres()
                    -- end
                    -- print("SPAWNED HOMING")
                end

                -- print("Homing Start: " , homing_start)
                if homing_start == false then 
                    UpdateHomingProjectiles()
                end

            end
        end

        -- UpdateHomingProjectiles(0)
    end

    if state == 4 and state_checker[4] == false then 
        print("LAZER ATTACK")
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
        entity = entity_ref, 
        position = entity_ref:GetTransform().mTranslate,  -- Note that this is randomnized by the previous few lines
        direction = Vec3.new(0,0,0), 
        speed = 0,
        stay_time = 2.0,      -- Time to stay still before homing (in seconds)
        lock_on_time = 15.0,   -- Time to locks onto the player : Used as an internal timer (for each bullet to calculate how long to home and lock on)
        lock_on_bool = false, -- To control when to home and when not to
    }

    -- local test_transform = homing_bullet:GetTransform().mTranslate
    -- local test_transform = projectile.entity:GetTransform()
    -- print("Transform: " , homing_bullet.entity:GetTransform().mTranslate)
    

    -- local homing_bullet = systemManager.ecs:NewEntityFromPrefab("Boss_Bullet_Homing",bullet_spawn_position)

    -- print("THIS HOMING BULLET POSITION: " , homing_bullet.position.x , " , " , homing_bullet.position.y  , " , " , homing_bullet.position.z)
    table.insert(homing_projectiles, homing_bullet)

    -- _G.bulletCounter = _G.bulletCounter + 1
    

        
    -- end
    -- Spawning Logic (End)

end

function UpdateHomingProjectiles()
    -- local counter = 0
    for i , projectile in ipairs(homing_projectiles) do
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
end 

function SummonMinions(summon_per_spawn_instance) 
    print("Number of Enemies to Summon: " , summon_per_spawn_instance)

    -- Pick which direction to ground slam in 
    enemySpawnDirection = math.random(1, 3)
    
    print("ENEMY SPAWN DIRECTION: " , enemySpawnDirection)

    -- Ground slam front (from boss perspective)
    if enemySpawnDirection == 1 then
        groundSlamPosition.x = 0
        groundSlamPosition.y = 2
        groundSlamPosition.z = 25
    end

    -- Ground slam right (from boss perspective)
    if enemySpawnDirection == 2 then
        groundSlamPosition.x = -15
        groundSlamPosition.y = 2
        groundSlamPosition.z = 25
    end

    -- Ground slam left (from boss perspective)
    if enemySpawnDirection == 3 then
        groundSlamPosition.x = 15
        groundSlamPosition.y = 2
        groundSlamPosition.z = 25
    end



    print("ENEMY TYPE: " , enemyType)

    -- [3/6] - Set to summon multiple enemies in 1 area 
    for i = 0 , summon_per_spawn_instance  do 
        enemyType = math.random(1, 4)
        print("SUMMON # " , _G.number_of_spawned_in_level_3)
        _G.number_of_spawned_in_level_3 = _G.number_of_spawned_in_level_3 + 1

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




-- function UpdateHomingProjectiles()
--     for i , projectiles in

-- end

