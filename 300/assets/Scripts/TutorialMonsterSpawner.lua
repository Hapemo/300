_G.one_instance = false
_G.respawn_timer = 0
local spawn_pos = Vec3.new(117.391, 7.443, 0.01)

local trojanEnt 
local trojanHealthComp 

local gameStateSys
local dt

function Alive()
    gameStateSys = systemManager:mGameStateSystem();
    dt = FPSManager.GetDT()
end

function Update()
    if(_G.respawn_timer > 0) then 
        _G.respawn_timer = _G.respawn_timer - dt
    end

    if(_G.one_instance == false and _G.respawn_timer <= 0) then 
        prefabEntity = systemManager.ecs:NewEntityFromPrefab("Tutorial_Trojan_Static", spawn_pos)
        trojanEnt = gameStateSys:GetEntity("Tutorial_Trojan_Static")
        trojanHealthComp = trojanEnt:GetHealthbar()
        print("SPAWNING TROJAN SOLDIER")
        _G.one_instance = true
        _G.respawn_timer = 0
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


