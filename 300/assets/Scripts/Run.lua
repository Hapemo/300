test = "HIIIII" 
local test4 = 6.0
vec = Vec3.new(0.3, 0.5, 0.7)
vec4 = Vec4.new(6,7,8,9)
name = "Girl"
speed = 20
create = 2
testing = 5

function Alive()

end

function Update()
    print(name)
    testing = testing + 1;
    --testie = systemManager.ecs:NewEntity();
    --For M1 demo
    --entity = Entity.new(script_entity.id)
    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end
    generalComponent = entity:GetGeneral()
    transformComponent = entity:GetTransform()
    --vfxComponent = entity:GetVFX();
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
        end

        --Test VFX Component
        --vfxComponent:EnableObjectBloom();
        --vfxComponent:SetEntityBloomThreshold(vec);

        -- Physics Set Velocity function
        -- physicsSys = systemManager:mPhysicsSystem();
        -- physicsSys:SetVelocity(entity, vec);
    elseif Input.CheckKey(State.HOLD, Key.KEY_B) then
        testEntity = systemManager.ecs:NewEntityByScene();
        -- scriptingSys = systemManager:mScriptingSystem();
        -- scriptingSys:AddScript(testEntity, "../assets\\Scripts\\Print.lua");
        rigidBodyComponent = testEntity:AddRigidBody();
        pointLightComponent = testEntity:AddPointLight();
        meshComponent = testEntity:AddMeshRenderer();
        pointLightComponent:SetColor(vec);
        meshComponent:SetColor(vec4);
        meshComponent:SetMesh("cube");
        meshComponent:SetTexture(MaterialType.NORMAL, "Wood Material 15_Normal");
        --Helper.printTest()
        --Helper.test = 300
        --script_entity.id = 60
        generalComponent:SetTag("static");
        tag = generalComponent:GetTag();
    end

    --Test Default Param
    --Testing()

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

    -- Test animation
    -- animationComponent = entity:GetAnimator()
    -- animationComponent:PauseAnimation()

    -- Test Bloom
    --graphicsSys = systemManager:mGraphicsSystem();
    --graphicsSys:SetGlobalBloomThreshold(vec)
    --graphicsSys:SetGlobalBloomExposure(0.5)
end

function Dead()

end

function Testing(a,b,c)
    a = a or 6
    b = b or 8
    c = c or "cat"

    print(a)
    print(b)
    print(c)
end

function OnTriggerEnter(Entity)
    print("From CollideTest.lua OnTriggerEnter")
end

function OnTriggerExit(Entity)
    print("OnTriggerExit")
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end
