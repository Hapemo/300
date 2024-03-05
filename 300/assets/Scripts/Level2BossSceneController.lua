-- saved states 

local savedpos = Vec3.new(-4,28,72)
local savedbosspos = Vec3.new(-6,11,-40)
local savedrotate = Vec3.new(270,7.6,72)
local savedplayerrotate = Vec3.new(0,0,0)

local STATE = -1 ------------------------------------ CHANGE THIS FOR INTRO---------------------------------------------
local firstTrigger = false
local bossEntity
local cameraEntity

-- screen shake
local Quakeintervalcd = 0
local QuakeLimit = 2
local QuakeLimit2 = 2.5

local quakinterv = 0.03
local quaking = 0

local magnitude = Vec2.new(-1.0,1.0)
local magfloat = 1.0
function Alive()

end

function Update()
    cameraEntity = gameStateSys:GetEntity("Camera")
    bossEntity = gameStateSys:GetEntity("BossLevel2")
    
    if STATE == 0 then
        if firstTrigger == false then
            firstTrigger = true
            _G.FreezePlayerControl = true
            savedplayerrotate.x = cameraEntity:GetTransform().mRotate.x
            savedplayerrotate.y = cameraEntity:GetTransform().mRotate.y
            savedplayerrotate.z = cameraEntity:GetTransform().mRotate.z
            
        end

        if(quaking<=QuakeLimit)then
            quaking = quaking+FPSManager.GetDT()
            Quakeintervalcd= Quakeintervalcd+FPSManager.GetDT()
            cameraEntity:GetTransform().mRotate.x = savedplayerrotate.x
            cameraEntity:GetTransform().mRotate.y = savedplayerrotate.y
            cameraEntity:GetTransform().mRotate.z = savedplayerrotate.z
            
            if(Quakeintervalcd>quakinterv)then
                cameraEntity:GetTransform().mRotate.x = cameraEntity:GetTransform().mRotate.x+math.random(magnitude.x,magnitude.y)+magfloat
                cameraEntity:GetTransform().mRotate.y = cameraEntity:GetTransform().mRotate.y+math.random(magnitude.x,magnitude.y)+magfloat
                Quakeintervalcd =0
            end
        else
            quaking = 0
            STATE = 1
        end

    elseif STATE == 1 then

        cameraEntity:GetTransform().mRotate.x = savedrotate.x
        cameraEntity:GetTransform().mRotate.y = savedrotate.y
        cameraEntity:GetTransform().mRotate.z = savedrotate.z
        cameraEntity:GetTransform().mTranslate.x = savedpos.x
        cameraEntity:GetTransform().mTranslate.y = savedpos.y
        cameraEntity:GetTransform().mTranslate.z = savedpos.z

        if(quaking<=QuakeLimit2)then
            quaking = quaking+FPSManager.GetDT()
            Quakeintervalcd= Quakeintervalcd+FPSManager.GetDT()

            if(Quakeintervalcd>quakinterv)then
                cameraEntity:GetTransform().mRotate.x = cameraEntity:GetTransform().mRotate.x+math.random(magnitude.x,magnitude.y)+magfloat
                cameraEntity:GetTransform().mRotate.y = cameraEntity:GetTransform().mRotate.y+math.random(magnitude.x,magnitude.y)+magfloat
                Quakeintervalcd =0
            end
            quakinterv = quakinterv+0.0004

        end
        MoveTo(bossEntity, savedbosspos, 850)

    end

    -- _G.FreezePlayerControl = true
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

function MoveTo(entity, targetPos, speed)
    -- print("moving")
    -- local platformPos = Vec3.new()
    local entityPos = entity:GetTransform().mTranslate
    -- platformPos.x = targetPos.x
    -- platformPos.y = entityPos.y
    -- platformPos.z = targetPos.z
    
    local dirVec = Helper.Vec3Minus(targetPos, entityPos)
    local distAway = Helper.Vec3Len(dirVec) 
    local dir = Helper.Normalize(dirVec)
    -- print(distAway)
    if (distAway < 0.03) then -- reached target position
        --targetPos.y = entity:GetTransform().mTranslate.y
        -- entityPos.x = platformPos.x
        -- entityPos.z = platformPos.z
        -- entityPos.y = entityPos.y
        -- Helper.SetTranslate(entity, platformPos)
        -- local zeroVector = Vec3.new()
        _G.phySys:SetVelocity(entity, Vec3.new())
        -- _G.phySys:ResetEntity(entity)
        return false
    else
        _G.phySys:SetVelocity(entity, Helper.Scale(dir, speed*FPSManager.GetDT()));
    end

   
    return true
end

