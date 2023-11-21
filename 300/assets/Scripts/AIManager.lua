local spawnPoint1
local spawnPoint2
local spawnPoint3
local spawnPoint4
local spawntimer
local where
function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    spawntimer = 0
    spawnPoint1 = gameStateSys:GetEntity("AISpawnPoint1")
    spawnPoint2 = gameStateSys:GetEntity("AISpawnPoint2")
    spawnPoint3 = gameStateSys:GetEntity("AISpawnPoint3")
    spawnPoint4 = gameStateSys:GetEntity("AISpawnPoint4")
    where = false
end

function Update()
    spawntimer = spawntimer + FPSManager.GetDT()
    if (spawntimer > 30.0) then
        if (where == true) then
            systemManager.ecs:NewEntityFromPrefab("enemy1_walking", spawnPoint1:GetTransform().mTranslate)
            systemManager.ecs:NewEntityFromPrefab("enemy1_walking", spawnPoint2:GetTransform().mTranslate)
            spawntimer = 0
            where = false
        else
            systemManager.ecs:NewEntityFromPrefab("enemy1_walking", spawnPoint3:GetTransform().mTranslate)
            systemManager.ecs:NewEntityFromPrefab("enemy1_walking", spawnPoint4:GetTransform().mTranslate)
            where = true
            spawntimer = 0
        end
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end
