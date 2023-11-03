local fin

function Alive()
    fin = Helper.GetScriptEntity(script_entity.id)
end

function Update()
    fin:GetTransform().mRotate.x = fin:GetTransform().mRotate.x + 40.0 * FPSManager:GetDT()
    fin:GetTransform().mRotate.y = fin:GetTransform().mRotate.y + 50.0 * FPSManager:GetDT()
    fin:GetTransform().mRotate.z = fin:GetTransform().mRotate.z + 60.0 * FPSManager:GetDT()

end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end
