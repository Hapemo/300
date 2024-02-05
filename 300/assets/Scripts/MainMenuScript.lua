


function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    graphicssys = systemManager:mGraphicsSystem()

    graphicssys:HideCursor(false)
    
end

function Update()
    -- Get entity attached to script
    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end

    gameStateSys = systemManager:mGameStateSystem();
    inputSys = systemManager:mInputActionSystem();

    if (inputSys:GetKeyDown(257)) then -- Press ENTER
        gameStateSys:ChangeGameState("Test")

    end
end

function Dead()
    graphicssys:HideCursor(true)
end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end
