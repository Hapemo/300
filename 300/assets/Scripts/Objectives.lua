
local this

-- for objectives bar
local objectivesComplete = 1000 -- Changed from 1000 from 100 cause 100 filled up too fast
local progress
local isInZone = false

local currentSpawnTimer = 0 -- keeps track of how often enemy spawning interval
local spawnTimer = 3 -- sets how long the enemy spawning interval is
local currentEnemyCount = 0 -- keeps track of how many enemies there are in the map
local maxEnemyCount = 3 -- sets how many enemies are allowed in the map

local spawnPos1 = Vec3.new()
local spawnPos2 = Vec3.new()
local spawnPos3 = Vec3.new()
local spawnPos4 = Vec3.new()

local mobtype = 0;

function Alive()

    this = Helper.GetScriptEntity(script_entity.id)
    progress = 0
    print("Progress is", progress)

    spawnPos1.x = 27; -- find 4 good spawn points on the map and designate the 4 spawn spots below
    spawnPos1.y = -10;
    spawnPos1.z = 27;

    spawnPos2.x = 27;
    spawnPos2.y = -10;
    spawnPos2.z = 27;

    spawnPos3.x = 27;
    spawnPos3.y = -10;
    spawnPos3.z = 27;

    spawnPos4.x = 27;
    spawnPos4.y = -10;
    spawnPos4.z = 27;

end

function Update()

    if (isInZone == true) then
        -- OBJECTIVE PROGRESS
        if (progress < objectivesComplete) then

            -- TODO: set objective UI alpha to 255

            progress = progress + 1
            print("Current progress =", progress)

            if (progress == 100) then
            -- TODO: set objective UI first bar alpha to 255
            end

            if (progress == 200) then
            -- TODO: set objective UI second bar alpha to 255
            end

            if (progress == 300) then
            -- TODO: set objective UI third bar alpha to 255
            end

            if (progress == 400) then
            -- TODO: set objective UI fourth bar alpha to 255
            end

            if (progress == 500) then
            -- TODO: set objective UI fifth bar alpha to 255
            end

            if (progress == 600) then
            -- TODO: set objective UI sixth bar alpha to 255
            end

            if (progress == 700) then
            -- TODO: set objective UI seventh bar alpha to 255
            end

            if (progress == 800) then
            -- TODO: set objective UI eigth bar alpha to 255
            end

             if (progress == 900) then
            -- TODO: set objective UI ninth bar alpha to 255
            end

            if (progress == 1000) then
            -- TODO: set objective UI tenth bar alpha to 255
            end

            -- TODO: since objective progress decreases outside zone, need to set alpha back to 0 if progress falls below each threshold

        end

        -- SPAWNING ENEMIES
        currentSpawnTimer = currentSpawnTimer + FPSManager.GetDT()

        if currentEnemyCount < maxEnemyCount and currentSpawnTimer > spawnTimer then
            -- TODO: Select a XYZ point in the map to spawn at, current it spawns inside the objective

                mobtype = math.random(1, 4) -- generate a random number to spawn a random enemy

                if (mobtype == 1) then systemManager.ecs:NewEntityFromPrefab("ILOVEYOU", spawnPos1) 
                elseif (mobtype == 2) then systemManager.ecs:NewEntityFromPrefab("Melissa", spawnPos1) 
                elseif (mobtype == 3) then systemManager.ecs:NewEntityFromPrefab("TrojanHorse", spawnPos1) 
                elseif (mobtype == 4) then systemManager.ecs:NewEntityFromPrefab("ZipBomb", spawnPos1)
                end

                currentEnemyCount = currentEnemyCount + 1
                currentSpawnTimer = 0 -- reset currentSpawnTimer so that next enemy can spawn
        end

    end

    if (isInZone == false) then
    -- OBJECTIVE Progress (decreases if player is outside objective)

        -- TODO: set objective UI alpha to 0

        if (progress > 0) then
        progress = progress - 1
        print("Current progress =", progress)
        end
    end

    if (progress == objectivesComplete) then
        print("Objective complete!")

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