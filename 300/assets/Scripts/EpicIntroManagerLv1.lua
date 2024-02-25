-- The main brain of epic intro for lv 1

_G.gameStateSys = systemManager:mGameStateSystem()
_G.inputMapSys = systemManager:mInputActionSystem()
_G.phySys = systemManager:mPhysicsSystem();

_G.TrojanHorseEpicIntroState = 0
-- 1. MoveToStartPos
-- 2. StartPosToLedge
-- 3. LedgeToFloor
-- 4. FloorZoomIn
-- 5. ShowInfo
-- 6. HideInfo
-- 7. FloorZoomOut

_G.activateTrojanSoldier = false
_G.activateMelissa = false
_G.activateILY = false
_G.activateZipBomb = false

local this
--local Camera_Scripting

--#region Trojan horse state control
local thStartPosState = false

--#region Entities
local trojanHorsePlatform
local epicTrojanHorse
local player
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

    -- Hide UI stuff
    
end

function Update()
    if _G.TrojanHorseEpicIntroState ~= 0 then RunTrojanHorseEpicIntro() end
    
    

    InsertBlackBorder(1)
    RetractBlackBorder(1)
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
end



function RunTrojanHorseEpicIntro()
    if _G.TrojanHorseEpicIntroState == 1 then -- MoveToStartPos
        -- print("_G.TrojanHorseEpicIntroState == 1")
        insertBlackBorder = true
        _G.FreezePlayerControl = true
        if not MoveTo(player, trojanHorsePlatform:GetTransform().mTranslate, 100) then
            print("_G.TrojanHorseEpicIntroState finished")
            -- Finished moving
            _G.TrojanHorseEpicIntroState = 2
            ActivateEpicScript(epicTrojanHorse)
        end
    elseif _G.TrojanHorseEpicIntroState == 2 then -- StartPosToLedge
        print("_G.TrojanHorseEpicIntroState == 2")

        --Helper.SetVelocity(player, Vec3.New())
        -- TODO
        -- Use camera to phase to top of ledge position

        -- Notes
        -- _G.TrojanHorseStartToLedge should be activated when stepped on platform, trojan horse will also be activated at the same time
    elseif _G.TrojanHorseEpicIntroState == 3 then -- LedgeToFloor
        -- Use camera to phase to the floor position with horse on left side

    elseif _G.TrojanHorseEpicIntroState == 4 then -- FloorZoomIn
        -- Zoom quickly into the horse

    elseif _G.TrojanHorseEpicIntroState == 5 then -- ShowInfo
        -- Push the TrojanHorseInfo UI quickly into the screen
        -- Then make it slowly move diagonally

        -- Notes
        -- When time's up, quickly retreat the info with same diagonality
    elseif _G.TrojanHorseEpicIntroState == 6 then -- HideInfo
        -- Retracts the TrojanHorseInfo UI quickly
    elseif _G.TrojanHorseEpicIntroState == 7 then -- FloorZoomOut
        -- Zooms out quickly and
        -- Retract the black borders
        _G.FreezePlayerControl = false
        retractBlackBorder = true

        -- deactivate ActivateTrojanHorse boolean
    end
end



----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Helper functions
----------------------------------------------------------------------------
----------------------------------------------------------------------------
function LookTowards(entity, target, speed)
    local currPos = entity:GetTransform().mTranslate
    local currDir = Helper.Vec3Minus(Camera_Scripting.GetTarget(entity), currPos)
    local targetDir = Helper.Vec3Minus(target, currPos)
    local upVector = Camera_Scripting.Cross(currDir, targetDir)

    local targetAngle = math.pi/180 * speed * FPSManager.GetDT()
    local newDir = RotateVector(upVector, currDir, targetAngle)
    -- local newDir = Camera_Scripting.Rotate(currDir, upVector, targetAngle)
    Camera_Scripting.SetTarget(entity, Helper.Vec3Add(newDir, currPos))

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

-- return false when reached target pos
function MoveTo(entity, targetPos, speed)
    print("moving")
    local tempCurrLocation = entity:GetTransform().mTranslate
    targetPos.y = tempCurrLocation.y
    local dirVec = Helper.Vec3Minus(targetPos, entity:GetTransform().mTranslate)
    local distAway = Helper.Vec3Len(dirVec) 
    local dir = Helper.Normalize(dirVec)
    print(distAway)
    if (distAway < 0.03) then -- reached target position
        targetPos.y = entity:GetTransform().mTranslate.y
        Helper.SetTranslate(entity, targetPos)
        local zeroVector = Vec3.new()
        zeroVector.x = 0
        zeroVector.y = 0
        zeroVector.z = 0
        _G.phySys:SetVelocity(entity, zeroVector)
        return false
    end

    _G.phySys:SetVelocity(entity, Helper.Scale(dir, speed*FPSManager.GetDT()));
    return true
end

function HideUI()

end

function ShowUI()

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