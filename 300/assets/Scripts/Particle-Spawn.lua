spawn = true
timer = 0
direction = Vec3.new(10,0,0)
colors = Vec4.new(0,0,0,1)

color1 = Vec4.new(1,0,0,1)
color2 = Vec4.new(0,1,0,1)
color3 = Vec4.new(0,0,1,1)
color4 = Vec4.new(0,1,1,1)
color5 = Vec4.new(1,0,1,1)
color6 = Vec4.new(1,1,0,1)
color7 = Vec4.new(1,1,1,1)

allcolor = {Vec4.new(1,0,0,1),Vec4.new(0,1,0,1),Vec4.new(0,0,1,1),Vec4.new(0,1,1,1),Vec4.new(1,0,1,1),Vec4.new(1,1,0,1),Vec4.new(1,1,1,1)}


function Alive()

end

function Update()

    entity = Helper.GetScriptEntity(script_entity.id)
    physicsSys = systemManager:mPhysicsSystem()
    transform = entity:GetTransform()

    timer = timer +  FPSManager.GetDT()
    
   
    
    if ( timer >1)then   
        for i = 7, 1, -1
        do
            spawned(i)
        end
        timer = 0
    end

end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function spawned(value)

    direction.x = math.random(-10,10)
    direction.y = math.random(-10,10)
    direction.z = math.random(-10,10)
    
    total = (direction.x +direction.y +direction.z)
    -- colors.x = direction.x/5
    -- colors.y = direction.y/5
    -- colors.z = direction.z/5
    direction.x =( direction.x/total)*5
    direction.y = (direction.y/total)*5
    direction.z = (direction.z/total)*5
    positions = entity:GetTransform().mTranslate

    prefabEntity = systemManager.ecs:NewEntityFromPrefab("parti",positions )   

    meshSys = prefabEntity:GetMeshRenderer()
    meshSys:SetColor(allcolor[value])
    physicsSys:SetVelocity(prefabEntity, direction)
end