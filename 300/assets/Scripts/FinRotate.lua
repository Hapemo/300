local fin

function Alive()
    fin = Helper.GetScriptEntity(script_entity.id)
end

function Update()
    fin:GetTransform().mRotate.x = fin:GetTransform().mRotate.x + 2.0 * FPSManager:GetDT()
    fin:GetTransform().mRotate.y = fin:GetTransform().mRotate.y + 2.0 * FPSManager:GetDT()

end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end
