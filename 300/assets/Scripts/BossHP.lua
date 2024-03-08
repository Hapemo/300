local boss_entity 
local bossHP_healthbar_comp

local this -- hp bar
function Alive()
    boss_entity = gameStateSys:GetEntityByScene("Boss", "BossStuff")

    this = Helper.GetScriptEntity(script_entity.id)

    -- if(this ~= nil) then 
    --     print("NOT NULL")
    -- end
    bossHP_healthbar_comp = boss_entity:GetHealthbar()

    if( bossHP_healthbar_comp ~= nil) then 
        print("BOSS has healthbar")
        print("BOSS HP: " , bossHP_healthbar_comp.health)
    end
end

function Update()
    this:GetUIrenderer():SetSlider(bossHP_healthbar_comp.health / bossHP_healthbar_comp.maxHealth)
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


