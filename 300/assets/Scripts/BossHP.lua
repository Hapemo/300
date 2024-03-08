local boss_entity 
_G.bossHP_healthbar_comp = nil

local this -- hp bar
function Alive()
    boss_entity = gameStateSys:GetEntityByScene("Boss", "BossStuff")

    this = Helper.GetScriptEntity(script_entity.id)

    -- if(this ~= nil) then 
    --     print("NOT NULL")
    -- end
    _G.bossHP_healthbar_comp= boss_entity:GetHealthbar()

    if(_G.bossHP_healthbar_comp ~= nil) then 
        print("BOSS has healthbar")
        print("BOSS HP: " , _G.bossHP_healthbar_comp.health)
    end
end

function Update()
    this:GetUIrenderer():SetSlider(_G.bossHP_healthbar_comp.health / _G.bossHP_healthbar_comp.maxHealth)

    if(_G.bossHP_healthbar_comp.health) then 
        -- win screen?
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


