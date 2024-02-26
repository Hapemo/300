-- This script controls the camera movement of the player for epic intro of enemies, uses global variables from EpicIntroManager
local this
--local Camera_Scripting

-- Entities
local trojanHorsePlatform

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

-- starting camera direction (835,250)
-- first camera shift for horse
-- second camera shift for horse (-15, 5)

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end

    -- if Camera_Scripting == nil then print("Camera Is NILvv") end

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

    -- Hide UI stuff
    

end

function Update()
    --#region Test LookTowards
    -- local origin = Vec3.new()
    -- origin.x = 0.1
    --LookTowards(origin, 1)
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

    if _G.TrojanHorseMoveToStartPos then
        MoveTo(trojanHorsePlatform:GetTransform().mTranslate, 50)
    elseif _G.TrojanHorseStartToLedge then
        -- TODO
        -- Use camera to phase to top of ledge position
        -- While adding screen black border at the same time


        -- Immobalize player (This shouhld be done once in the platform code)

        -- Notes
        -- _G.TrojanHorseStartToLedge should be activated when stepped on platform, trojan horse will also be activated at the same time
    elseif _G.TrojanHorseLedgeToFloor then
        -- Use camera to phase to the floor position with horse on left side

    elseif _G.TrojanHorseFloorZoomIn then
        -- Zoom quickly into the horse

    elseif _G.TrojanHorseShowInfo then
        -- Push the TrojanHorseInfo UI quickly into the screen
        -- Then make it slowly move diagonally

        -- Notes
        -- When time's up, quickly retreat the info with same diagonality
    elseif _G.TrojanHorseHideInfo then
        -- Retracts the TrojanHorseInfo UI quickly
    elseif _G.TrojanHorseFloorZoomOut then
        -- Zooms out quickly and
        -- Retract the black borders
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





function LookTowards(target, speed)
    local currPos = this:GetTransform().mTranslate
    local currDir = Helper.Vec3Minus(Camera_Scripting.GetTarget(this), currPos)
    local targetDir = Helper.Vec3Minus(target, currPos)
    local upVector = Camera_Scripting.Cross(currDir, targetDir)

    local targetAngle = math.pi/180 * speed * FPSManager.GetDT()
    local newDir = RotateVector(upVector, currDir, targetAngle)
    -- local newDir = Camera_Scripting.Rotate(currDir, upVector, targetAngle)
    Camera_Scripting.SetTarget(this, Helper.Vec3Add(newDir, currPos))

    if (AngleBetween(newDir, targetDir) < targetAngle) then return false end
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

function MoveTo(targetPos, speed)
    local dir = Helper.Normalize(Helper.Vec3Minus(targetPos, this:GetTransform().mTranslate))
    dir.y = 0
    if (Helper.Vec3Len(dir) < 0.05) then -- reached target position
        local newTargetPos = targetPos
        newTargetPos.y = this:GetTransform().mTranslate.y
        Helper.SetTranslate(this, newTargetPos)
        -- local zeroVector = Vec3.new()
        _G.phySys:SetVelocity(this, Helper.Scale(dir, Vec3.new()))
        return false
    end

    _G.phySys:SetVelocity(this, Helper.Scale(dir, speed*FPSManager.GetDT()));
    return true
end

function HideUI()

end

function ShowUI()

end