local fov = 45

local savedpos = Vec3.new(-1.770,9.200,39.440)
local savedbosspos = Vec3.new(-3.396,-15.330,-64.750)
local savedrotate = Vec3.new(270,7.6,72)
local savedplayerrotate = Vec3.new(0,0,0)

local STATE =  -1 ------------------------------------ CHANGE THIS FOR INTRO---------------------------------------------
local firstTrigger = false
local bossEntity
local cameraEntity
local gunEntity

-- fov
local fov =45

-- screen shake
local Quakeintervalcd = 0
local QuakeLimit = 2
local QuakeLimit2 = 2

local quakinterv = 0.03
local quaking = 0

local magnitude = Vec2.new(-1.0,1.0)
local magfloat = 1.0
local initonce

function Alive()
    initonce =false
end

function Update()
    cameraEntity = gameStateSys:GetEntity("Camera")
    bossEntity = gameStateSys:GetEntity("Level2Boss")
    
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
        gunEntity = gameStateSys:GetEntity("gun")
        systemManager.ecs:SetDeleteEntity(gunEntity)
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
                Quakeintervalcd = 0
            end
            quakinterv = quakinterv+0.0004

        end

        if( fov > 35)then
            fov = fov - 0.05
            Camera_Scripting.SetFov(cameraEntity,fov)
        end

    end

    _G.FreezePlayerControl = true
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


