local this
local currentGroundSlamTimer = 0
local maxGroundSlamTimer = 0.8 -- how long the ground slam lasts
local height

function Alive()

end

function Update()
    
    currentGroundSlamTimer = currentGroundSlamTimer + FPSManager.GetDT()

    -- spawn for 0.2 seoncds at 0 height, then despawn
    -- reaeat three times

    if currentGroundSlamTimer >= maxGroundSlamTimer then
        entity = Helper.GetScriptEntity(script_entity.id)
        systemManager.ecs:SetDeleteEntity(entity)
    end

    if currentGroundSlamTimer < maxGroundSlamTimer / 2 then
        this = Helper.GetScriptEntity(script_entity.id)
        this:GetTransform().mTranslate.y = this:GetTransform().mTranslate.y + 0.1
    end

    if currentGroundSlamTimer > maxGroundSlamTimer / 2 then
        this = Helper.GetScriptEntity(script_entity.id)
        this:GetTransform().mTranslate.y = this:GetTransform().mTranslate.y - 0.1
    end

end

function Dead()

end

function OnTriggerEnter(Entity)
    
    -- TODO: Damage player while in contact

end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


