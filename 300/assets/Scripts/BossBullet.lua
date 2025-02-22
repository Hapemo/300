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

-- local homing_chuckle_audio

function Alive()
    bulletObject = Helper.GetScriptEntity(script_entity.id)
    this = Helper.GetScriptEntity(script_entity.id)
    gameStateSys = systemManager:mGameStateSystem();
    general = this:GetGeneral()

    dmgAudioEnt = gameStateSys:GetEntity("DamageAudio")

end

function Update()

    -- [Homing Missle] -> Health stuff
    if this:GetHealthbar().health <= 0 then 
        print("BULLET DED")
        systemManager.ecs:SetDeleteEntity(this) -- Delete the bullet upon contact
     end

     bulletLifeTime = bulletLifeTime + FPSManager.GetDT()

        if(bulletLifeTime > bulletDeathTime) then
            systemManager.ecs:SetDeleteEntity(bulletObject)
        end

     -- [Homing Missle] -> Health stuff (end)

     

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
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


