local fov = 45

local savedpos = Vec3.new(-1.6,33.4,58.7)
local savedplayerpos = Vec3.new(-1.1,-2.1,-16.3)
local savedbosspos = Vec3.new(0,33.0,-39)
local savedrotate = Vec3.new(270,-1.5,72)
local savedplayerrotate = Vec3.new(0,0,0)

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
function Alive()
    initonce =false
    inittwice = false
end

function Update()
    cameraEntity = gameStateSys:GetEntity("Camera")
    bossEntity = gameStateSys:GetEntity("Level2Boss")
    gunEntity = gameStateSys:GetEntity("gun")


    if(STATE == -1)then
        if bossEntity:GetHealthbar().health <= 0 then
            STATE = 3
        end
    end

    if STATE == 0 then
        if firstTrigger == false then
            firstTrigger = true
            _G.FreezePlayerControl = true
            gunEntity:GetTransform().mScale.y =0
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
        -- gunEntity = gameStateSys:GetEntity("gun")
        -- systemManager.ecs:SetDeleteEntity(gunEntity)
        cameraEntity:GetTransform().mRotate.x = savedrotate.x
        cameraEntity:GetTransform().mRotate.y = savedrotate.y
        cameraEntity:GetTransform().mRotate.z = savedrotate.z
        cameraEntity:GetTransform().mTranslate.x = savedpos.x
        cameraEntity:GetTransform().mTranslate.y = savedpos.y
        cameraEntity:GetTransform().mTranslate.z = savedpos.z
        bossEntity:GetTransform().mTranslate.x = savedbosspos.x
        bossEntity:GetTransform().mTranslate.y = savedbosspos.y
        bossEntity:GetTransform().mTranslate.z = savedbosspos.z

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

        if( fov > 35)then
            fov = fov - 0.05
            Camera_Scripting.SetFov(cameraEntity,fov)
        else
            STATE =2
        end
    
    elseif STATE == 2 then

        cameraEntity:GetTransform().mTranslate.x = savedplayerpos.x  
        cameraEntity:GetTransform().mTranslate.y = savedplayerpos.y 
        cameraEntity:GetTransform().mTranslate.z = savedplayerpos.z
        -- cameraEntity:GetTransform().mRotate.x = savedrotate.x
        -- cameraEntity:GetTransform().mRotate.y = savedrotate.y
        -- cameraEntity:GetTransform().mRotate.z = savedrotate.z
        Camera_Scripting.SetFov(cameraEntity,savedfovplayer)
        gunEntity:GetTransform().mScale.y =1
        _G.FreezePlayerControl = false  
        STATE=-1

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
            
            -- _G.FreezePlayerControl = false
            if inittwice == false then 
                controllerL2 = gameStateSys:GetEntity("DialogueControllerLevel2")
                controllerL2Scripts = controllerL2:GetScripts()
                controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/DialogueControllerLevel2.lua")

                if controllerL2Script ~= nil then
                    controllerL2Script:RunFunction("FinishBoss")
                end
                inittwice = true
            end
            STATE = 4
        end
        -- if( fov > 35)then
        --     fov = fov - 0.05
        --     Camera_Scripting.SetFov(cameraEntity,fov)
        -- else
        --     STATE =2
        -- end

        

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


function StartBoss()
    STATE = 0
end