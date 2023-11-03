function Alive()

end

function Update()
    -- Must call this before calling AISystem functions
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();
    inputSys = systemManager:mInputActionSystem();

    -- Get entity attached to script
    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end

    -- Create new Entity in current scene example
    --testEntity = systemManager.ecs:NewEntityByScene();

    -- Get Entity exisiting in scene by name example
    --floorEntity = gameStateSys:GetEntity("Floor", "Test1")

    --AI TEST--

    -- vec = Vec3.new(50,0,50)
    vec = Vec3.new(0,0,0)

    if inputSys:GetButton("up") then 
        vec.x = vec.x - 60
    end
    if inputSys:GetButton("down") then 
        vec.x = vec.x + 60
    end
    if inputSys:GetButton("left") then 
        vec.z = vec.z + 60
    end
    if inputSys:GetButton("right") then 
        vec.z = vec.z - 60
    end

    if inputSys:GetButtonDown("Jump") then
        vec.y = vec.y + 200
    end
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
