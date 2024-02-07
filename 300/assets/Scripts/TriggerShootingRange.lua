
local this

function Alive()
    -- Get Trigger Box (This Entity - holding this script)
    this = Helper.GetScriptEntity(script_entity.id)
    
    print("TRIGGER SHOOTING RANGE INIT")
end

function Update()
    
end

function Dead()

end

function OnTriggerEnter(Entity)

    print("PLAYER ENTERED THE ZONE") 
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
    if(tagid == 0) then 
        print("PLAYER ENTERED THE ZONE") 
    end
end

function OnContactExit(Entity)

end


