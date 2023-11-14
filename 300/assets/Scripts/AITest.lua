local vec = Vec3.new()
local firstvec = Vec3.new()
local secondvec = Vec3.new()
local originalScaleX

local deathEntity
local deathComp
local deathAudioSource
local aiSys
local phySys
local aiComp
local entity
local entity2

-- for example you want to reference out hp variable to another script
--local hp = 100

-- function GetHP()
--     return hp
-- end

function Alive()
    gameStateSys = systemManager:mGameStateSystem()

    deathEntity = gameStateSys:GetEntity("Death" , "testSerialization")
    deathComp   = deathEntity:GetAudio()
    deathAudioSource = Helper.CreateAudioSource(deathEntity)

    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();
    aiComp = entity:GetAISetting();
end

function Update()

    -- Get AISetting function
    --entity2 = aiComp:GetTargetConst();
    --aiComp:SetTarget(entity);
    --print (entity2.id)

    -- Create new Entity in current scene example
    --testEntity = systemManager.ecs:NewEntityByScene();

    -- Get Entity exisiting in scene by name example
    --gameStateSys = systemManager:mGameStateSystem();
    --floorEntity = gameStateSys:GetEntity("Floor", "Test1")

    --AI TEST--

    -- Must call this before calling AISystem functions
    
    -- vec = Vec3.new(50,0,50)
    vec = aiSys:GetDirection(entity)
    entity:GetTransform().mRotate.y = Helper.DirectionToAngle(vec)

    vec.x = vec.x * 20;
    vec.y = entity:GetRigidBody().mVelocity.y;
    vec.z = vec.z * 20;
    phySys:SetVelocity(entity, vec);
    
    if (entity:GetTransform().mScale.x < 2.0) then
        deathAudioSource:Play()
        deathAudioSource:SetVolume(0.2)
        systemManager.ecs:SetDeleteEntity(entity)
    end
        
    -- AI functions
    -- aiSys:SetPredictiveVelocity(entity, entity, 0.5)
    -- aiSys:PredictiveShootPlayer(entity, 0.5, 2, 4)

    --print("Jiayou Jazzi")
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end
