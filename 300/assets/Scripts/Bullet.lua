viewVec = Vec3.new(0,0,0)
movement = Vec3.new(0,0,0)

function Alive()


end

function Update()

    gameStateSys = systemManager:mGameStateSystem()
    cameraEntity = gameStateSys:GetEntity("Camera", "testSerialization")
    positions = cameraEntity:GetTransform().mTranslate


    viewVec = Camera_Scripting.GetDirection(cameraEntity)
    entityobj = Helper.GetScriptEntity(script_entity.id)
    physicsSys = systemManager:mPhysicsSystem()

    -- movement.x= movement.x+movement.x
    --   movement.x = movement.x+ viewVec.x *0     
    --   movement.y = movement.y+ viewVec.y *0   
    --   movement.z = movement.z+ viewVec.z *0

    --   print(viewVec.x)
    --   print(viewVec.y)
    --   print(viewVec.z)
      
   
    
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()
    tagid = generalComponent.tagid
    if (tagid == 3) then
        --floorCount = floorCount + 1;
        -- gameStateSys = systemManager:mGameStateSystem();
        -- gameStateSys:DeleteEntity(entityobj)
        systemManager.ecs:DeleteEntity(entityobj)
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end
