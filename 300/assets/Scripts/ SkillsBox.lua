
local entityobj

function Alive()

end

function Update()
    
end

function Dead()

end

function OnTriggerEnter(Entity)
    -- print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
    entityobj = Helper.GetScriptEntity(script_entity.id)
    generalComponent = Entity:GetGeneral()

    if (generalComponent.name == "Camera")then

        _G.collided =true
        systemManager.ecs:SetDeleteEntity(entityobj)
    end


end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


