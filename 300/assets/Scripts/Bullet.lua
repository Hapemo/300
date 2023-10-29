viewVec = Vec3.new(0,0,0)
movement = Vec3.new(0,100,0)
multiplier = Vec3.new(0.4,0.4,0.4)
function Alive()


end

function Update()

    gameStateSys = systemManager:mGameStateSystem()
    cameraEntity = gameStateSys:GetEntity("Camera", "testSerialization")
    positions = cameraEntity:GetTransform().mTranslate


    viewVec = Camera_Scripting.GetDirection(cameraEntity)
    entityobj = Helper.GetScriptEntity(script_entity.id)
    viewVec = Helper.Normalize(viewVec)
    physicsSys = systemManager:mPhysicsSystem()


    -- movement.x = movement.x+ viewVec.x     
    -- movement.y = movement.y+ viewVec.y    
    -- movement.z = movement.z+ viewVec.z     
    --physicsSys:SetVelocity(entityobj, movement)
    
end

function Dead()

end

function OnTriggerEnter(Entity)
    -- generalComponent = Entity:GetGeneral()
    -- tagid = generalComponent.tagid
    -- if (tagid == 4) then
    --     --floorCount = floorCount + 1;
    --     systemManager.ecs:DeleteEntity()
    -- end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end
