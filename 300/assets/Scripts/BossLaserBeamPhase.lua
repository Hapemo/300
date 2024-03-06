local speed
local angle = 0.0
local axis = Vec3.new()

local translation = Vec3.new()

local ent 

-- [5] Lazer Attack 
local lazer_timer = 0 
local lazer_duration = 3.0

function Alive()

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
end

function Update()
    translation = trans.mTranslate 
    if(_G.activateLazerScript == true) then 

        lazer_timer = lazer_timer + FPSManager.GetDT()
        if(lazer_timer < lazer_duration) then
            translation.y = -1.19
            angle = angle + speed * (FPSManager.GetDT() * speed)
            Helper.SetRealRotateQuaternion(ent, axis, angle)
        else            
            _G.state_checker[5] = true -- mark the database
            attacking = false     
            
            for i = 1, #_G.state_checker do
                print("Phase " .. i .. ": " .. tostring(_G.state_checker[i]))
            end

            _G.activateLazerScript = false
            attacking = false
        end

    else
        translation.y = -100  
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


