function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in script!")
    end
end

function Update()
    if this:GetHealthbar().health <= 0 then
        --increase count of dead enemy
        _G.PreObjectivesCounter = _G.PreObjectivesCounter + 1;
        --transformed away fr now lol
        this:GetTransform().mTranslate.x = 1000;
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


