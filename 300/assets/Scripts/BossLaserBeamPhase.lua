local speed
local angle = 0.0
local axis = Vec3.new()

function Alive()

    -- testing timer countdown for the rotation of the laser
    ent = Helper.GetScriptEntity(script_entity.id)
    trans = ent:GetTransform()
    speed = 7
    if(ent:GetGeneral().name == "Laser1") then
        axis.x = 0.0
        axis.y = 1.0
        axis.z = 0.0
        angle = 0.0
    elseif (ent:GetGeneral().name == "Laser2") then
        axis.x = 0.0
        axis.y = 1.0
        axis.z = 0.0
        angle = 90.0
    end
end

function Update()

    angle = angle + speed * (FPSManager.GetDT() * speed)
    Helper.SetRealRotateQuaternion(ent, axis, angle)
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()

    if(generalComponent.name == "Camera") then
        if(Entity:HasHealthbar()) then 
            healthComponent = Entity:GetHealthbar()
            healthComponent.health = healthComponent.health - 6
        end
    end
end

function OnTriggerExit(Entity)
    generalComponent = Entity:GetGeneral()
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


