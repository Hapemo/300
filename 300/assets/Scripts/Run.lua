function Alive()

end

function Start()

end

function Update()
    --Working
    --systemManager.ecs:DeleteEntity(entity)
    
    --Testing

    entity = Entity.new(script_entity_id)
    if entity == nil then
        print("NULL!!!!")
    end
    generalComponent = entity:GetGeneralComponent()
    transformComponent = entity:GetTransformComponent()
    generalComponent.name = "Enemy"

    if Input.CheckKey(State.PRESS, Key.KEY_SPACE) then
        transformComponent.mScale.x = transformComponent.mScale.x + 50
    end

    -- for i = 0, #generalEntities do
    --     print(generalEntities[i].id, generalEntities[i])
    -- end
end

function Exit()

end

function Dead()

end
