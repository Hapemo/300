local bulletObject
local bulletLifeTime = 0
local bulletDeathTime = 10

-- Phase 3 attack from Boss Level (Lv 3) 
local bullet_damage = 20 -- Adjust Accordingly

local this
local general
local spawn_bullet_name = "Boss_Bullet_Homing"


function Alive()
    bulletObject = Helper.GetScriptEntity(script_entity.id)
    this = Helper.GetScriptEntity(script_entity.id)
    general = this:GetGeneral()
end

function Update()
    while bulletObject ~= nil do 
        -- Customized Check (to prevent homing from despawning unless it collides with something) -> disable lifetime
        -- if(general.name == spawn_bullet_name) then -- won't delete the homing bullets
        --     break
        -- end
        --print("THERE IS A BULLET OBJECT")
         bulletLifeTime = bulletLifeTime + FPSManager.GetDT()
         print("BULLET LIFETIME: " , bulletLifeTime)
         if(bulletLifeTime > bulletDeathTime) then
             systemManager.ecs:SetDeleteEntity(bulletObject)
             -- print("DELETING BULLET")
         end
         break
     end
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()

    tagid = generalComponent.tagid
    -- print("TAG ID: " , tagid)

    local player_healthbar_component

    -- print("ON TRIGGER ENTER BOSS BULLET")

    if(tagid == 0) then 
        if(Entity:HasHealthbar()) then 
            -- print("HAS HEALTHBAR")
            player_healthbar_component = Entity:GetHealthbar()
        end

        if(player_healthbar_component ~= nil) then 
            player_healthbar_component.health = player_healthbar_component.health - bullet_damage
            _G.playerHealthCurrent = _G.playerHealthCurrent - bullet_damage

            healthbar = gameStateSys:GetEntity("HealthBar", "UI")
            if(healthbar ~= nil ) then 
                -- print("HEALTHBAR EXISTS")
                -- print("CURRENT / MAX: " , playerHealthCurrent / playerHealthMax)
                -- healthbar:GetUIrenderer():SetSlider(0)
                -- healthbar:GetUIrenderer():SetSlider(playerHealthCurrent/playerHealthMax)
            end

            -- print("TAKING DAMAGE: " , bullet_damage)
            -- print("HP LEFT: " , player_healthbar_component.health)
        end

        -- print("PLAYER HIT")
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


