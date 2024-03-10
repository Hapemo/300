-- saved states 

-- local savedpos = Vec3.new(-1.770,9.200,39.440)
-- local savedbosspos = Vec3.new(-3.396,-15.330,-64.750)
-- local savedrotate = Vec3.new(270,7.6,72)
local savedplayerrotate = Vec3.new(0,0,0)

local gunscale = 1

local rotateY = -8.7


_G.level3intro = true

local STATE =  0 ------------------------------------ CHANGE THIS FOR INTRO---------------------------------------------
local firstTrigger = false
local bossEntity
local cameraEntity
local gunEntity
local uiHider

-- fov
local fov =57
local savedfovstart =57
local savedfovend = 46
local savedfovplayer = 45

-- screen shake
local Quakeintervalcd = 0
local QuakeLimit = 2
local QuakeLimit2 = 2
local quaketimer = 0

local quakinterv = 0.03
local quaking = 0

local magnitude = Vec2.new(-1.0,1.0)
local magfloat = 0.5
local initonce


local roared = false

local graphicsSys
function Alive()
    initonce =false
    uiHider = gameStateSys:GetEntity("UIHider")
    cameraEntity = gameStateSys:GetEntity("Camera")
    gunEntity = gameStateSys:GetEntity("gun")
    bossEntity = gameStateSys:GetEntity("Boss")

    graphicsSys = systemManager:mGraphicsSystem()


    _G.FreezePlayerControl = true
    gunEntity:GetTransform().mScale.y =0
    Camera_Scripting.SetFov(cameraEntity,savedfov)

    HideUI()
end

function Update()
    cameraEntity = gameStateSys:GetEntity("Camera")
    gunEntity = gameStateSys:GetEntity("gun")
    bossEntity = gameStateSys:GetEntity("Boss")
    
    if(bossEntity:GetAnimator():IsEndOfAnimation()) then
        
        -- print("ANIMATION DONE")
        if roared == false then 
            bossEntity:GetMeshRenderer():SetMesh("Boss_Roar" , bossEntity)
            roared = true
        end
    end

    -- gunEntity.GetTransform().mScale.y =0
    -- Camera_Scripting.SetFov(cameraEntity,savedfov)
    
    if STATE == 0 then
        graphicsSys:IgnoreUIScene("UI")
        graphicsSys:IgnoreUIScene("BossUI")
    --     if firstTrigger == false then
    --         firstTrigger = true
    --         _G.FreezePlayerControl = true

        Camera_Scripting.SetFov(cameraEntity,fov)
        fov = fov - 0.05

        if(fov <= savedfovend)then
            savedplayerrotate.x = cameraEntity:GetTransform().mRotate.x
            savedplayerrotate.y = cameraEntity:GetTransform().mRotate.y
            savedplayerrotate.z = cameraEntity:GetTransform().mRotate.z
            STATE = 1
        end

    elseif STATE == 1 then  
        quaketimer= quaketimer+FPSManager.GetDT()
        if(quaketimer >= 1  and quaketimer <1.5)then
            cameraEntity:GetTransform().mRotate.x = savedplayerrotate.x
            cameraEntity:GetTransform().mRotate.y = savedplayerrotate.y
            cameraEntity:GetTransform().mRotate.z = savedplayerrotate.z
            Quakeintervalcd= Quakeintervalcd+FPSManager.GetDT()
            if(Quakeintervalcd>quakinterv)then
                cameraEntity:GetTransform().mRotate.x = cameraEntity:GetTransform().mRotate.x+math.random(magnitude.x,magnitude.y)+magfloat
                cameraEntity:GetTransform().mRotate.y = cameraEntity:GetTransform().mRotate.y+math.random(magnitude.x,magnitude.y)+magfloat
                Quakeintervalcd =0
            end
        elseif(quaketimer >= 5)then
            STATE = 2
        end


    elseif STATE == 2 then  

        gunEntity:GetTransform().mScale.y =1
        Camera_Scripting.SetFov(cameraEntity,savedfovplayer)

            controllerL2 = gameStateSys:GetEntity("DialogueControllerLevel3")
            controllerL2Scripts = controllerL2:GetScripts()
            controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/ DialogueControllerLevel3.lua")

            if controllerL2Script ~= nil then
                controllerL2Script:RunFunction("StartIntro")
            end
            graphicsSys:UnignoreUIScene("UI")
            graphicsSys:UnignoreUIScene("BossUI")

            _G.FreezePlayerControl = false
            ShowUI()
        STATE = -1
    end


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
    if (distAway < 0.1) then -- reached target position
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

function ChangeState()
    STATE = 0
end

function HideUI()
    uiHider:GetUIrenderer().mColor.w = 27/255
end

function ShowUI()
    uiHider:GetUIrenderer().mColor.w = 25/255
end