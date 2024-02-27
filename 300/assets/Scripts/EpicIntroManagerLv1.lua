-- The main brain of epic intro for lv 1

_G.gameStateSys = systemManager:mGameStateSystem()
_G.inputMapSys = systemManager:mInputActionSystem()
_G.phySys = systemManager:mPhysicsSystem();
_G.aiSys = systemManager:mAISystem();

_G.TrojanHorseEpicIntroState = 0
-- 1. MoveToStartPos
-- 2. StartPosToLedge
-- 3. LedgeToFloor
-- 4. FloorZoomIn
-- 5. ShowInfo
-- 6. HideInfo
-- 7. FloorZoomOut

_G.activateEpicTrojanHorse = false
_G.activateTrojanSoldier = false
_G.activateMelissa = false
_G.activateILY = false
_G.activateZipBomb = false

local this
--local Camera_Scripting


local defaultZoom = 45

--#region Trojan horse epic intro variables
_G.THdashStopTimer = 2
local THs2Charge = 1.8
local THZoomValue = 10
local THFlyViewWaitTime = 0.4
local THShowInfoSlowdown = 0.25
local THdashStopTimerCount
local THLedgeViewPosition
local THLedgeStopLooking
local THFlyViewPosition1Finish
local THFlyViewPosition2Finish
local THFlyViewPosition1
local THFlyViewPosition2
local THFlyViewWait
local THShowInfoSlowdownCounter


--#region Entities
local epicTrojanHorse
local trojanHorsePlatform
local player
local uiHider
--#endregion

--#region EpicIntroUI
local epicIntroUI
local showRightEpicUIPos = 0.44
local hideRightEpicUIPos = 1.53
local showLeftEpicUIPos = -0.44
local hideLeftEpicUIPos = -1.53

--#endregion

--#region black border variables
local insertBlackBorder
local retractBlackBorder
local topBorder
local bottomBorder
local BorderInnerLimitPos = 0.93 -- The y position in screen the border can move till
local BorderOuterLimitPos = 1.20 -- The y position off screen the border can move till
local topBorderUpperLimit
local topBorderLowerLimit
local bottomBorderUpperLimit
local bottomBorderLowerLimit
--#endregion

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end
    
    -- Black Border Control stuff
    insertBlackBorder = false
    retractBlackBorder = false
    topBorder = _G.gameStateSys:GetEntity("TopBlackBorder")
    bottomBorder = _G.gameStateSys:GetEntity("BottomBlackBorder")
    topBorderUpperLimit = BorderOuterLimitPos
    topBorderLowerLimit = BorderInnerLimitPos
    bottomBorderUpperLimit = -BorderInnerLimitPos
    bottomBorderLowerLimit = -BorderOuterLimitPos

    -- Init entities
    trojanHorsePlatform = _G.gameStateSys:GetEntity("TrojanHorseEpicTrigger")
    epicTrojanHorse = _G.gameStateSys:GetEntity("EpicIntroTrojanHorse")
    player = _G.gameStateSys:GetEntity("Camera")
    uiHider = _G.gameStateSys:GetEntity("UIHider")
    epicIntroUI = _G.gameStateSys:GetEntity("EpicIntroInfoUI")

    -- Hide UI stuff

    InitTrojanHorseEpicIntro()
    
end

function Update()
    if _G.activateEpicTrojanHorse then 
        SetupTrojanHorseEpicIntro()
        _G.activateEpicTrojanHorse = false
        -- need to _G.activateEpicTrojanHorse to true some where else
    end
    if _G.TrojanHorseEpicIntroState ~= 0 then RunTrojanHorseEpicIntro() end
    
    

    InsertBlackBorder(1)
    RetractBlackBorder(1)
    --#region Test LookTowards
    -- local origin = Vec3.new()
    -- origin.x = 0.1
    -- LookTowards(player, origin, 1)
    --#endregion

    --#region Test MoveTo
    --MoveTo(_G.gameStateSys:GetEntity("TrojanHorseEpicTrigger"):GetTransform().mTranslate, 50)
    --#endregion

    --#region Test black border control
    -- if (inputMapSys:GetButtonDown("InsertBlackBorder")) then
    --     -- print("insertBlackBorder = true") 
    --     insertBlackBorder = true end
    -- if (inputMapSys:GetButtonDown("RetractBlackBorder")) then 
    --     -- print("retractBlackBorder = true")
    --     retractBlackBorder = true end
    -- InsertBlackBorder(1)
    -- RetractBlackBorder(1)
    --#endregion

    --#region Test ZoomCamTo

    -- print(Camera_Scripting.GetFov(player))
    -- if (inputMapSys:GetButtonDown("nine")) then
    --     ZoomCamTo(player, 0, 1)
    -- end
    -- if (inputMapSys:GetButtonDown("zero")) then
    --     ZoomCamTo(player, 900, 1)
    -- end
    -- ZoomCamTo(player, )
    --#endregion


    --#region Test ui hider
    if (inputMapSys:GetButtonDown("nine")) then
        HideUI()
    end
    if (inputMapSys:GetButtonDown("zero")) then
        ShowUI()
    end
    --#endregion
end


function InitTrojanHorseEpicIntro()
    THLedgeViewPosition = Vec3.new(236, -40.5, 0)
    THFlyViewPosition1 = Vec3.new(134.6, -11.2, 0)
    THFlyViewPosition2 = Vec3.new(108, 1, 0)
    THdashStopTimerCount = 0
    THShowInfoSlowdownCounter = 0
    THLedgeStopLooking = false
    THFlyViewPosition1Finish = true
    THFlyViewPosition2Finish = true
    THFlyViewWait = true
end

function SetupTrojanHorseEpicIntro() 
    epicIntroUI:GetUIrenderer():SetTexture("TrojanHorseInfoUI")
    epicIntroUI:GetTransform().mTranslate.x = hideRightEpicUIPos
    insertBlackBorder = true
    _G.FreezePlayerControl = true
    _G.TrojanHorseEpicIntroState = 1
    HideUI()
end

function RunTrojanHorseEpicIntro()
    if _G.TrojanHorseEpicIntroState == 1 then -- MoveToStartPos
        -- print("_G.TrojanHorseEpicIntroState == 1")
        if not MoveTo(player, trojanHorsePlatform:GetTransform().mTranslate, 100) then
            -- print("finish moving")
            -- Finished moving
            _G.TrojanHorseEpicIntroState = 2
            systemManager.ecs:SetDeleteEntity(trojanHorsePlatform)
            ActivateEpicScript(epicTrojanHorse)
        end
        -- Notes
        -- _G.TrojanHorseStartToLedge should be activated when stepped on platform, trojan horse will also be activated at the same time
    elseif _G.TrojanHorseEpicIntroState == 2 then -- StartPosToLedge
        -- print("_G.TrojanHorseEpicIntroState == 2")
        if not THLedgeStopLooking then
            THLedgeStopLooking = not LookTowardsInterpolation(player, THLedgeViewPosition, 200)
        end
        
        if _G.aiSys:ConeOfSight(epicTrojanHorse, player, 70, 40) then _G.TrojanHorseEpicIntroState = 3 end
        -- TODO
        -- Use camera to phase to top of ledge position

    elseif _G.TrojanHorseEpicIntroState == 3 then -- LedgeToFloor
        -- print("_G.TrojanHorseEpicIntroState == 3")
        --LookTowards(player, epicTrojanHorse:GetTransform().mTranslate, 15)
        THdashStopTimerCount = THdashStopTimerCount + FPSManager:GetDT()
        
        if THFlyViewWait then
            if THdashStopTimerCount > THFlyViewWaitTime then THFlyViewWait = false end
        elseif THFlyViewPosition1Finish then
            -- print("THFlyViewPosition1")
            if not LookTowardsInterpolation(player, THFlyViewPosition1, 200) then THFlyViewPosition1Finish = false end
        elseif THFlyViewPosition2Finish then
            -- print("THFlyViewPosition2")
            if not LookTowardsInterpolation(player, THFlyViewPosition2, 160) then THFlyViewPosition2Finish = false end
        elseif THdashStopTimerCount > _G.THdashStopTimer + THs2Charge then
            epicTrojanHorse:GetAnimator():PauseAnimation()
            THdashStopTimerCount = 0
            _G.TrojanHorseEpicIntroState = 4
        end
        -- Use camera to phase to the floor position with horse on left side

    elseif _G.TrojanHorseEpicIntroState == 4 then -- FloorZoomIn
        -- print("_G.TrojanHorseEpicIntroState == 4")
        if (not ZoomCamTo(player, THZoomValue, 150)) then
            _G.TrojanHorseEpicIntroState = 5
        end
        
    elseif _G.TrojanHorseEpicIntroState == 5 then -- ShowInfo
        -- print("state 5")
        local speed = 5
        THShowInfoSlowdownCounter = THShowInfoSlowdownCounter + FPSManager.GetDT()

        if (THShowInfoSlowdownCounter < THShowInfoSlowdown) then
            MoveEpicIntroUI(epicIntroUI, 4.5, true, true)
        elseif not MoveEpicIntroUI(epicIntroUI, 0.01, true, true) then _G.TrojanHorseEpicIntroState = 6 end

        -- print("_G.TrojanHorseEpicIntroState == 5")

        -- Push the TrojanHorseInfo UI quickly into the screen
        -- Then make it slowly move diagonally

        -- Notes
        -- When time's up, quickly retreat the info with same diagonality
    elseif _G.TrojanHorseEpicIntroState == 6 then -- HideInfo
        -- print("state 6")
        if not MoveEpicIntroUI(epicIntroUI, 4, true, false) then _G.TrojanHorseEpicIntroState = 7 end
        -- Retracts the TrojanHorseInfo UI quickly
    elseif _G.TrojanHorseEpicIntroState == 7 then -- FloorZoomOut
        -- print("state 7")
        -- Zooms out quickly and
        -- Retract the black borders

        if (not ZoomCamTo(player, defaultZoom, 150)) then
            _G.TrojanHorseEpicIntroState = 0
            _G.FreezePlayerControl = false
            retractBlackBorder = true
            ShowUI()
        end

        -- deactivate ActivateTrojanHorse boolean
    end
end



----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Helper functions
----------------------------------------------------------------------------
----------------------------------------------------------------------------

-- returns false when finishes
function LookTowards(entity, target, speed)
    -- print(target)
    -- local currTarget = Vec3.new()
    -- currTarget.x = target.x
    -- currTarget.y = target.y
    -- currTarget.z = target.z
    local currPos = entity:GetTransform().mTranslate
    local currDir = Helper.Vec3Minus(Camera_Scripting.GetTarget(entity), currPos)
    local targetDir = Helper.Vec3Minus(target, currPos)
    local upVector = Camera_Scripting.Cross(currDir, targetDir)

    local targetAngle = math.pi/180 * speed * FPSManager.GetDT()
    local newDir = RotateVector(upVector, currDir, targetAngle)
    local newDirVec = Helper.DirectionToYawPitch(newDir)

    -- print(newDirVec)
    entity:GetTransform().mRotate.x = newDirVec.x
    entity:GetTransform().mRotate.y = newDirVec.y
    entity:GetTransform().mRotate.z = newDirVec.z

    -- local newDir = Camera_Scripting.Rotate(currDir, upVector, targetAngle)
    -- print(Camera_Scripting.GetTarget(entity))
    Camera_Scripting.SetTarget(entity, Helper.Vec3Add(newDir, currPos))
    -- print(Camera_Scripting.GetTarget(entity))

    if (AngleBetween(newDir, targetDir) < targetAngle) then return false end
    return true
end

-- returns false when finishes
function LookTowardsInterpolation(entity, targetPitchYaw, speed)
    local mRotate = entity:GetTransform().mRotate

    -- Cleaning up mRotate
    if (mRotate.x > -1) then
        mRotate.x = mRotate.x % 360
    else 
        mRotate.x = mRotate.x % (-360)
    end

    local direction = Helper.Normalize(Vec3.new(targetPitchYaw.x - mRotate.x, targetPitchYaw.y - mRotate.y, 0))

    -- local newRotate = Helper.Vec3Add(mRotate, Helper.Scale(direction, speed * FPSManager:GetDT()))
    -- print(newRotate)
    -- _G.phySys:SetRotation(entity, newRotate)

    local toMove = Helper.Scale(direction, speed * FPSManager:GetDT())
    -- print(toMove)
    Helper.SetRotate(entity, toMove)

    local interpolationApart = Helper.Vec3Len(Helper.Vec3Minus(entity:GetTransform().mRotate, targetPitchYaw))
    if (interpolationApart < Helper.Vec3Len(toMove)) then 
        Helper.ChangeRotate(entity, targetPitchYaw)
        return false 
    end
    return true
end

-- Rotate vector about normal in set amount of angle
function RotateVector(normal, vector, angle)
    local finalVector = Vec3.new()

    local alpha = math.sin(angle)
    local beta = math.cos(angle)
    local gamma = 1-beta
    local a = normal.x
    local b = normal.y
    local c = normal.z

    finalVector.x = vector.x*(gamma*a*a + beta) + vector.y*(gamma*a*b - alpha*c) + vector.z*(gamma*a*c + alpha*b)
    finalVector.y = vector.x*(gamma*a*b + alpha*c) + vector.y*(gamma*b*b + beta) + vector.z*(gamma*b*c - alpha*a)
    finalVector.z = vector.x*(gamma*a*c - alpha*b) + vector.y*(gamma*b*c + alpha*a) + vector.z*(gamma*c*c + beta)

    return finalVector
end

function AngleBetween(v1, v2)
    local normV1 = Helper.Normalize(v1)
    local normV2 = Helper.Normalize(v2)
    local value = math.acos(normV1.x*normV2.x + normV1.y*normV2.y + normV1.z*normV2.z)
    return value
end

function ZoomCamTo(entity, targetFOV, speed)
    -- print("ZoomCamTo")
    local currFOV = Camera_Scripting.GetFov(entity)
    local delta = speed*FPSManager:GetDT()
    if (targetFOV - currFOV) < 0 then delta = -delta end
    Camera_Scripting.SetFov(entity, currFOV + delta)

    if delta > 0 then
        -- print("delta > 0")
        if currFOV + delta > targetFOV then 
            Camera_Scripting.SetFov(entity, targetFOV)
            return false
        end
    else
        -- print("delta <= 0")
        if currFOV + delta < targetFOV then 
            Camera_Scripting.SetFov(entity, targetFOV)
            return false
        end
    end

    return true
end

-- Insert the top and bottom black borders, return False when operation is finished
function InsertBlackBorder(speed)
    if insertBlackBorder == false then return end
    -- print("InsertBlackBorder running")
    
    -- Move border
    local movement = Vec3.new()
    movement.y = -speed*FPSManager.GetDT()
    Helper.Translate(topBorder, movement)
    movement.y = speed*FPSManager.GetDT()
    Helper.Translate(bottomBorder, movement)

    -- Finish moving
    if topBorder:GetTransform().mTranslate.y < topBorderLowerLimit and bottomBorder:GetTransform().mTranslate.y > bottomBorderUpperLimit then
        -- print("InsertBlackBorder stopped")
        topBorder:GetTransform().mTranslate.y = topBorderLowerLimit
        bottomBorder:GetTransform().mTranslate.y = bottomBorderUpperLimit
        insertBlackBorder = false
    end
end

-- Retracts the top and bottom black borders, return False when operation is finished
function RetractBlackBorder(speed)
    if retractBlackBorder == false then return end
    -- print("retractBlackBorder running")

    -- Move border
    local movement = Vec3.new()
    movement.y = speed*FPSManager.GetDT()
    Helper.Translate(topBorder, movement)
    movement.y = -speed*FPSManager.GetDT()
    Helper.Translate(bottomBorder, movement)

    -- Finish moving
    if topBorder:GetTransform().mTranslate.y > topBorderUpperLimit and bottomBorder:GetTransform().mTranslate.y < bottomBorderLowerLimit then
        -- print("retractBlackBorder stopped")
        topBorder:GetTransform().mTranslate.y = topBorderUpperLimit
        bottomBorder:GetTransform().mTranslate.y = bottomBorderLowerLimit
        retractBlackBorder = false
    end
end

-- return false when reached target pos
function MoveTo(entity, targetPos, speed)
    -- print("moving")
    local platformPos = Vec3.new()
    local entityPos = entity:GetTransform().mTranslate
    platformPos.x = targetPos.x
    platformPos.y = entityPos.y
    platformPos.z = targetPos.z
    
    local dirVec = Helper.Vec3Minus(platformPos, entityPos)
    local distAway = Helper.Vec3Len(dirVec) 
    local dir = Helper.Normalize(dirVec)
    -- print(distAway)
    if (distAway < 0.03) then -- reached target position
        --targetPos.y = entity:GetTransform().mTranslate.y
        -- entityPos.x = platformPos.x
        -- entityPos.z = platformPos.z
        -- entityPos.y = entityPos.y
        Helper.SetTranslate(entity, platformPos)
        -- local zeroVector = Vec3.new()
        -- _G.phySys:SetVelocity(entity, Vec3.new())
        _G.phySys:ResetEntity(entity)
        return false
    end

    _G.phySys:SetVelocity(entity, Helper.Scale(dir, speed*FPSManager.GetDT()));
    return true
end

function HideUI()
    uiHider:GetUIrenderer().mColor.w = 27/255
end

function ShowUI()
    uiHider:GetUIrenderer().mColor.w = 25/255
end

-- Take in targeted UI position
function MoveEpicIntroUI(entity, speed, isRight, isShow)

    local x = entity:GetTransform().mTranslate.x
    local delta = speed*FPSManager.GetDT()
    -- print(delta)
    local keepRunning = true
    -- print(x)
    if isRight then
        if isShow then
            x = x - delta
            if x < showRightEpicUIPos then 
                x = showRightEpicUIPos
                keepRunning = false
            end
        else
            x = x + delta
            if x > hideRightEpicUIPos then 
                x = hideRightEpicUIPos
                keepRunning = false
            end
        end
    else
        if isShow then
            x = x + delta
            if x > showLeftEpicUIPos then 
                x = showLeftEpicUIPos
                keepRunning = false
            end
        else
            x = x - delta
            if x < hideLeftEpicUIPos then 
                x = hideLeftEpicUIPos
                keepRunning = false
            end
        end
    end

    Helper.SetTranslate(entity, Vec3.new(x,0,0))

    return keepRunning
end

function ActivateEpicScript(Entity)
    local name = string.lower(Entity:GetGeneral().name)
    print(name)
    local scriptName = ""
    -- Check name of entity
    if (string.find(name, "melissa")) then
        scriptName = "..\\assets\\Scripts\\EpicIntroMelissa.lua"
    elseif (string.find(name, "soldier")) then 
        scriptName = "..\\assets\\Scripts\\EpicIntroTrojanSoldier.lua"
    elseif (string.find(name, "horse")) then 
        scriptName = "..\\assets\\Scripts\\EpicIntroTrojanHorse.lua"
    elseif (string.find(name, "ily")) then 
        scriptName = "..\\assets\\Scripts\\EpicIntroILOVEYOU.lua"
    elseif (string.find(name, "zipbomb")) then 
        scriptName = "..\\assets\\Scripts\\EpicIntroZipBomb.lua"
    end
    print(scriptName)
    Entity:GetScripts():AddScript(Entity, scriptName)
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