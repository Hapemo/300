local speed
local angle = 0.0
local axis = Vec3.new()

function Alive()

    -- testing timer countdown for the rotation of the laser
    ent = Helper.GetScriptEntity(script_entity.id)
    trans = ent:GetTransform()
    axis.x = 0.0
    axis.y = 1.0
    axis.z = 0.0
    speed = 5
    angle = 0.0
end

function Update()

    angle = angle + speed * (FPSManager.GetDT() * speed)
    Helper.SetRealRotateQuaternion(ent, axis, angle)
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()
    print("enter", generalComponent.name)
end

function OnTriggerExit(Entity)
    generalComponent = Entity:GetGeneral()
    print("exit", generalComponent.name)
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


