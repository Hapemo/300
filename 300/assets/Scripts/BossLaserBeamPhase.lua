local speed
local original_rotation

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

    print("rotation of laser beam: ", trans.mRotate.y)
    if(trans.mRotate.y < (original_rotation + 360)) then
        trans.mRotate.y = trans.mRotate.y + (FPSManager.GetDT() * speed)
    elseif (trans.mRotate.y >= (original_rotation + 360)) then
        trans.mRotate.y = original_rotation
    end
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


