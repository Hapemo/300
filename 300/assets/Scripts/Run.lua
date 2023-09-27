function Alive()

end

function Update()
    --For M1 demo
    entity = Entity.new(script_entity_id)
    if entity == nil then
        print("Entity nil in script!")
    end
    generalComponent = entity:GetGeneralComponent()
    transformComponent = entity:GetTransformComponent()
    -- Change entity name to enemy
    generalComponent.name = "Enemy"

    -- Make character move x-axis
    if Input.CheckKey(State.HOLD, Key.UP) then
        transformComponent.mTranslate.y = transformComponent.mTranslate.y + 20
    elseif Input.CheckKey(State.HOLD, Key.LEFT) then
        transformComponent.mTranslate.x = transformComponent.mTranslate.x - 20
    elseif Input.CheckKey(State.HOLD, Key.DOWN) then
        transformComponent.mTranslate.y = transformComponent.mTranslate.y - 20
    elseif Input.CheckKey(State.HOLD, Key.RIGHT) then
        transformComponent.mTranslate.x = transformComponent.mTranslate.x + 20
    end
end

function Dead()

end
