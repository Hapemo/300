-- INCOMPLETE SCRIPT!!!

local timer
local timerCap
local this
local damage = 30

function Alive()

    timer = 0
    timerCap = 0.5
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Explosion Entity nil in script!") end
    -- If there's particle effect, here should spawn some particles
    
    this:GetAudio():SetPlay()
end

function Update()
    timer = timer + FPSManager.GetDT()
    if timer > timerCap then
        systemManager.ecs:SetDeleteEntity(this)
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    local tagid = Entity:GetGeneral().tagid
    if (tagid == 0) then --player id
        -- Decrease player health here, when player got hit by bomb
        Entity:GetHealthbar().health = Entity:GetHealthbar().health - damage
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end
