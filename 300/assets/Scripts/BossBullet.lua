local bulletObject
local bulletLifeTime = 0
local bulletDeathTime = 15

-- Phase 3 attack from Boss Level (Lv 3) 
local bullet_damage = 6 -- Adjust Accordingly

local this
local general
local spawn_bullet_name = "Boss_Bullet_Homing"
local gameStateSys

local dmgAudioEnt

function Alive()
    bulletObject = Helper.GetScriptEntity(script_entity.id)
    this = Helper.GetScriptEntity(script_entity.id)
    gameStateSys = systemManager:mGameStateSystem();
    general = this:GetGeneral()

    dmgAudioEnt = gameStateSys:GetEntity("DamageAudio")
end

function Update()
    while bulletObject ~= nil do 
        -- Customized Check (to prevent homing from despawning unless it collides with something) -> disable lifetime
        if(general.name == spawn_bullet_name) then -- won't delete the homing bullets
            break
        end
  
         bulletLifeTime = bulletLifeTime + FPSManager.GetDT()

         if(bulletLifeTime > bulletDeathTime) then
            --  systemManager.ecs:SetDeleteEntity(bulletObject)
         end

         for i , projectile in ipairs(_G.homing_projectiles) do
            if this == projectile.entity then 
                if(bulletLifeTime > bulletDeathTime) then
                    -- table.remove(_G.homing_projectiles, i) -- Delete entry in the table.
                    -- systemManager.ecs:SetDeleteEntity(this) -- Delete the bullet upon contact
                    break
                end
            end
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
        -- print("HIT BULLET")
        if(Entity:HasHealthbar()) then 
            -- print("HAS HEALTHBAR")
            player_healthbar_component = Entity:GetHealthbar()
        end

        if(player_healthbar_component ~= nil) then 
            player_healthbar_component.health = player_healthbar_component.health - bullet_damage
            -- print("TAKING DAMAGE FROM HOMING BULLET")
            -- print("DAMAGE TAKEN: " , bullet_damage)
            -- print("HP LEFT: ", player_healthbar_component.health)
            dmgAudioEnt:GetAudio():SetPlay(0.5)
        end

        -- Update Database (Homing Bullets)
        local deleted_id = 0

        for i , projectile in ipairs(_G.homing_projectiles) do
            if this == projectile.entity then 
                table.remove(_G.homing_projectiles, i) -- Delete entry in the table.
                systemManager.ecs:SetDeleteEntity(this) -- Delete the bullet upon contact
                break
            end
        end

        -- if(hello == nil) then 
        --     print("OMG DELETED LA")
        -- end
        
        
        -- print("PLAYER HIT")
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


