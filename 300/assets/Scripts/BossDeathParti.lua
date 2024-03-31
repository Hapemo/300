viewVec = Vec3.new(0,0,0)
movement = Vec3.new(0,0,0)
_G.bossDeath = false


timer = 0
direction = Vec3.new(10,0,0)
colors = Vec4.new(0,0,0,1)
positions = Vec3.new(0,0,0)


color1 = Vec4.new(1,0,0,1)
color2 = Vec4.new(0,1,0,1)
color3 = Vec4.new(0,0,1,1)
color4 = Vec4.new(0,1,1,1)
color5 = Vec4.new(1,0,1,1)
color6 = Vec4.new(1,1,0,1)
color7 = Vec4.new(1,1,1,1)

allcolor = {Vec4.new(1,0,0,1),Vec4.new(0,1,0,1),Vec4.new(0,0,1,1),Vec4.new(0,1,1,1),Vec4.new(1,0,1,1),Vec4.new(1,1,0,1),Vec4.new(1,1,1,1)}


local prefabEntity;
local meshSys;
local entityobj


function Alive()

end

function Update()

    if _G.bossDeath == true then
        entityobj = Helper.GetScriptEntity(script_entity.id)

        for i = 20, 1, -1
        do
            spawned(1)
        end

        systemManager.ecs:SetDeleteEntity(entityobj)
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end


function spawned(value)
    positions = entityobj:GetTransform().mTranslate

    prefabEntity = systemManager.ecs:NewEntityFromPrefab("parti2",positions )   

    meshSys = prefabEntity:GetMeshRenderer()
    meshSys:SetColor(allcolor[value])
end