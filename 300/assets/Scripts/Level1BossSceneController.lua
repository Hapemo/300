-- saved states 

local savedpos = Vec3.new(-1.770,9.200,39.440)
-- local savedbosspos = Vec3.new(-3.396,-15.330,-64.750)
local savedbosspos = Vec3.new(-4.2,10,-28.6)
local savedrotate = Vec3.new(270,1.070,72)
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

local graphicsSys

local ani_timer

local playtimer =0
local playlimit = 5

local boss_slam_audio
local boss_roar_audio

_G.pause_animation_boss_lv1 = true --used for lv1 boss

local objective_complete_quake_roar = false
local quake_roar_played = false
local initial_slam = false
local secondary_slam = false
local secondary_roar = false


function Alive()
    initonce =false
    graphicsSys = systemManager:mGraphicsSystem()
    gameStateSys = systemManager:mGameStateSystem()

    boss_slam_audio = gameStateSys:GetEntityByScene("BossSlamAudio", "Level1Boss")
    boss_roar_audio = gameStateSys:GetEntityByScene("BossRoarAudio", "Level1Boss")
    transition_audio = gameStateSys:GetEntity("TransitionHelper")

    if boss_slam_audio ~= nil then 
        print("SLAM OK")
    end

    if boss_roar_audio ~= nil then 
        print("ROAR OK")
    end
end

function Update()

    -- STATE = 1
    cameraEntity = gameStateSys:GetEntity("Camera")
    bossEntity = gameStateSys:GetEntity("BossLevel2")
    bossEntity:GetAnimator():PauseAnimation()
    --print("current frame: ", bossEntity:GetAnimator():GetFrame())
    if STATE == 0 then
        bossEntity:GetAnimator():SetFrame(0.0) -- intialize initial position
        bossEntity:GetAnimator():PauseAnimation()
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

                
                if  objective_complete_quake_roar  == false then 
                    boss_roar_audio:GetAudio():SetPlay(1.0)
                    objective_complete_quake_roar  = true
                end

            end
        else
            quaking = 0
            STATE = 1
        end

    elseif STATE == 1 then
        transition_audio:GetAudio():SetStop()

        -- if _G.pause_animation_boss_lv1 == false then
            bossEntity:GetAnimator():UnpauseAnimation()
        -- end
     

        graphicsSys:IgnoreUIScene("UI")
        graphicsSys:IgnoreUIScene("Objectives")

        gunEntity = gameStateSys:GetEntity("gun")
        systemManager.ecs:SetDeleteEntity(gunEntity)
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
                Quakeintervalcd =0
            end
            quakinterv = quakinterv+0.0004

        end

        if bossEntity:GetAnimator():GetFrame() >= 50.0 then 
            if initial_slam == false then 
                boss_slam_audio:GetAudio():SetPlay(1.0)
                initial_slam = true
            end
        end

        if bossEntity:GetAnimator():GetFrame() >= 75.0 then 
            if secondary_slam == false then 
                boss_slam_audio:GetAudio():SetPlay(1.0)
                secondary_slam = true
            end
        end

        if bossEntity:GetAnimator():GetFrame() >= 95.0 then 
            if secondary_roar == false then 
                boss_roar_audio:GetAudio():SetPlay(1.0)
                secondary_roar = true
            end
        end

        if( fov >35)then
            fov = fov -0.05
            Camera_Scripting.SetFov(cameraEntity,fov)
        end

        --if MoveTo(bossEntity, savedbosspos, 10000) == false then
            playtimer= playtimer +FPSManager.GetDT()

            if(playtimer >= playlimit)then
                if initonce == false then
                    initonce = true
                    controllerL2 = gameStateSys:GetEntity("DialogueController")
                    controllerL2Scripts = controllerL2:GetScripts()
                    controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/DialogueControllerLevel1.lua")
        
                    if controllerL2Script ~= nil then
                        graphicsSys:UnignoreUIScene("UI")
                        graphicsSys:UnignoreUIScene("Objectives")
    
                        controllerL2Script:RunFunction("FinishObjective3")
                    end
                end
            end
            
        --end

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

function DialogueUpdate()
    --print("current frame: ", bossEntity:GetAnimator():GetFrame())
    if(bossEntity:GetAnimator():IsEndOfAnimation()) then
        --print("i've entered here")
        bossEntity:GetMeshRenderer():SetMesh("Boss_Idle", bossEntity)
    end
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