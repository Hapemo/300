test = "HIIIII" 
local test4 = 6.0
vec = Vec3.new(3, 5, 7)
name = "Girl"
speed = 20

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
        transformComponent.mTranslate.y = transformComponent.mTranslate.y + speed
    elseif Input.CheckKey(State.HOLD, Key.LEFT) then
        transformComponent.mTranslate.x = transformComponent.mTranslate.x - speed
    elseif Input.CheckKey(State.HOLD, Key.DOWN) then
        transformComponent.mTranslate.y = transformComponent.mTranslate.y - speed
    elseif Input.CheckKey(State.HOLD, Key.RIGHT) then
        transformComponent.mTranslate.x = transformComponent.mTranslate.x + speed
    elseif Input.CheckKey(State.HOLD, Key.KEY_A) then
        -- print("Value: ", test)
        -- print("VEC X: ", vec.x)
        -- print("VEC Y: ", vec.y)
        -- print("VEC Z: ", vec.z)
        -- print("Name: ", name)
        --print("Value2: ", test2)
    end
end

function Dead()

end
