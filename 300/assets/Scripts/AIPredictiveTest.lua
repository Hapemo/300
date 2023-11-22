function Alive()

end

function Update()
    -- Must call this before calling AISystem functions
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();
    inputSys = systemManager:mInputActionSystem();
    gameStateSys = systemManager:mGameStateSystem();

    -- Get entity attached to script
    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end


    -- Create new Entity in current scene example
    --testEntity = systemManager.ecs:NewEntityByScene();

    -- Get Entity exisiting in scene by name example
    --floorEntity = gameStateSys:GetEntity("Floor")

    --AI TEST--
    if inputSys:GetKeyDown(79) then -- press O
        player = gameStateSys:GetEntity("Player")
        
        aiSys:PredictiveShootPlayer(entity, 80.0, 10, 0)
        -- aiSys:SetPredictiveVelocity(entity, player, 80.0)
    end

    if inputSys:GetKeyDown(80) then -- press P
        vec = Vec3.new(0,0,0)
        phySys:SetVelocity(entity, vec);
    end

    -- vec = Vec3.new(50,0,50)

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
