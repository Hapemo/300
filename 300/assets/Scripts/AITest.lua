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

    -- AI functions
    aiSys:SetPredictiveVelocity(entity, entity, 0.5)
    aiSys:PredictiveShootPlayer(entity, 0.5, 2, 4)
    vec = aiSys:GetDirection(entity)

    --print("Jiayou Jazzi")
end

function Dead()

end

function OnTriggerEnter(Entity)

end

function OnTriggerExit(Entity)

end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end
