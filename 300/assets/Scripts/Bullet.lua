viewVec = Vec3.new(0,0,0)
movement = Vec3.new(0,0,0)
spawn = true
timer = 0
direction = Vec3.new(10,0,0)
colors = Vec4.new(0,0,0,1)
positions = Vec3.new(0,0,0)


color1 = Vec4.new(1,0,0,1)
color2 = Vec4.new(0,1,0,1)
color3 = Vec4.new(0,0,1,1)
color4 = Vec4.new(0,1,1,1)
color5 = Vec4.new(1,0,1,1)
color6 = Vec4.new(1,1,0,1)
color7 = Vec4.new(1,1,1,1)

allcolor = {Vec4.new(1,0,0,1),Vec4.new(0,1,0,1),Vec4.new(0,0,1,1),Vec4.new(0,1,1,1),Vec4.new(1,0,1,1),Vec4.new(1,1,0,1),Vec4.new(1,1,1,1)}

local entityobj

local bullethitAudioEntity
local bullethitAudioComp

local bulletObject
local bulletLifeTime = 0
local bulletDeathTime = 150


-- Testing Damage System
local bulletTag = "REVOLVER" -- "REVOLVER" , "SHOTGUN" , "MACHINE GUN"
local enemytag1HP = 100
local shotGunDamage = 12 -- per bullet
local revolverDamage = 35 -- per bullet
local machineGunDamage = 3 -- per bullet


-- local bullethitEntity
-- local bullethitcomp
-- local bullethitAudioSource 


function Alive()
    
end

function Update()

    gameStateSys = systemManager:mGameStateSystem()
    cameraEntity = gameStateSys:GetEntity("Camera")
    positions = cameraEntity:GetTransform().mTranslate


    viewVec = Camera_Scripting.GetDirection(cameraEntity)
    physicsSys = systemManager:mPhysicsSystem()

    bullethitAudioEntity = gameStateSys:GetEntity("Bullet Hit")
    bullethitAudioComp = bullethitAudioEntity:GetAudio()
    -- bullethitAudioSource = Helper.CreateAudioSource(bullethitEntity)

    bulletObject = Helper.GetScriptEntity(script_entity.id)
    bulletGeneral = bulletObject:GetGeneral()

    bulletTag = bulletGeneral:GetTag()

    while bulletObject ~= nil do 
       --print("THERE IS A BULLET OBJECT")
        bulletLifeTime = bulletLifeTime + 1
        -- print("BULLET LIFETIME: " , bulletLifeTime)
        if(bulletLifeTime > bulletDeathTime) then
            systemManager.ecs:SetDeleteEntity(bulletObject)
            print("DELETING BULLET")
        end
        break
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()
    entityobj = Helper.GetScriptEntity(script_entity.id)

    tagid = generalComponent.tagid
    if (tagid == 1) then
        for i = 7, 1, -1
        do
            spawned(i)
        end
        gameStateSys = systemManager:mGameStateSystem()
        bullethitAudioComp:SetPlay(0.2)

        Entity:GetTransform().mScale.x = Entity:GetTransform().mScale.x * 0.9
        Entity:GetTransform().mScale.y = Entity:GetTransform().mScale.y * 0.9
        Entity:GetTransform().mScale.z = Entity:GetTransform().mScale.z * 0.9


                
        systemManager.ecs:SetDeleteEntity(entityobj)
    end

    if (tagid == 3) then
        for i = 7, 1, -1
        do
            spawned(i)
        end
        --floorCount = floorCount + 1;
        -- gameStateSys = systemManager:mGameStateSystem();
        -- gameStateSys:DeleteEntity(entityobj)
        systemManager.ecs:SetDeleteEntity(entityobj)
    end
    if (tagid == 4) then
        for i = 7, 1, -1
        do
            spawned(i)
        end
        --floorCount = floorCount + 1;
        -- gameStateSys = systemManager:mGameStateSystem();
        -- gameStateSys:DeleteEntity(entityobj)
        systemManager.ecs:SetDeleteEntity(entityobj)
    end


    if (tagid == 1) then
        for i = 7, 1, -1
        do
            spawned(i)
        end

        if(bulletTag == "REVOLVER") then 
            enemytag1HP = enemytag1HP - revolverDamage
            print("ENEMY TAKING DAMAGE: ", revolverDamage)
            print("HP LEFT: " , enemytag1HP)
        end

        if(bulletTag == "SHOTGUN") then 
            enemytag1HP = enemytag1HP - shotGunDamage
            print("ENEMY TAKING DAMAGE: ", shotGunDamage)
            print("HP LEFT: " , enemytag1HP)
        end

        if(bulletTag == "MACHINE_GUN") then 
            enemytag1HP = enemytag1HP - machineGunDamage
            print("ENEMY TAKING DAMAGE: ", machineGunDamage)
            print("HP LEFT: " , enemytag1HP)
        end

        if(enemytag1HP <= 0) then 
            print("ENEMY DIES")
            systemManager.ecs:SetDeleteEntity(entityobj)
        end
    end

end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end


function spawned(value)

    direction.x = math.random(-5,5)
    direction.y = math.random(-5,5)
    direction.z = math.random(-5,5)
    
    total = (direction.x +direction.y +direction.z)
    -- colors.x = direction.x/5
    -- colors.y = direction.y/5
    -- colors.z = direction.z/5
    direction.x =( direction.x/10)*15
    direction.y = (direction.y/10)*15
    direction.z = (direction.z/10)*15
    positions = entityobj:GetTransform().mTranslate

    prefabEntity = systemManager.ecs:NewEntityFromPrefab("parti",positions )   

    meshSys = prefabEntity:GetMeshRenderer()
    meshSys:SetColor(allcolor[value])
    physicsSys = systemManager:mPhysicsSystem()
    physicsSys:SetVelocity(prefabEntity, direction)
end