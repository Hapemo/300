function Alive()

end

function Update()
        pauseMenuUI = Helper.GetScriptEntity(script_entity.id)
    pauseMenuUI:GetTransform().mTranslate.x = 1000;
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

function TriggerPause()
    pauseMenuUI = Helper.GetScriptEntity(script_entity.id)
    pauseMenuUI:GetTransform().mTranslate.x = 0;
print("came")
end


