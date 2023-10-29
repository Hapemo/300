viewVec = vec3.new()
movement = Vec3.new()

function Alive()
    viewVec = Camera_Scripting.GetDirection(cameraEntity)
    entityobj = Helper.GetScriptEntity(script_entity.id)
    viewVec = Helper.Normalize(viewVec)
end

function Update()
    movement = movement+ viewVec*100
    physicsSys:SetVelocity(cameraEntity, movement)
    
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()
    tagid = generalComponent.tagid
    if (tagid == 4) then
        --floorCount = floorCount + 1;
        systemManager.ecs:DeleteEntity()
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end
