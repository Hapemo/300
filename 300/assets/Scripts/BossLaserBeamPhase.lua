local speed
local angle = 0.0
local axis = Vec3.new()

local translation = Vec3.new()
local trans

local ent 

-- [5] Lazer Attack 
local lazer_timer = 0 
local lazer_duration = 11

local laser_phase 
local laserPhaseAudio 

local laser_audio_played = false
local gameStateSys

-- [M5] Player Damaged (Audio)
local dmgAudioEnt
local dmgLazerAudioEnt

function Alive()
    gameStateSys = systemManager:mGameStateSystem()

    laser_phase = gameStateSys:GetEntity("LaserPhase")
    laserPhaseAudio = laser_phase:GetAudio()
    -- testing timer countdown for the rotation of the laser
    ent = Helper.GetScriptEntity(script_entity.id)
    trans = ent:GetTransform()
    speed = 7
    if(ent:GetGeneral().name == "Laser1") then
        axis.x = 0.0
        axis.y = 1.0
        axis.z = 0.0
        angle = 0.0
    elseif (ent:GetGeneral().name == "Laser2") then
        axis.x = 0.0
        axis.y = 1.0
        axis.z = 0.0
        angle = 90.0
    end

    dmgAudioEnt = gameStateSys:GetEntity("DamageAudio")
    dmgLazerAudioEnt = gameStateSys:GetEntity("DamageLazerAudio")
end

function Update()
    translation = trans.mTranslate 
    if(_G.activateLazerScript == true) then 
        -- print("HELLO ACTIVATE")
        if laser_audio_played == false then 
            -- laserPhaseAudio:SetPlay(0.5)
            laser_audio_played = true
        end
        lazer_timer = lazer_timer + FPSManager.GetDT()  
        -- print("LAZER TIMER: " , lazer_timer)
   
        if(lazer_timer < lazer_duration) then
            local position = Vec3.new(0 , -1.19, 8.5)
            Helper.SetTranslate(ent, position)
            -- translation.y = -1.19
            angle = angle + speed * (FPSManager.GetDT() * speed)
            Helper.SetRealRotateQuaternion(ent, axis, angle)
        else 
            _G.phase5_roar_bool = false           
            laserPhaseAudio:SetStop()
            local position = Vec3.new(0 , - 100, 8.5)
            Helper.SetTranslate(ent, position)
            _G.state_checker[5] = true -- mark the database
            attacking = false     
            
            for i = 1, #_G.state_checker do
                print("Phase " .. i .. ": " .. tostring(_G.state_checker[i]))
            end
            _G.state_selected = false
            _G.Boss_Not_Flinchable = false
            _G.activateLazerScript = false
            _G.attacking = false
            _G.just_enter_5 = true
            _G.lazer_appear_timer = 0
            _G.play_laser_timer = 0
            _G.phase5_roar_timer = 0
        end

    else
        local position = Vec3.new(0 , - 100, 8.5)
        Helper.SetTranslate(ent, position)
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()

    if(generalComponent.name == "Camera") then
        if(Entity:HasHealthbar()) then 
            healthComponent = Entity:GetHealthbar()
            healthComponent.health = healthComponent.health - 6
                        
            -- dmgAudioEnt:GetAudio():SetPlay()
            dmgLazerAudioEnt:GetAudio():SetPlay()
        end
    end
end

function OnTriggerExit(Entity)
    generalComponent = Entity:GetGeneral()
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


