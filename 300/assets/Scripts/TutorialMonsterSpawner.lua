-- Tutorial (Phase 1) -> Learn about toggling 
-- [Will keep spawning till monsters are killed using all 3 weapons]
_G.trojan_1_instance = false
_G.trojan_2_instance = false

_G.Melissa_1_instance = false

_G.trojan_soldier_1_instance = false
_G.trojan_soldier_2_instance = false

_G.Tutorial_Boss_ILOVEYOU_instance = false

_G.respawn_timer = 0
local spawn_pos = Vec3.new(117.391, 7.443, 0.01)

local trojanEnt 
local trojanHealthComp 

local gameStateSys
local dt

local TutorialBossEntity
local TutorialBossHealth

function Alive()
    gameStateSys = systemManager:mGameStateSystem();
    dt = FPSManager.GetDT()
end

function Update()

    -- Spawning Logic
    if(_G.Tutorial_Phase_1_Activate == true and _G.GUITutorialActive == false) then
        -- Spawn Tutorial Boss
        if(_G.Tutorial_Boss_ILOVEYOU_instance == false ) then
            TutorialBossEntity = systemManager.ecs:NewEntityFromPrefab("ILOVEYOU_TUTORIAL_BOSS", spawn_pos)
            _G.Tutorial_Boss_ILOVEYOU_instance = true
        end

        if(TutorialBossEntity ~= nil) then
            TutorialBossHealth = TutorialBossEntity:GetHealthbar()
            
            if(TutorialBossHealth.health <= 0) then 
                _G.Tutorial_Boss_Defeated = true
            end
        end
    end

    -- [Got Some Visual Bug - not using anymore] 2/9

    
    -- if(_G.Tutorial_Boss_Defeated == true) then 
    --     if(_G.trojan_1_instance == false) then 
    --         prefabEntity = systemManager.ecs:NewEntityFromPrefab("TrojanHorse", spawn_pos)
    --         _G.trojan_1_instance = true
    --     end

    --     if(_G.trojan_2_instance == false) then 
    --         prefabEntity = systemManager.ecs:NewEntityFromPrefab("TrojanHorse", spawn_pos)
    --         _G.trojan_2_instance = true
    --     end

    --     if(_G.Melissa_1_instance == false) then 
    --         prefabEntity = systemManager.ecs:NewEntityFromPrefab("Melissa", spawn_pos)
    --         _G.Melissa_1_instance = true
    --     end

    --     if(_G.trojan_soldier_1_instance == false) then 
    --         prefabEntity = systemManager.ecs:NewEntityFromPrefab("TrojanSoldier", spawn_pos)
    --         _G.trojan_soldier_1_instance = true
    --     end

    --     if(_G.trojan_soldier_2_instance == false) then 
    --         prefabEntity = systemManager.ecs:NewEntityFromPrefab("TrojanSoldier", spawn_pos)
    --         _G.trojan_soldier_2_instance = true
    --     end
    -- end



    -- if(_G.Tutorial_Phase_2_Activate == true) then 
    --     -- trojanHorse = gameStateSys:GetEntity("TrojanHorse")
    --     -- melissa = gameStateSys:GetEntity("Melissa")
    --     -- trojanSoldier = gameStateSys:GetEntity("TrojanSoldier")

    --     if(trojanHorse ~= nil) then 

    --     end

    --     if(melissa ~= nil) then 

    --     end

    --     if(trojanSoldier ~= nil) then 


    -- end

    -- Check if they are dead, then flip the boolean
    -- if()
    

    
    -- if(_G.respawn_timer > 0) then 
    --     _G.respawn_timer = _G.respawn_timer - dt
    -- end

    -- if(_G.one_instance == false and _G.respawn_timer <= 0) then 
    --     prefabEntity = systemManager.ecs:NewEntityFromPrefab("TrojanHorse", spawn_pos)
    --     -- trojanEnt = gameStateSys:GetEntity("Tutorial_Trojan_Static")
    --     -- trojanHealthComp = trojanEnt:GetHealthbar()
    --     print("SPAWNING TROJAN SOLDIER")
    --     _G.one_instance = true
    --     _G.respawn_timer = 0
    -- end

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


