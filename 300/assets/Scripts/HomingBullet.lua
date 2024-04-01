local bulletObject
local bulletLifeTime = 0
local bulletDeathTime = 15

-- Phase 3 attack from Boss Level (Lv 3) 
local bullet_damage = 6 -- Adjust Accordingly


local general
local spawn_bullet_name = "Boss_Bullet_Homing"
local gameStateSys

local dmgAudioEnt

-- Player Stuff
local player 
local player_position = Vec3.new()
local direction_to_player = Vec3.new()

-- Homing Missle Stuff
local this
local homing_transform = Vec3.new()
local homing_position = Vec3.new()
local forward_vector = Vec3.new(0,0,1)
local stay_time = 2.0                     -- Time to stay still
local initial_homing_speed = 8            -- Speed to set when the wait time is over.
local homing_speed = 0                    -- Current Homing Speed (initialied to '0' at first)
local homing_duration = 10.0              -- Time to home in 

function Alive()
    bulletObject = Helper.GetScriptEntity(script_entity.id)
    gameStateSys = systemManager:mGameStateSystem()
    physicsSys = systemManager:mPhysicsSystem()


    dmgAudioEnt = gameStateSys:GetEntity("DamageAudio")

    -- Player
    player = gameStateSys:GetEntity("Camera")
    player_position = player:GetTransform().mTranslate

    -- Homing Missle
    this = Helper.GetScriptEntity(script_entity.id)
    general = this:GetGeneral()

    homing_transform = this:GetTransform()
    homing_position = this:GetTransform().mTranslate

    -- print("Player Position: " , player_position.x , player_position.y , player_position.z)
    print("HOMING BULLET: " , homing_position.x , homing_position.y, homing_position.z)

    -- print("HOMING BULLET SPAWNED")


    -- if player ~= nil then 
    --     print("PLAYER OK")
    -- end

    -- homing_chuckle_audio = this:GetAudio()
end

function Update()

    -- [Homing Missle] -> Health stuff
    if this:GetHealthbar().health <= 0 then 
        print("BULLET DED")
        systemManager.ecs:SetDeleteEntity(this) -- Delete the bullet upon contact
     end

     bulletLifeTime = bulletLifeTime + FPSManager.GetDT()

        if(bulletLifeTime > bulletDeathTime) then
            systemManager.ecs:SetDeleteEntity(bulletObject)
        end

        -- for i , projectile in ipairs(_G.homing_projectiles) do
        -- if this == projectile.entity then 
        --     if(bulletLifeTime > bulletDeathTime) then
        --         table.remove(_G.homing_projectiles, i) -- Delete entry in the table.
        --         systemManager.ecs:SetDeleteEntity(this) -- Delete the bullet upon contact
        --         break
        --     end
        -- end
        -- end
     -- [Homing Missle] -> Health stuff (end)


     -- [Homing Logic]
     if stay_time > 0 then 
         stay_time = stay_time - FPSManager.GetDT()
     end

     if stay_time < 0 then 
        homing_duration = homing_duration - FPSManager.GetDT()
     end

    --  print("HOMING DURATION: " , homing_duration)

     if stay_time <= 0 then
        homing_speed = initial_homing_speed
     end
     
    if homing_duration > 0 and homing_speed ~= 0 then 
        direction_to_player = Subtract(player_position, this:GetTransform().mTranslate)  
        direction_to_player = Helper.Normalize(direction_to_player)

        -- print("DIRECTION TO PLAYER :" , direction_to_player.x, direction_to_player.y, direction_to_player.z)

        direction_to_player.x =  direction_to_player.x * homing_speed
        direction_to_player.y =  direction_to_player.y * homing_speed
        direction_to_player.z =  direction_to_player.z * homing_speed

        -- Homing looking at you... (to a certain extent)
        local angle_to_rotate = CalculateAngle(this:GetTransform().mTranslate, player_position, forward_vector)
        local axis_to_rotate = Vec3.new()
        axis_to_rotate.x = 0 
        axis_to_rotate.y = angle_to_rotate * (180 / math.pi)
        axis_to_rotate.z = 0

        Helper.SetRealRotateQuaternion(this, axis_to_rotate, angle_to_rotate)

        physicsSys:SetVelocity(this, direction_to_player)
    end


     -- End of [Homing Logic]
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()

    tagid = generalComponent.tagid
    -- print("TAG ID: " , tagid)

    local player_healthbar_component

    -- print("ON TRIGGER ENTER BOSS BULLET")

    if(tagid == 0) then 
        -- print("HIT BULLET")
        if(Entity:HasHealthbar()) then 
            -- print("HAS HEALTHBAR")
            player_healthbar_component = Entity:GetHealthbar()
        end

        if(player_healthbar_component ~= nil) then 
            player_healthbar_component.health = player_healthbar_component.health - bullet_damage
            -- print("TAKING DAMAGE FROM HOMING BULLET")
            -- print("DAMAGE TAKEN: " , bullet_damage)
            -- print("HP LEFT: ", player_healthbar_component.health)
            dmgAudioEnt:GetAudio():SetPlay(0.5)
            systemManager.ecs:SetDeleteEntity(this)
        end
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end

-- Helper Functions (Math)
function Subtract(vector1, vector2)
    local result = Vec3.new()

    result.x = vector1.x - vector2.x
    result.y = vector1.y - vector2.y
    result.z = vector1.z - vector2.z

    return result
end

-- M6 : Calculate Homing Bullet's Orientation (angle to rotate) -> bullet to player
function CalculateAngle(bullet_pos, player_position, forward_vector)

    -- Calculate the direction vector from bullet to player
    local direction = Vec3.new()

    direction.x = player_position.x - bullet_pos.x 
    direction.y = player_position.y - bullet_pos.y 
    direction.z = player_position.z - bullet_pos.z

    -- Normalize the vector/direction
    local directionNorm = Helper.Normalize(direction)

    -- Calculates the dot product between [bullet's forward direction] & [direction from bullet to player - normalized]
    local dot_product_forward = dotProduct(forward_vector, directionNorm) 

 
    -- Calculate the angle between the bullet's forward direction & the direction vector.
    -- [forward direction] -> represents the object's orientation or where it is facing. 
    local angle = math.acos(dot_product_forward)

    -- Use <Cross Product> to determine if angle is positive / negative
    local cross = crossProduct(forward_vector, directionNorm)
    -- print("ANGLE: " , angle * (180 / math.pi))

    local l = dotProduct(cross, cross)
    
    return angle
end


function crossProduct(v1, v2)
    local x = v1.y * v2.z - v1.z * v2.y
    local y = v1.z * v2.x - v1.x * v2.z
    local z = v1.x * v2.y - v1.y * v2.x
    
    return {x = x, y = y, z = z}
end

function dotProduct(v1, v2)
    local result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
    return result
end