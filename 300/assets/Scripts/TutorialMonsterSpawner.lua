_G.one_instance = false
local spawn_pos = Vec3.new(117.391, 7.443, 0.01)

local trojanEnt 
local trojanHealthComp 

local gameStateSys

function Alive()
    gameStateSys = systemManager:mGameStateSystem();
end

function Update()
    if(_G.one_instance == false) then 
        prefabEntity = systemManager.ecs:NewEntityFromPrefab("Tutorial_Trojan_Static", spawn_pos)
        trojanEnt = gameStateSys:GetEntity("Tutorial_Trojan_Static")
        trojanHealthComp = trojanEnt:GetHealthbar()
        print("SPAWNING TROJAN SOLDIER")
        _G.one_instance = true
    end

    -- if(trojanHealthComp ~= nil) then 
    --     if(trojanHealthComp.health <= 0) then 
    --         print("EMPTY HP LIAO BRO")
    --         systemManager.ecs:SetDeleteEntity(trojanEnt)
    --     end
    -- end
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


