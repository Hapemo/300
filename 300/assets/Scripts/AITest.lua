local vec = Vec3.new()
local firstvec = Vec3.new()
local secondvec = Vec3.new()
local originalScaleX

local deathEntity
local deathComp
local deathAudioSource

function Alive()
    gameStateSys = systemManager:mGameStateSystem()

    deathEntity = gameStateSys:GetEntity("Death" , "testSerialization")
    deathComp   = deathEntity:GetAudio()
    deathAudioSource = Helper.CreateAudioSource(deathEntity)

    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end
    originalScaleX = entity:GetTransform().mScale.x
end

function Update()
    -- Get entity attached to script


    -- Create new Entity in current scene example
    --testEntity = systemManager.ecs:NewEntityByScene();

    -- Get Entity exisiting in scene by name example
    --gameStateSys = systemManager:mGameStateSystem();
    --floorEntity = gameStateSys:GetEntity("Floor", "Test1")

    --AI TEST--

    -- Must call this before calling AISystem functions
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();
    -- vec = Vec3.new(50,0,50)
    vec = aiSys:GetDirection(entity)

    firstvec.x = 0.0
    firstvec.y = 0.0
    firstvec.z = 1.0
    secondvec.x = vec.x
    secondvec.y = 0
    secondvec.z = vec.z

    dotDivideMag = vec.z / math.sqrt(vec.x * vec.x + vec.z * vec.z)
    radians = math.acos(dotDivideMag)
    degree = radians * 180.0 / 3.141596
    if (vec.x < 0) then
        degree = 360.0 - degree
    end
    entity:GetTransform().mRotate.y = degree

    vec.x = vec.x * 20;
    vec.y = entity:GetRigidBody().mVelocity.y;
    vec.z = vec.z * 20;
    phySys:SetVelocity(entity, vec);
    
    if (entity:GetTransform().mScale.x < originalScaleX / 2.0) then
        deathAudioSource.Play()
        deathAudioSource.SetVolume(0.2)
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
