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
local bulletTag = "PISTOL" -- "REVOLVER" , "SHOTGUN" , "MACHINE GUN"
local enemytag1HP = 100
local pistolDamage = 15   -- per bullet (added on 1/31)
local shotGunDamage = 12 -- per bullet
local revolverDamage = 75 -- per bullet (adjusted damage on 2/4)
local machineGunDamage = 1 -- per bullet


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

    -- bullethitAudioEntity = gameStateSys:GetEntity("Bullet Hit")
    -- bullethitAudioComp = bullethitAudioEntity:GetAudio()
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
            -- print("DELETING BULLET")
        end
        break
    end

end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()

    --yujun's hardcode :D
    if (generalComponent.name == "tutorialTrojan") then
        entityobj = Helper.GetScriptEntity(script_entity.id)
        systemManager.ecs:SetDeleteEntity(entityobj)
        testScriptEntity = gameStateSys:GetEntity("tutorialTrojan")
        TestScripts = testScriptEntity:GetScripts()
        testScript = TestScripts:GetScript("../assets/Scripts/TutorialTrojanSoldier.lua")
        if testScript ~= nil then
            testScript:RunFunction("KillMe")
        end
    end

    -- print("HIT, " , generalComponent.name)

    local healthComponent 
    
    if(Entity:HasHealthbar()) then 
        -- print("HIT AN ENTITY WITH HEALTHBAR")
        healthComponent = Entity:GetHealthbar()
    end


    if(healthComponent ~= nil) then 
        -- print("HEALTH COMPONENT ASSIGNED")
        -- print("HEALTH:" , healthComponent.health)
    end

    entityobj = Helper.GetScriptEntity(script_entity.id)

    -- print("THIS ENTITY is: " , generalComponent.name)

    tagid = generalComponent.tagid
    if (tagid == 1) then
        for i = 3, 1, -1
        do
            spawned(i)
        end
        gameStateSys = systemManager:mGameStateSystem()

        if(bulletTag == "PISTOL") then 
            if(healthComponent ~= nil) then 
                healthComponent.health = healthComponent.health - pistolDamage * _G.powerLevel
                -- print("PISTOL HIT")
                -- print("HP Left: ", healthComponent.health)
                if(healthComponent.health <= 0 ) then
                    systemManager.ecs:SetDeleteEntity(Entity)
                    -- Interacts with "TutorialMonsterSpawner.lua"
                    if (generalComponent.name == "TrojanHorse") then 
                        _G.trojan_1_instance = false
                    end
                    if (generalComponent.name == "Melissa") then 
                        _G.Melissa_1_instance = false
                    end

                    if (generalComponent.name == "TrojanSoldier") then 
                        _G.trojan_soldier_1_instance = false
                    end
                end
            else

            
            end
        end

        if(bulletTag == "REVOLVER") then 
            if(healthComponent ~= nil) then 
                healthComponent.health = healthComponent.health - revolverDamage * _G.powerLevel
                -- print("DAMAGE (REVOLVER): " , revolverDamage * _G.powerLevel)
                if(healthComponent.health <= 0 ) then
                    systemManager.ecs:SetDeleteEntity(Entity)
                    -- Interacts with "TutorialMonsterSpawner.lua"
                    if (generalComponent.name == "TrojanHorse") then 
                        _G.trojan_1_instance = false
                    end
                    if (generalComponent.name == "Melissa") then 
                        _G.Melissa_1_instance = false
                    end

                    if (generalComponent.name == "TrojanSoldier") then 
                        _G.trojan_soldier_1_instance = false
                    end
                end
            else
            end
        end

        if(bulletTag == "SHOTGUN") then 
            if(healthComponent ~= nil) then 
                healthComponent.health = healthComponent.health - shotGunDamage * _G.powerLevel
                if(healthComponent.health <= 0 ) then
                    systemManager.ecs:SetDeleteEntity(Entity)

                    -- Interacts with "TutorialMonsterSpawner.lua"
                    if (generalComponent.name == "TrojanHorse") then 
                    _G.trojan_1_instance = false
                    end
                    if (generalComponent.name == "Melissa") then 
                        _G.Melissa_1_instance = false
                    end

                    if (generalComponent.name == "TrojanSoldier") then 
                        _G.trojan_soldier_1_instance = false
                    end
                end
            else
            end
        end

        if(bulletTag == "MACHINE_GUN") then 
            if(healthComponent ~= nil) then 
                healthComponent.health = healthComponent.health - machineGunDamage * _G.powerLevel
                if(healthComponent.health <= 0 ) then
                    systemManager.ecs:SetDeleteEntity(Entity)

                    -- Interacts with "TutorialMonsterSpawner.lua"
                    if (generalComponent.name == "TrojanHorse") then 
                    _G.trojan_1_instance = false
                    end
                    if (generalComponent.name == "Melissa") then 
                        _G.Melissa_1_instance = false
                    end

                    if (generalComponent.name == "TrojanSoldier") then 
                        _G.trojan_soldier_1_instance = false
                    end
                
                end
            end

        end

        -- Start Cooldown (for spawning)
        if(healthComponent.health <= 0 ) then
            _G.respawn_timer = 0.5 -- interacts with "TutorialMonstrSpawner.lua"
            _G.one_instance = false
        end
        -- print("BLLET TAG:" , bulletTag)
        -- if(bulletTag == "REVOLVER") then 
        --     print("REVOLVER BULLET")
        -- end
        -- bullethitAudioComp:SetPlay(0.2)

        -- Entity:GetTransform().mScale.x = Entity:GetTransform().mScale.x * 0.9
        -- Entity:GetTransform().mScale.y = Entity:GetTransform().mScale.y * 0.9
        -- Entity:GetTransform().mScale.z = Entity:GetTransform().mScale.z * 0.9


                
        systemManager.ecs:SetDeleteEntity(entityobj)
    end

    -- if (tagid == 3) then
    --     for i = 3, 1, -1
    --     do
    --         spawned(i)
    --     end
    --     --floorCount = floorCount + 1;
    --     -- gameStateSys = systemManager:mGameStateSystem();
    --     -- gameStateSys:DeleteEntity(entityobj)
    --     systemManager.ecs:SetDeleteEntity(entityobj)
    -- end
    -- if (tagid == 4) then
    --     for i = 7, 1, -1
    --     do
    --         spawned(i)
    --     end
    --     --floorCount = floorCount + 1;
    --     -- gameStateSys = systemManager:mGameStateSystem();
    --     -- gameStateSys:DeleteEntity(entityobj)
    --     systemManager.ecs:SetDeleteEntity(entityobj)
    -- end


    -- if (tagid == 1) then
    --     for i = 3, 1, -1
    --     do
    --         spawned(i)
    --     end
    --     if(enemytag1HP <= 0) then 
    --         print("ENEMY DIES")
    --         systemManager.ecs:SetDeleteEntity(entityobj)
    --     end
    -- end

end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end


function spawned(value)
    positions = entityobj:GetTransform().mTranslate

    prefabEntity = systemManager.ecs:NewEntityFromPrefab("parti",positions )   

    meshSys = prefabEntity:GetMeshRenderer()
    meshSys:SetColor(allcolor[value])
end

