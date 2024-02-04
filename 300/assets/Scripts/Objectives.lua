
local this

-- for objectives bar
local objectivesComplete = 500
local progress
local isInZone = false

local currentSpawnTimer = 0 -- keeps track of how often enemy spawning interval
local spawnTimer = 10 -- sets how long the enemy spawning interval is
local currentEnemyCount = 0 -- keeps track of how many enemies there are in the map
local maxEnemyCount = 3 -- sets how many enemies are allowed in the map

local mobSpawnPos1 = Vec3.new()
local mobSpawnPos2 = Vec3.new()
local mobSpawnPos3 = Vec3.new()
local mobSpawnPos4 = Vec3.new()

local objectiveBarSpawnPos = Vec3.new()
local objCount = 0

-- for spawning 0s
local spawndataPos = Vec3.new()
local moveTime = 0
local reseed = 0
-- end

local mobtype = 0;
local isInit

function Alive()
    isInit = false
    this = Helper.GetScriptEntity(script_entity.id)
    progress = 0
    print("Progress is", progress)

    mobSpawnPos1.x = 18;
    mobSpawnPos1.y = -9;
    mobSpawnPos1.z = 60;

    mobSpawnPos2.x = -22;
    mobSpawnPos2.y = -9;
    mobSpawnPos2.z = 40;

    mobSpawnPos3.x = -22;
    mobSpawnPos3.y = -9;
    mobSpawnPos3.z = -8;

    mobSpawnPos4.x = 8;
    mobSpawnPos4.y = -9;
    mobSpawnPos4.z = 13;
end

function Update()
    if isInit == false then
        gameStateSys = systemManager:mGameStateSystem();
        testScriptEntity = gameStateSys:GetEntity("Controller")
        TestScripts = testScriptEntity:GetScripts()
        testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")

        if testScript ~= nil then
            testScript:RunFunction("AddObjective")
        end

        objectiveBarSpawnPos.x = 0.8;
        objectiveBarSpawnPos.y = 0.65;
        objectiveBarSpawnPos.z = 0;
        objectivebar = systemManager.ecs:NewEntityFromPrefab("Objective Bar 1", objectiveBarSpawnPos)

        isInit = true
    end

        gameStateSys = systemManager:mGameStateSystem();
        testScriptEntity = gameStateSys:GetEntity("Controller")
        TestScripts = testScriptEntity:GetScripts()
        testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")

        if testScript ~= nil then
            objCount = testScript:ReturnValueInt("GetCountObj")

            if objCount == 3 then
                objectivebar:GetTransform().mTranslate.x = 0.815;
            end

            if objCount == 2 then
                 objectivebar:GetTransform().mTranslate.x = 0.7;
            end

            if objCount == 1 then
                objectivebar:GetTransform().mTranslate.x = 0.58;
            end
        end

    -- SPAWNING ENEMIES
    currentSpawnTimer = currentSpawnTimer + FPSManager.GetDT()

    if currentEnemyCount < maxEnemyCount and currentSpawnTimer > spawnTimer then

        mobtype = math.random(2, 3) -- generate a random number to spawn a random enemy between Trojan and Melissa only (for level 1)

            if (mobtype == 1) then systemManager.ecs:NewEntityFromPrefab("ILOVEYOU", mobSpawnPos1) 
                elseif (mobtype == 2) then systemManager.ecs:NewEntityFromPrefab("Melissa", mobSpawnPos2) 
                elseif (mobtype == 3) then systemManager.ecs:NewEntityFromPrefab("TrojanHorse", mobSpawnPos3) 
                elseif (mobtype == 4) then systemManager.ecs:NewEntityFromPrefab("ZipBomb", mobSpawnPos4)
            end
        currentEnemyCount = currentEnemyCount + 1
        currentSpawnTimer = 0 -- reset currentSpawnTimer so that next enemy can spawn
    end

    objectivebar:GetUIrenderer():SetSlider(progress/objectivesComplete);

    ent = Helper.GetScriptEntity(script_entity.id)
    transform = ent:GetTransform()

    -- print(math.random() +math.random(-20,20) )


    

    moveTime = moveTime + FPSManager.GetDT()

    reseed = reseed + FPSManager.GetDT()

    if(reseed >2)then

        math.randomseed(os.time())     
        reseed = 0
    end

    if(moveTime >0.4)then

        -- for i = 1, 2 , 1 
        -- do 

        -- print(math.random(-200,200))

            spawndataPos.x = transform.mTranslate.x  +math.random(-300,300)/100
            spawndataPos.y = transform.mTranslate.y 
            spawndataPos.z = transform.mTranslate.z +math.random(-300,300)/100

            bulletPrefab = systemManager.ecs:NewEntityFromPrefab("1s", spawndataPos)

            -- spawndataPos.x = transform.mTranslate.x  +math.random(-300,300)/100
            -- spawndataPos.y = transform.mTranslate.y 
            -- spawndataPos.z = transform.mTranslate.z +math.random(-300,300)/100

            -- bulletPrefab = systemManager.ecs:NewEntityFromPrefab("0s", spawndataPos)

            -- spawndataPos.x = transform.mTranslate.x  +math.random(-300,300)/100
            -- spawndataPos.y = transform.mTranslate.y 
            -- spawndataPos.z = transform.mTranslate.z +math.random(-300,300)/100

            -- bulletPrefab = systemManager.ecs:NewEntityFromPrefab("0s", spawndataPos)


        -- end
        moveTime = 0
    end


    if (isInZone == true) then

        if (progress < objectivesComplete) then

            --objectivebar:GetTransform().mTranslate.y = 0.7; -- Show the objective progress
            -- OBJECTIVE PROGRESS
            if (progress < objectivesComplete) then
                progress = progress + 1
                print("Current progress =", progress)
            end
        end

    end

    if (isInZone == false) then
    -- OBJECTIVE Progress (decreases if player is outside objective)

        --objectivebar:GetTransform().mTranslate.y = 20; -- Hide the objective progress

        if (progress < objectivesComplete) then

            if (progress > 0) then
                progress = progress - 1
                print("Current progress =", progress)
            end

        end
    end

    if (progress == objectivesComplete) then
        print("Objective complete!")
        --objectivebar:GetTransform().mTranslate.y = 20; -- Hide the objective progress
        entityobj = Helper.GetScriptEntity(script_entity.id)
        systemManager.ecs:SetDeleteEntity(entityobj)
        gameStateSys = systemManager:mGameStateSystem();
  
        testScriptEntity = gameStateSys:GetEntity("Controller")
        TestScripts = testScriptEntity:GetScripts()
        testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")

        if testScript ~= nil then
            testScript:RunFunction("RemoveObjective")
        end

        gameStateSys = systemManager:mGameStateSystem();
        testScriptEntity = gameStateSys:GetEntity("Controller")
        TestScripts = testScriptEntity:GetScripts()
        testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")

        if testScript ~= nil then
            objCount = testScript:ReturnValueInt("GetCountObj")
            if objCount == 0 then
                gameStateSys:ChangeGameState("WinMenu")
            end
        end

        --gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetPlay() -- TODO: Play audio
        -- TODO: Spawn another objective entity prefab at XYZ location in the map
        -- TODO: Spawn particles at local position for additional feedback
        -- TODO: Disable/hide/destroy this objective entity

    end

end

function Dead()
    
end

function OnTriggerEnter(Entity)
    local tagid = Entity:GetGeneral().tagid  
    print("tagid = ", tagid)
    if (tagid == 0) then -- if colliding with player
        isInZone = true
    end

    -- TODO: Turn all enemies hostiles
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity) 
    local tagid = Entity:GetGeneral().tagid      
    if (tagid == 0) then -- if colliding with player
        isInZone = false
    end

    -- TODO: Non hostile if not near
end