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
    -- Change entity name to enemy
    generalComponent.name = "Enemy"

    -- Make character move x-axis
    if Input.CheckKey(State.PRESS, Key.KEY_SPACE) then
        transformComponent.mTranslate.x = transformComponent.mTranslate.x + 30
    end

    -- for i = 0, #generalEntities do
    --     print(generalEntities[i].id, generalEntities[i])
    -- end
end

function Exit()

end

function Dead()

end
