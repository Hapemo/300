local this
local currTime
function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    currTime = 0
end

function Update()
    currTime = currTime + 1
    if (currTime > 180) then
        gameStateSys:ChangeGameState("MainMenu")
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


