local boss_entity 
local bossHP_healthbar


function Alive()
    boss_entity = gameStateSys:GetEntityByScene("Boss", "BossStuff")

    bossHP_healthbar = boss_entity:GetHealthbar()

    if( bossHP_healthbar ~= nil) then 
        print("BOSS has healthbar")
    end
end

function Update()
    boss_entity:GetUIrenderer():SetSlider()
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


