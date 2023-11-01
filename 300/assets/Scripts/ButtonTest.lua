function Alive()

end

function Update()
    entityobj = Helper.GetScriptEntity(script_entity.id)
    buttonComponent = entityobj:GetButton();
    if (buttonComponent:IsClicked())
        print("Clicked!")
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end
