
local this

-- for objectives bar
local objectivesComplete = 100
local progress
local isInZone = false

local currentSpawnTimer = 0 -- keeps track of how often enemy spawning interval
local spawnTimer = 3 -- sets how long the enemy spawning interval is
local currentEnemyCount = 0 -- keeps track of how many enemies there are in the map
local maxEnemyCount = 3 -- sets how many enemies are allowed in the map

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    progress = 0
    -- print("Progress is", progress)
end

function Update()
    if (isInZone == true) then

        -- OBJECTIVE PROGRESS
        if (progress < objectivesComplete) then
            progress = progress + 1
            print("Current progress =", progress)
        end

        -- SPAWNING ENEMIES
        currentSpawnTimer = currentSpawnTimer + FPSManager.GetDT()
        -- TODO: Check if current enemyCount in map is more than maxEnemyCount before executing below

        if currentEnemyCount < maxEnemyCount then
            if currentSpawnTimer > spawnTimer then 
                -- TODO: Add RNG to select a random enemy prefab to spawn, also select a XYZ point in the map to spawn at
                systemManager.ecs:NewEntityFromPrefab("ILOVEYOU", this:GetTransform().mTranslate) -- spawn the enemy
                currentEnemyCount = currentEnemyCount + 1
                currentSpawnTimer = 0 -- reset currentSpawnTimer
            end
        end

        -- TODO: Show Objective UI
    end

    if (progress == objectivesComplete) then
        print("Objective complete!")
        gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetPlay() -- play some audio feedback TODO: Change Audio File
        -- TODO: Spawn another objective entity prefab at XYZ location in the map
        -- TODO: Spawn particles for additional feedback
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

    -- TODO: Non hostile if not near, progress depletes?
end
