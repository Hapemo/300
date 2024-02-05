function Alive()

end

function Update()
    
end

function Dead()

end

function OnTriggerEnter(Entity)
    print("ontriggerenter")
    gameStateSys = systemManager:mGameStateSystem()
    gameStateSys:ChangeGameState("Test")
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


