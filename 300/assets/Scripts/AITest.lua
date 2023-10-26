function Alive()

end

function Update()
    -- Get entity attached to script
    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end

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
    vec.x = vec.x * 20;
    vec.y = vec.y * 20;
    vec.z = vec.z * 20;
    phySys:SetVelocity(entity, vec);

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
