_G.PreObjectivesCounter = 0;
function Alive()
    objectives = gameStateSys:GetEntity("Objectives")
end

function Update()
    --set >= 4 for now as there are 4 'A' enemies in the scene rn
    if _G.PreObjectivesCounter >= 4 then
        --spawn the objectives portals
        objectives.GetTransform().mTranslate.y = -11
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


