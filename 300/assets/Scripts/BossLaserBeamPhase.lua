local speed
local original_rotation
local new_rotation

function Alive()

    -- testing timer countdown for the rotation of the laser
    ent = Helper.GetScriptEntity(script_entity.id)
    trans = ent:GetTransform()
    speed = 50
    original_rotation = trans.mRotate.y
end

function Update()

    ent = Helper.GetScriptEntity(script_entity.id)
    trans = ent:GetTransform()

    new_rotation = Vec3.new()

    if(trans.mRotate.y < (original_rotation + 360)) then
        print("trans is: ", trans.mRotate.y)
        print("cap is: ", original_rotation + 360)
        new_rotation.x = trans.mRotate.x
        new_rotation.y = trans.mRotate.y + (FPSManager.GetDT() * speed)
        new_rotation.z = trans.mRotate.z
    elseif (trans.mRotate.y >= (original_rotation + 360)) then
        new_rotation.x = trans.mRotate.x
        new_rotation.y = original_rotation
        new_rotation.z = trnas.mRotate.z
    end
    Helper.SetRealRotate(ent, new_rotation)
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


