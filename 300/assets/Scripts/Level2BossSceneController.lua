local fov = 45

local savedpos = Vec3.new(-1.6,33.4,58.7)
local savedplayerpos = Vec3.new(-1.1,-2.1,-16.3)
local savedbosspos = Vec3.new(0,30,-42.6)
local savedrotate = Vec3.new(270,-1.5,72)
local savedplayerrotate = Vec3.new(0,0,0)

local deadbosspos = Vec3.new(0, -30, -42.6)

local STATE =  -1 ------------------------------------ CHANGE THIS FOR INTRO---------------------------------------------
local firstTrigger = false
local bossEntity
local cameraEntity
local gunEntity

-- fov
local fov =45

local savedfovstart =57
local savedfovend = 33
local savedfovplayer = 45
-- screen shake
local Quakeintervalcd = 0
local QuakeLimit = 2
local QuakeLimit2 = 2

local quakinterv = 0.03
local quaking = 0

local magnitude = Vec2.new(-1.0,1.0)
local magfloat = 1.0
local initonce
local inittwice

-- boss run
local runtimer = 0
local runtimerover = 3

local looper = 0
local loop3r = 0

local graphicsSys
local isfightingboss
function Alive()
    initonce =false
    graphicsSys = systemManager:mGraphicsSystem()
    bossEntity = gameStateSys:GetEntity("Level2Boss")
    bossEntity:GetAnimator():PauseAnimation()
    inittwice = false
    isfightingboss = false
end

function Update()
    STATE = 1
    cameraEntity = gameStateSys:GetEntity("Camera")
    bossEntity = gameStateSys:GetEntity("Level2Boss")
    gunEntity = gameStateSys:GetEntity("gun")

    --bossEntity:GetAnimator():PauseAnimation()
    -- print("state: ", STATE)
    -- print("boolean: ", isfightingboss)

    if(STATE == -1) then
        if(isfightingboss == false) then
            bossEntity:GetTransform().mTranslate.x = savedbosspos.x
            bossEntity:GetTransform().mTranslate.y = -savedbosspos.y
            bossEntity:GetTransform().mTranslate.z = savedbosspos.z
        end

        if(isfightingboss == true) then
            bossEntity:GetTransform().mTranslate.x = savedbosspos.x
            bossEntity:GetTransform().mTranslate.y = savedbosspos.y
            bossEntity:GetTransform().mTranslate.z = savedbosspos.z
            if(looper == 0) then
                bossEntity:GetMeshRenderer():SetMesh("Boss_Roar", bossEntity)
                looper = 1
            elseif(looper == 1) then
                if(bossEntity:GetAnimator():IsEndOfAnimation()) then
                    looper = 0
                end
            end
            if bossEntity:GetHealthbar().health <= 0 then
                STATE = 3
            end
        end
    end

    if STATE == 0 then
        -- need to pause animation and initialize
        bossEntity:GetAnimator():SetFrame(0.0) -- intialize initial position
        bossEntity:GetAnimator():PauseAnimation()
        bossEntity:GetTransform().mTranslate.x = savedbosspos.x
        bossEntity:GetTransform().mTranslate.y = -savedbosspos.y
        bossEntity:GetTransform().mTranslate.z = savedbosspos.z
        if firstTrigger == false then
            firstTrigger = true

            _G.FreezePlayerControl = true
            savedplayerrotate.x = cameraEntity:GetTransform().mRotate.x
            savedplayerrotate.y = cameraEntity:GetTransform().mRotate.y
            savedplayerrotate.z = cameraEntity:GetTransform().mRotate.z
            -- savedplayerpos.x = cameraEntity:GetTransform().mTranslate.x+1
            -- savedplayerpos.y = cameraEntity:GetTransform().mTranslate.y+1
            -- savedplayerpos.z = cameraEntity:GetTransform().mTranslate.z
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
        -- boss intro fbx
        bossEntity:GetAnimator():UnpauseAnimation()

        graphicsSys:IgnoreUIScene("UI")
        graphicsSys:IgnoreUIScene("Objectives2")
        gunEntity:GetTransform().mScale.y =0
        -- gunEntity = gameStateSys:GetEntity("gun")
        -- systemManager.ecs:SetDeleteEntity(gunEntity)
        cameraEntity:GetTransform().mRotate.x = savedrotate.x
        cameraEntity:GetTransform().mRotate.y = savedrotate.y
        cameraEntity:GetTransform().mRotate.z = savedrotate.z
        cameraEntity:GetTransform().mTranslate.x = savedpos.x
        cameraEntity:GetTransform().mTranslate.y = savedpos.y
        cameraEntity:GetTransform().mTranslate.z = savedpos.z
        
        if(bossEntity:GetAnimator():GetFrame() < 30.0) then
            bossEntity:GetTransform().mTranslate.x = savedbosspos.x
            bossEntity:GetTransform().mTranslate.y = -savedbosspos.y
            bossEntity:GetTransform().mTranslate.z = savedbosspos.z
        elseif(bossEntity:GetAnimator():GetFrame() >= 30.0) then
            bossEntity:GetTransform().mTranslate.x = savedbosspos.x
            bossEntity:GetTransform().mTranslate.y = savedbosspos.y
            bossEntity:GetTransform().mTranslate.z = savedbosspos.z
        end

        if(quaking<=QuakeLimit2)then
            quaking = quaking+FPSManager.GetDT()
            Quakeintervalcd= Quakeintervalcd+FPSManager.GetDT()

            if(Quakeintervalcd>quakinterv)then
                cameraEntity:GetTransform().mRotate.x = cameraEntity:GetTransform().mRotate.x+math.random(magnitude.x,magnitude.y)+magfloat
                cameraEntity:GetTransform().mRotate.y = cameraEntity:GetTransform().mRotate.y+math.random(magnitude.x,magnitude.y)+magfloat
                Quakeintervalcd = 0

            end
            --quakinterv = quakinterv+0.0004
        end

        if( fov > 35)then
            fov = fov - 0.05
            Camera_Scripting.SetFov(cameraEntity,fov)
        else
            if(bossEntity:GetAnimator():IsEndOfAnimation()) then
                STATE = 2
            end
        end
    
    elseif STATE == 2 then
        if(looper == 0) then
            bossEntity:GetMeshRenderer():SetMesh("Boss_Roar", bossEntity)
            looper = 1
        elseif(looper == 1) then
            if(bossEntity:GetAnimator():IsEndOfAnimation()) then
                --looper = 0
            end
        end
        cameraEntity:GetTransform().mTranslate.x = savedplayerpos.x  
        cameraEntity:GetTransform().mTranslate.y = savedplayerpos.y 
        cameraEntity:GetTransform().mTranslate.z = savedplayerpos.z
        -- cameraEntity:GetTransform().mRotate.x = savedrotate.x
        -- cameraEntity:GetTransform().mRotate.y = savedrotate.y
        -- cameraEntity:GetTransform().mRotate.z = savedrotate.z
        Camera_Scripting.SetFov(cameraEntity,savedfovplayer)
        gunEntity:GetTransform().mScale.y =1
        graphicsSys:UnignoreUIScene("Objectives2")

        graphicsSys:UnignoreUIScene("UI")
        _G.FreezePlayerControl = false  
        STATE=-1
        isfightingboss = true
        fov = 45
        if initonce == false then
            controllerL2 = gameStateSys:GetEntity("DialogueControllerLevel2")
            controllerL2Scripts = controllerL2:GetScripts()
            controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/DialogueControllerLevel2.lua")

            if controllerL2Script ~= nil then
                controllerL2Script:RunFunction("FinishObjective3")
            end
            initonce = true
        end
    end

    if STATE == 3 then
        if(loop3r == 0) then
            bossEntity:GetMeshRenderer():SetMesh("Boss_Death", bossEntity)
            loop3r = 1
        elseif(loop3r == 1) then
            if(bossEntity:GetAnimator():GetFrame() >= 52.0) then
                bossEntity:GetTransform().mTranslate.x = deadbosspos.x
                bossEntity:GetTransform().mTranslate.y = deadbosspos.y
                bossEntity:GetTransform().mTranslate.z = deadbosspos.z
            end

            if(bossEntity:GetAnimator():IsEndOfAnimation()) then
                bossEntity:GetAnimator():PauseAnimation()
            end
        end
        graphicsSys:IgnoreUIScene("Objectives2")

        graphicsSys:IgnoreUIScene("UI")
        gunEntity:GetTransform().mScale.y =0
        _G.FreezePlayerControl = true  
        
        cameraEntity:GetTransform().mRotate.x = savedrotate.x
        cameraEntity:GetTransform().mRotate.y = savedrotate.y
        cameraEntity:GetTransform().mRotate.z = savedrotate.z
        cameraEntity:GetTransform().mTranslate.x = savedpos.x
        cameraEntity:GetTransform().mTranslate.y = savedpos.y
        cameraEntity:GetTransform().mTranslate.z = savedpos.z
        -- bossEntity:GetTransform().mTranslate.x = savedbosspos.x
        -- bossEntity:GetTransform().mTranslate.y = savedbosspos.y
        -- bossEntity:GetTransform().mTranslate.z = savedbosspos.z

        if(quaking<=QuakeLimit2)then
            quaking = quaking+FPSManager.GetDT()
            Quakeintervalcd= Quakeintervalcd+FPSManager.GetDT()

            if(Quakeintervalcd>quakinterv)then
                cameraEntity:GetTransform().mRotate.x = cameraEntity:GetTransform().mRotate.x+math.random(magnitude.x,magnitude.y)+magfloat
                cameraEntity:GetTransform().mRotate.y = cameraEntity:GetTransform().mRotate.y+math.random(magnitude.x,magnitude.y)+magfloat
                Quakeintervalcd = 0

            end
            -- quakinterv = quakinterv+0.0004

        end

        runtimer = runtimer + FPSManager.GetDT()

        if(runtimer > runtimerover)then
            gunEntity:GetTransform().mScale.y =1

            if inittwice == false then 
                controllerL2 = gameStateSys:GetEntity("DialogueControllerLevel2")
                controllerL2Scripts = controllerL2:GetScripts()
                controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/DialogueControllerLevel2.lua")

                if controllerL2Script ~= nil then
                    controllerL2Script:RunFunction("FinishBoss")
                end
                inittwice = true
            end
            graphicsSys:UnignoreUIScene("Objectives2")
            graphicsSys:UnignoreUIScene("UI")
            _G.FreezePlayerControl = false
            STATE = 4
        end
        -- if( fov > 35)then
        --     fov = fov - 0.05
        --     Camera_Scripting.SetFov(cameraEntity,fov)
        -- else
        --     STATE =2
        -- end

        

    end

    if(STATE == 4) then
        bossEntity:GetTransform().mTranslate.x = deadbosspos.x
        bossEntity:GetTransform().mTranslate.y = deadbosspos.y
        bossEntity:GetTransform().mTranslate.z = deadbosspos.z
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

function DialogueUpdate()
    --print("current frame: ", bossEntity:GetAnimator():GetFrame())
    if(bossEntity:GetAnimator():IsEndOfAnimation()) then
        --print("i've entered here")
        bossEntity:GetMeshRenderer():SetMesh("Boss_Roar", bossEntity)
    end
end

function StartBoss()
    STATE = 0
end