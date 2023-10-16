test = "HIIIII" 
local test4 = 6.0
vec = Vec3.new(3, 5, 7)
name = "Girl"
speed = 20
create = 2
testing = 67

function Alive()

end

function Update()

    --testie = systemManager.ecs:NewEntity();
    --For M1 demo
    entity = Entity.new(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end
    generalComponent = entity:GetGeneralComponent()
    transformComponent = entity:GetTransformComponent()
    -- Change entity name to enemy
    generalComponent.name = "Enemy"
    --[[ Get entity Tag
    tag = generalComponent:GetTag();
    print(tag) 
    --]]
    
    -- if(0) then
    --     print("wow")
    -- end

    -- if(" ") then
    --     print("can meh")
    -- end

    -- Make character move x-axis
    if (Input.CheckKey(State.HOLD, Key.UP)) then
        transformComponent.mTranslate.y = transformComponent.mTranslate.y + speed
    elseif (Input.CheckKey(State.HOLD, Key.LEFT)) then
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
        
        -- Set entity Tag
        generalComponent:SetTag("eneMY");
        tag = generalComponent:GetTag();
        print("Tag is: "..tag)
        if (create == 2) then
            for i = 1, 4 do
                testEntity = systemManager.ecs:NewEntityByScene();
            end
            create = create + 1
            print("came in here!!!")
        end

        -- Physics Set Velocity function
        -- physicsSys = systemManager:mPhysicsSystem();
        -- physicsSys:SetVelocity(entity, vec);
    elseif Input.CheckKey(State.HOLD, Key.KEY_B) then
        --testEntity = systemManager.ecs:NewEntityByScene();
        --scriptingSys = systemManager:mScriptingSystem();
        --scriptingSys:AddScript(testEntity, "../assets\\Scripts\\Run.lua");
        Helper.printTest()
        --Helper.test = 300
        testing = testing + 1
        --script_entity.id = 60
    end

    -- --initialize array
    -- array = {}
    -- --its able to store any type in the same array (give it a key, then value)
    -- array["first"] = 11 
    -- array[20] = "woww"
    -- --loop the whole array and print whichever info u want, key/value or both
    -- for key,value in pairs(array) do
    --     print(key)
    -- end
    
    -- print("Start")
    -- array = {2, "cat", "pig", false}
    -- for i = 1, 4 do
    --     print("yes")
    -- end
    -- print("End")
end

function Dead()

end
