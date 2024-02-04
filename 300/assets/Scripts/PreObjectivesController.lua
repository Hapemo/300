_G.PreObjectivesCounter = 0;
local objective1
local objective2
local objective3
function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    objective1 = gameStateSys:GetEntity("Objectives1")
    objective2 = gameStateSys:GetEntity("Objectives2")
    objective3 = gameStateSys:GetEntity("Objectives3")
    -- objective1 =  gameStateSys:GetEntityByScene("Objectives1" , "testSerialization")
    -- objective2 =  gameStateSys:GetEntityByScene("Objectives2" , "testSerialization")
    -- objective3 =  gameStateSys:GetEntityByScene("Objectives3" , "testSerialization")

    objectives = gameStateSys:GetEntity("Objectives1")
end

function Update()
    --set >= 4 for now as there are 4 'A' enemies in the scene rn
    -- print("OBJECTIVE COUNTER: " , _G.PreObjectivesCounter)
    if _G.PreObjectivesCounter >= 4 then
        --spawn the objectives portals
        print("SPAWN PORTALS")
        objective1:GetTransform().mTranslate.y = -10.5
        objective2:GetTransform().mTranslate.y = -10.5
        objective3:GetTransform().mTranslate.y = -10.5
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


