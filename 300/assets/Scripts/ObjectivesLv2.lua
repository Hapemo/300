
local this

local const flt_epsilon = 1e-5

-- for objectives bar
local objectivesComplete = 500
local progress
local isInZone = false

local currentSpawnTimer = 0 -- keeps track of how often enemy spawning interval
local spawnTimer = 7.5       -- sets how long the enemy spawning interval is
local currentEnemyCount = 0 -- keeps track of how many enemies there are in the map
local maxEnemyCount = 15    -- sets how many enemies are allowed in the map

local mobSpawnPos1 = Vec3.new()
local mobSpawnPos2 = Vec3.new()
local mobSpawnPos3 = Vec3.new()
local mobSpawnPos4 = Vec3.new()
local mobspawnpoint = Vec3.new()

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

    mobSpawnPos1.x = 21;
    mobSpawnPos1.y = -4;
    mobSpawnPos1.z = 24;

    mobSpawnPos2.x = -20;
    mobSpawnPos2.y = -4;
    mobSpawnPos2.z = 36;

    mobSpawnPos3.x = -7;
    mobSpawnPos3.y = 1.5;
    mobSpawnPos3.z = 50;

    mobSpawnPos4.x = -1;
    mobSpawnPos4.y = -3.4;
    mobSpawnPos4.z = 50;
end

function Update()
    gameStateSys = systemManager:mGameStateSystem()
    inputMapSys = systemManager:mInputActionSystem()
    if(inputMapSys:GetButtonDown("NextLevel")) then
        x = gameStateSys:GetEntity("TransitionHelper", "Transition") 
        y = x:GetScripts()
        z = y:GetScript("../assets/Scripts/Transition.lua")

        if z ~= nil then
            z:SetNextGameStateHelper("SetNextGameState", "Test3")
            --z:RunFunctionWithParam("SetNextGameState", "Test2")
            z:RunFunction("StartTransition")
        end
    end

    if isInit == false then
        gameStateSys = systemManager:mGameStateSystem();
        testScriptEntity = gameStateSys:GetEntity("Controller")
        TestScripts = testScriptEntity:GetScripts()
        testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")

        if testScript ~= nil then
            --testScript:RunFunction("AddObjective")
            testScript:RunFunction("InitializeObjectiveCount") -- set the starting number
        end

        objectiveBarSpawnPos.x = 0.8;
        objectiveBarSpawnPos.y = 0.7;
        objectiveBarSpawnPos.z = 0;
        objectivebar = systemManager.ecs:NewEntityFromPrefab("Objective Bar 1", objectiveBarSpawnPos)
        -- objectivebar = gameStateSys:GetEntityByScene("Objective Bar 1","Objectives")
        isInit = true
    end

        gameStateSys = systemManager:mGameStateSystem();
        testScriptEntity = gameStateSys:GetEntity("Controller")
        TestScripts = testScriptEntity:GetScripts()
        testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")

        if testScript ~= nil then
            objCount = testScript:ReturnValueInt("GetCountObj")

            if objCount == 3 then
                objectivebar:GetTransform().mTranslate.x =  0.58;
            end

            if objCount == 2 then
                 objectivebar:GetTransform().mTranslate.x = 0.7;
            end

            if objCount == 1 then
                objectivebar:GetTransform().mTranslate.x = 0.815;
            end
        end

        if (inputMapSys:GetButtonDown("nine")) then
            _G.completedEpicM = true 
            _G.completedEpicZB = true 
            print("EpicM and EpicZB completed")
        end

        if(_G.completedEpicM == true and _G.completedEpicZB == true) then
            -- SPAWNING ENEMIES
            currentSpawnTimer = currentSpawnTimer + FPSManager.GetDT()

            if currentEnemyCount < maxEnemyCount and currentSpawnTimer > spawnTimer then
                mobtype = math.random(1, 4) -- generate a random number to spawn a random enemy between Trojan and Melissa only (for level 1)
                local mobspawnpoint_rand = math.random(1, 4)

                    if(mobspawnpoint_rand == 1) then
                        mobspawnpoint = mobSpawnPos1
                    elseif(mobspawnpoint_rand == 2) then
                        mobspawnpoint = mobSpawnPos2
                    elseif(mobspawnpoint_rand == 3) then
                        mobspawnpoint = mobSpawnPos3
                    elseif(mobspawnpoint_rand == 4) then
                        mobspawnpoint = mobSpawnPos4
                    end

                    if (mobtype == 1) then 
                        systemManager.ecs:NewEntityFromPrefab("ILOVEYOU2", mobspawnpoint) 
                    elseif (mobtype == 2) then 
                        systemManager.ecs:NewEntityFromPrefab("Melissa2", mobspawnpoint) 
                    elseif (mobtype == 3) then 
                        systemManager.ecs:NewEntityFromPrefab("TrojanHorse", mobspawnpoint) 
                    elseif (mobtype == 4) then 
                        systemManager.ecs:NewEntityFromPrefab("ZipBomb2", mobspawnpoint)
                    end
                currentEnemyCount = currentEnemyCount + 1
                currentSpawnTimer = 0 -- reset currentSpawnTimer so that next enemy can spawn
            end
        end

    --print("Current progress =", progress/objectivesComplete)
    -- print("SPAWNING")
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


    if(moveTime > 0.4)then
        -- only appear when the platform is raised
        if( math.abs(transform.mTranslate.y - (-4.7)) <= flt_epsilon
            or math.abs(transform.mTranslate.y - 1) <= flt_epsilon 
            or math.abs(transform.mTranslate.y - (-10)) <= flt_epsilon) then
            -- print("Spawning 1's and 0's")
            spawndataPos.x = transform.mTranslate.x  +math.random(-300,300)/100
            spawndataPos.y = transform.mTranslate.y 
            spawndataPos.z = transform.mTranslate.z +math.random(-300,300)/100

            bulletPrefab = systemManager.ecs:NewEntityFromPrefab("1s", spawndataPos)

            spawndataPos.x = transform.mTranslate.x  +math.random(-300,300)/100
            spawndataPos.y = transform.mTranslate.y 
            spawndataPos.z = transform.mTranslate.z +math.random(-300,300)/100

            bulletPrefab = systemManager.ecs:NewEntityFromPrefab("0s", spawndataPos)
        end
        moveTime = 0
    end

    if (isInZone == true) then

        if (progress < objectivesComplete) then

            --objectivebar:GetTransform().mTranslate.y = 0.7; -- Show the objective progress
            -- OBJECTIVE PROGRESS
            if (progress < objectivesComplete) then
                progress = progress + 1
               -- print("Current progress =", progress/objectivesComplete)
            end
        end

    end

    if (isInZone == false) then
    -- OBJECTIVE Progress (decreases if player is outside objective)
        --objectivebar:GetTransform().mTranslate.y = 20; -- Hide the objective progress

        if (progress < objectivesComplete) then

            if (progress > 0) then
                progress = progress - 1
               -- print("Current progress =", progress)
            end

        end
    end

    if (progress == objectivesComplete) then
        --objectivebar:GetTransform().mTranslate.y = 20; -- Hide the objective progress
        entityobj = Helper.GetScriptEntity(script_entity.id)

        if entityobj:GetGeneral().name == "Objectives1" then
            controllerL2 = gameStateSys:GetEntity("DialogueControllerLevel2")
            controllerL2Scripts = controllerL2:GetScripts()
            controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/DialogueControllerLevel2.lua")

            if controllerL2Script ~= nil then
                controllerL2Script:RunFunction("FinishObjective1")
            end
        end
        if entityobj:GetGeneral().name == "Objectives2" then
            controllerL2 = gameStateSys:GetEntity("DialogueControllerLevel2")
            controllerL2Scripts = controllerL2:GetScripts()
            controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/DialogueControllerLevel2.lua")

            if controllerL2Script ~= nil then
                controllerL2Script:RunFunction("FinishObjective2")
            end
        end
        if entityobj:GetGeneral().name == "Objectives3" then
            controllerL2 = gameStateSys:GetEntity("DialogueControllerLevel2")
            controllerL2Scripts = controllerL2:GetScripts()
            controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/DialogueControllerLevel2.lua")

            if controllerL2Script ~= nil then
                controllerL2Script:RunFunction("FinishObjective3")
            end
        end

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