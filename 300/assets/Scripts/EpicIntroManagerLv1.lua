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

_G.TSEpicIntroState = 0
-- 1. Move to start pos
-- 2. Wait for trojan soldiers to come out
-- 3. Quick Look & ZoomCamTo at solder 1 
-- 4. Slow panning to right 
-- 5. Quick Look at solder 2 
-- 6. Slow panning to right 
-- 7. Quick Look at solder 3 
-- 8. Slow panning to right 
-- 9. Quick Look at solder 4 
-- 10. Slow panning to right 
-- 11. ShowInfo 
-- 12. Hideinfo
-- 13. Zoom Out

_G.ILYEpicIntroState = 0
-- 1. Move to start pos and activate ILY
-- 2. Wait for ILY to shoot player (move to next state when shot by ILY)
-- 3. Wait a bit after getting shot then rotate at normal speed to ILY
-- 4. Zoom into ILY
-- 5. Show Info
-- 6. Hide Info
-- 7. Zoom out and resume game

_G.ZBEpicIntroState = 0
-- 1. Move to start pos
-- 2. Make player look at front (ZBView1)
-- 3. Walk forward and pass the zipbomb (using ZBpos1) (It should catch a glimpse of the zip bomb)
-- 4. Walk back and activate zipbomb midway
-- 5. Upon hearing zipbomb fuse, QUICK turn to look at zipbomb
-- 6. ShowInfo
-- 7. HideInfo
-- 8. result game

_G.MEpicIntroState = 0
-- 1. Move to start pos
-- 2. Make player look at front (MView1) (Make melissa scream when ended (attach script to it))
-- 3. Wait timer for melissa to scream midway (Pause animation and audio)
-- 4. Zoom in
-- 5. Show info
-- 6. Hide information
-- 7. result game

_G.activateEpicTrojanHorse = false
_G.activateEpicTS = false
_G.activateEpicM = false
_G.activateEpicILY = false
_G.activateEpicZB = false

_G.completedEpicTH = false
_G.completedEpicTS = false
_G.completedEpicILY = false
_G.completedEpicM = false
_G.completedEpicZB = false

local this
--local Camera_Scripting


local defaultZoom = 45

--#region Trojan horse epic intro variables
_G.THdashStopTimer = 2
local THs2Charge = 1.8
local THZoomValue = 10
local THFlyViewWaitTime = 0.4
local THdashStopTimerCount
local THLedgeViewPosition
local THLedgeStopLooking
local THFlyViewPosition1Finish
local THFlyViewPosition2Finish
local THFlyViewPosition1
local THFlyViewPosition2
local THFlyViewWait
--#endregion

--#region Trojan soldier epic intro variables
local TSView1
local TSView2
local TSView3
local TSView4
local TSWaitTime
local TSWaitCounter
local TSPanningTime
local TSPanningCounter
local TSZoomValue
local TSZoom1Done
local TSLook1Done
--#endregion

--#region ILY epic intro variables
local shotTriggerEffectTime
local shotWaitTime
local shotWaitTimeCounter
_G.ILYShotAlready = false
local ILYView1
local ILYView2
local ILYZoomValue
--#endregion

--#region ZB epic intro variables
local ZBWaitTimerCounter
local ZBWaitTimer
local ZBPos1
local ZBPos2
local ZBPos15
local ZBView1
local ZBView2
local ZBRunOnce
--#endregion

--#region M epic intro variables
local MPos1
local MView1
local MViewUpperLimitx
local MViewLowerLimitx
local MAddScriptRunOnce
local MZoomVal
--#endregion

--#region Entities
local player
local uiHider

local epicTrojanHorse
local trojanHorsePlatform

local epicTS1
local epicTS2
local epicTS3
local epicTS4
local TSPlatform

local epicILY
local ILYPlatform

local epicZB
local ZBPlatform

local epicM
local MPlatform
--#endregion

--#region EpicIntroUI
local epicIntroUI
local ShowInfoSlowdown = 0.25
local ShowInfoSlowdownCounter
local ShowInfoMinTime = 4
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
    player = _G.gameStateSys:GetEntity("Camera")
    uiHider = _G.gameStateSys:GetEntity("UIHider")
    epicIntroUI = _G.gameStateSys:GetEntity("THEpicIntroInfoUI")

    -- Hide UI stuff

    InitTrojanHorseEpicIntro()
    InitTSEpicIntro()
    InitILYEpicIntro()
    InitZBEpicIntro()
    InitMEpicIntro()
end

function Update()
    if _G.activateEpicTrojanHorse then 
        SetupTrojanHorseEpicIntro()
        _G.activateEpicTrojanHorse = false
        -- need to _G.activateEpicTrojanHorse to true some where else
    end
    if _G.TrojanHorseEpicIntroState ~= 0 then RunTrojanHorseEpicIntro() end
    
    if _G.activateEpicTS then 
        SetupTSEpicIntro()
        _G.activateEpicTS = false
    end
    if _G.TSEpicIntroState ~= 0 then RunTSEpicIntro() end

    if _G.activateEpicILY then 
        SetupILYEpicIntro()
        _G.activateEpicILY = false
    end
    if _G.ILYEpicIntroState ~= 0 then RunILYEpicIntro() end
    
    if _G.activateEpicZB then 
        SetupZBEpicIntro()
        _G.activateEpicZB = false
    end
    if _G.ZBEpicIntroState ~= 0 then RunZBEpicIntro() end
    
    if _G.activateEpicM then 
        SetupMEpicIntro()
        _G.activateEpicM = false
    end
    if _G.MEpicIntroState ~= 0 then RunMEpicIntro() end

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
    -- if (inputMapSys:GetButtonDown("nine")) then
    --     HideUI()
    -- end
    -- if (inputMapSys:GetButtonDown("zero")) then
    --     ShowUI()
    -- end
    --#endregion

    
    InsertBlackBorder(1)
    RetractBlackBorder(1)
end




----------------------------------------------------------------------------
-- Trojan Horse Intro Functions
----------------------------------------------------------------------------

--#region trojan horse intro
function InitTrojanHorseEpicIntro()
    trojanHorsePlatform = _G.gameStateSys:GetEntity("TrojanHorseEpicTrigger")
    epicTrojanHorse = _G.gameStateSys:GetEntity("EpicIntroTrojanHorse")
    THLedgeViewPosition = Vec3.new(236, -40.5, 0)
    THFlyViewPosition1 = Vec3.new(134.6, -11.2, 0)
    THFlyViewPosition2 = Vec3.new(108, 1, 0)
    THdashStopTimerCount = 0
    THLedgeStopLooking = false
    THFlyViewPosition1Finish = true
    THFlyViewPosition2Finish = true
    THFlyViewWait = true
end

function SetupTrojanHorseEpicIntro() 
    epicIntroUI = _G.gameStateSys:GetEntity("THEpicIntroInfoUI")
    epicIntroUI:GetTransform().mTranslate.x = hideRightEpicUIPos
    _G.TrojanHorseEpicIntroState = 1
    GeneralSetup()
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
        ShowInfoSlowdownCounter = ShowInfoSlowdownCounter + FPSManager.GetDT()
        local minTimeReached = false
        if ShowInfoSlowdownCounter > ShowInfoMinTime then minTimeReached = true end
        if (ShowInfoSlowdownCounter < ShowInfoSlowdown) then
            MoveEpicIntroUI(epicIntroUI, 4.3, true, true)
        elseif not MoveEpicIntroUI(epicIntroUI, 0.01, true, true) and minTimeReached then _G.TrojanHorseEpicIntroState = 6 end

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
            epicTrojanHorse:GetAnimator():UnpauseAnimation()
            ShowUI()
            _G.completedEpicTH = true
        end

        -- deactivate ActivateTrojanHorse boolean
    end
end
--#endregion


--#region trojan soldier intro
function InitTSEpicIntro()
    TSPlatform = _G.gameStateSys:GetEntity("TrojanSoldierEpicTrigger")
    epicTS1 = _G.gameStateSys:GetEntity("EpicTrojanSoldier1")
    epicTS2 = _G.gameStateSys:GetEntity("EpicTrojanSoldier2")
    epicTS3 = _G.gameStateSys:GetEntity("EpicTrojanSoldier3")
    epicTS4 = _G.gameStateSys:GetEntity("EpicTrojanSoldier4")
    -- THLedgeViewPosition = Vec3.new(236, -40.5, 0)
    -- THFlyViewPosition1 = Vec3.new(134.6, -11.2, 0)
    -- THFlyViewPosition2 = Vec3.new(108, 1, 0)
    -- THdashStopTimerCount = 0
    -- ShowInfoSlowdownCounter = 0
    -- THLedgeStopLooking = false
    -- THFlyViewPosition1Finish = true
    -- THFlyViewPosition2Finish = true
    -- THFlyViewWait = true
end

function SetupTSEpicIntro() 
    epicIntroUI = _G.gameStateSys:GetEntity("TSEpicIntroInfoUI")
    epicIntroUI:GetTransform().mTranslate.x = hideLeftEpicUIPos
    _G.TSEpicIntroState = 1
    TSView1 = Vec3.new(147, 1.8, 0)
    TSView2 = Vec3.new(172, 1.5, 0)
    TSView3 = Vec3.new(203, 1.5, 0)
    TSView4 = Vec3.new(227, 1.5, 0)
    TSWaitTime = 2.3
    TSWaitCounter = 0
    TSPanningTime = 1.5
    TSPanningCounter = 0
    TSZoomValue = 10
    TSZoom1Done = false
    TSLook1Done = false
    GeneralSetup()
end

function RunTSEpicIntro()
    print("_G.TSEpicIntroState:")
    print(_G.TSEpicIntroState)
    if _G.TSEpicIntroState > 2 then 
        Helper.SetRealRotate(epicTS3, Vec3.new(0,90,0))
        epicTS3:GetTransform().mTranslate = Vec3.new(13.7, -11, 32.4)
    end

    if _G.TSEpicIntroState == 1 then -- Move to start pos
        if not MoveTo(player, TSPlatform:GetTransform().mTranslate, 100) then
            -- print("finish moving")
            -- Finished moving
            _G.TSEpicIntroState = 2
            systemManager.ecs:SetDeleteEntity(TSPlatform)
            AddScriptToTS()
        end
    elseif _G.TSEpicIntroState == 2 then -- Wait for trojan soldiers to come out
        TSWaitCounter = TSWaitCounter + FPSManager.GetDT()
        if TSWaitCounter > TSWaitTime then
            RemoveScriptsFromTS()
            _G.TSEpicIntroState = 3 
        end
    elseif _G.TSEpicIntroState == 3 then -- Quick Look & ZoomCamTo at solder 1
        if not TSZoom1Done then TSZoom1Done = not LookTowardsInterpolation(player, TSView4, 150) end
        if not TSLook1Done then TSLook1Done = not ZoomCamTo(player, TSZoomValue, 125) end
        if TSLook1Done and TSZoom1Done then _G.TSEpicIntroState = 4 end
    elseif _G.TSEpicIntroState == 4 then -- Slow panning to right 
        TSPanningCounter = TSPanningCounter + FPSManager.GetDT()
        if (TSPanningCounter > TSPanningTime) then
            TSPanningCounter = 0
            _G.TSEpicIntroState = 5
        end
        LookTowardsInterpolation(player, Helper.Vec3Minus(TSView4, Vec3.new(179,0,0)), 5.25)
    elseif _G.TSEpicIntroState == 5 then -- Quick Look at solder 2 
        if not LookTowardsInterpolation(player, TSView3, 150) then _G.TSEpicIntroState = 6 end
    elseif _G.TSEpicIntroState == 6 then -- Slow panning to right 
        TSPanningCounter = TSPanningCounter + FPSManager.GetDT()
        if (TSPanningCounter > TSPanningTime) then
            TSPanningCounter = 0
            _G.TSEpicIntroState = 7
        end
        LookTowardsInterpolation(player, Helper.Vec3Minus(TSView3, Vec3.new(179,0,0)), 5.25)
    elseif _G.TSEpicIntroState == 7 then -- Quick Look at solder 3 
        if not LookTowardsInterpolation(player, TSView2, 150) then _G.TSEpicIntroState = 8 end
    elseif _G.TSEpicIntroState == 8 then -- Slow panning to right 
        TSPanningCounter = TSPanningCounter + FPSManager.GetDT()
        if (TSPanningCounter > TSPanningTime) then
            TSPanningCounter = 0
            _G.TSEpicIntroState = 9
        end
        LookTowardsInterpolation(player, Helper.Vec3Minus(TSView2, Vec3.new(179,0,0)), 5.25)
    elseif _G.TSEpicIntroState == 9 then -- Quick Look at solder 4 
        if not LookTowardsInterpolation(player, TSView1, 150) then _G.TSEpicIntroState = 10 end
    elseif _G.TSEpicIntroState == 10 then -- Slow panning to right
        TSPanningCounter = TSPanningCounter + FPSManager.GetDT()
        if (TSPanningCounter > TSPanningTime) then
            TSPanningCounter = 0
            _G.TSEpicIntroState = 11
        end 
        LookTowardsInterpolation(player, Helper.Vec3Minus(TSView1, Vec3.new(179,0,0)), 5.25)
    elseif _G.TSEpicIntroState == 11 then -- ShowInfo 
        ShowInfoSlowdownCounter = ShowInfoSlowdownCounter + FPSManager.GetDT()
        local minTimeReached = false
        if ShowInfoSlowdownCounter > ShowInfoMinTime then minTimeReached = true end
        if (ShowInfoSlowdownCounter < ShowInfoSlowdown) then
            MoveEpicIntroUI(epicIntroUI, 4.3, false, true)
        elseif not MoveEpicIntroUI(epicIntroUI, 0.01, false, true) and minTimeReached then _G.TSEpicIntroState = 12 end
    elseif _G.TSEpicIntroState == 12 then -- Hideinfo
            if not MoveEpicIntroUI(epicIntroUI, 4, false, false) then _G.TSEpicIntroState = 13 end
    elseif _G.TSEpicIntroState == 13 then -- Zoom Out
        if (not ZoomCamTo(player, defaultZoom, 150)) then
            _G.TSEpicIntroState = 0
            _G.FreezePlayerControl = false
            retractBlackBorder = true
            ShowUI()
            AddScriptToTS()
            _G.completedEpicTS = true
        end
    end
end

function AddScriptToTS()
    local scriptName = "..\\assets\\Scripts\\EpicIntroTrojanSoldier.lua"
    epicTS1:GetScripts():AddScript(epicTS1, scriptName)
    epicTS2:GetScripts():AddScript(epicTS2, scriptName)
    epicTS3:GetScripts():AddScript(epicTS3, scriptName)
    epicTS4:GetScripts():AddScript(epicTS4, scriptName)
end

function RemoveScriptsFromTS()
    local scriptName = "..\\assets\\Scripts\\EpicIntroTrojanSoldier.lua"
    epicTS1:GetScripts():DeleteScript(scriptName)
    epicTS2:GetScripts():DeleteScript(scriptName)
    epicTS3:GetScripts():DeleteScript(scriptName)
    epicTS4:GetScripts():DeleteScript(scriptName)
end

--#endregion

--#region I Love You intro
function InitILYEpicIntro()
    ILYPlatform = _G.gameStateSys:GetEntity("ILYEpicTrigger")
    epicILY = _G.gameStateSys:GetEntity("EpicILY")
end

function SetupILYEpicIntro() 
    epicIntroUI = _G.gameStateSys:GetEntity("ILYEpicIntroInfoUI")
    epicIntroUI:GetTransform().mTranslate.x = hideLeftEpicUIPos
    _G.ILYEpicIntroState = 1
    shotTriggerEffectTime = 2.33
    shotWaitTime = shotTriggerEffectTime + 0.5
    shotWaitTimeCounter = 0
    _G.ILYShotAlready = false
    ILYView1 = Vec3.new(129.5, -29, 0)
    ILYView2 = Vec3.new(129.5, 0, 0)
    ILYZoomValue = 5
    GeneralSetup()
end

function RunILYEpicIntro()
    if _G.ILYShotAlready then
        Helper.SetRealRotate(epicILY, Vec3.new(0,130.013,0))
    end 

    if _G.ILYEpicIntroState == 1 then -- Move to start pos and dwactivate ILY
        if not MoveTo(player, ILYPlatform:GetTransform().mTranslate, 100) then
            _G.ILYEpicIntroState = 100 -- To keep ILY waiting
            systemManager.ecs:SetDeleteEntity(ILYPlatform)
            epicILY:GetScripts():AddScript(epicILY, "..\\assets\\Scripts\\EpicIntroILY.lua")
        end
    elseif _G.ILYEpicIntroState == 2 then -- Wait for ILY to shoot player (move to next state when shot by ILY), then wait abit more
        LookTowardsInterpolation(player, ILYView2, 75)
        shotWaitTimeCounter = shotWaitTimeCounter + FPSManager.GetDT()
        if not _G.ILYShotAlready then
            if shotWaitTimeCounter >shotTriggerEffectTime then 
                _G.ILYShotAlready = true 
                -- print("SHOTTED!!!")
                _G.ILYBulletHitPlayer = true
                _G.phySys:SetVelocity(epicILY, Vec3.new())
                epicILY:GetAnimator():PauseAnimation()
            end
        elseif shotWaitTime < shotWaitTimeCounter then _G.ILYEpicIntroState = 3 end
    elseif _G.ILYEpicIntroState == 3 then -- Rotate at normal speed to ILY (freeze ILY at the end)
        if not LookTowardsInterpolation(player, ILYView1, 125) then 
            _G.ILYEpicIntroState = 4
        end
    elseif _G.ILYEpicIntroState == 4 then -- Zoom into ILY
        if not ZoomCamTo(player, ILYZoomValue, 100) then _G.ILYEpicIntroState = 5 end
    elseif _G.ILYEpicIntroState == 5 then -- Show Info
        -- ShowInfoSlowdownCounter = ShowInfoSlowdownCounter + FPSManager.GetDT()
        -- if (ShowInfoSlowdownCounter < ShowInfoSlowdown) then
        --     MoveEpicIntroUI(epicIntroUI, 4.3, false, true)
        -- end
        ShowInfoSlowdownCounter = ShowInfoSlowdownCounter + FPSManager.GetDT()
        local minTimeReached = false
        if ShowInfoSlowdownCounter > ShowInfoMinTime then minTimeReached = true end
        if (ShowInfoSlowdownCounter < ShowInfoSlowdown) then
            MoveEpicIntroUI(epicIntroUI, 4.35, false, true)
        elseif not MoveEpicIntroUI(epicIntroUI, 0.01, false, true) and minTimeReached then _G.ILYEpicIntroState = 6 end
    elseif _G.ILYEpicIntroState == 6 then -- Hide Info
        if not MoveEpicIntroUI(epicIntroUI, 4, false, false) then _G.ILYEpicIntroState = 7 end
    elseif _G.ILYEpicIntroState == 7 then -- Zoom out and resume game
        if (not ZoomCamTo(player, defaultZoom, 100)) then
            _G.ILYEpicIntroState = 0
            _G.FreezePlayerControl = false
            retractBlackBorder = true
            epicILY:GetAnimator():UnpauseAnimation()
            ShowUI()
            _G.completedEpicILY = true
        end
    end
end

--#endregion

--#region Zip Bomb Intro
function InitZBEpicIntro()
    ZBPlatform = _G.gameStateSys:GetEntity("ZipBombEpicTrigger")
    epicZB = _G.gameStateSys:GetEntity("EpicZipBomb")
end

function SetupZBEpicIntro() 
    --print("SetupZBEpicIntro()")
    epicIntroUI = _G.gameStateSys:GetEntity("ZBEpicIntroInfoUI")
    epicIntroUI:GetTransform().mTranslate.x = hideRightEpicUIPos
    _G.ZBEpicIntroState = 1
    ZBPos1 = Vec3.new(-2.2, -3.8, 13.5)
    ZBPos15 = Vec3.new(-2.2, -3.8, 12.3)
    ZBPos2 = Vec3.new(-2.2, -3.8, 12.1)
    ZBView1 = Vec3.new(90, 0, 0)
    ZBView2 = Vec3.new(165, -3, 0)
    ZBWaitTimerCounter = 0
    ZBWaitTimer = 1
    ZBAddScriptTime = 1
    ZBRunOnce = true
    GeneralSetup()
end

function RunZBEpicIntro()
    -- if _G.ZBShotAlready then
    --     epicZB:GetTransform().mRotate = Vec3.new(0, 130.013, 0)
    -- end 
    -- Always look at the player when midway through exploding
    Helper.LookAtTarget(epicZB)
    --print(_G.ZBEpicIntroState)

    if _G.ZBEpicIntroState == 1 then -- Move to start pos
        if not MoveTo(player, ZBPlatform:GetTransform().mTranslate, 100) then
            _G.ZBEpicIntroState = 2 -- To keep ZB waiting
            systemManager.ecs:SetDeleteEntity(ZBPlatform)
        end
    elseif _G.ZBEpicIntroState == 2 then -- Make player look at front (ZBView1)
        if not LookTowardsInterpolation(player, ZBView1, 75) then _G.ZBEpicIntroState = 3 end        
    elseif _G.ZBEpicIntroState == 3 then -- Walk forward and pass the zipbomb (using ZBpos1) (It should catch a glimpse of the zip bomb)
        if not MoveTo(player, ZBPos1, 100) then _G.ZBEpicIntroState = 4 end
    elseif _G.ZBEpicIntroState == 4 then -- Walk back and activate zipbomb midway (add script to zb)
        ZBWaitTimerCounter = ZBWaitTimerCounter + FPSManager.GetDT()
        if ZBWaitTimer < ZBWaitTimerCounter then
            if not MoveTo(player, ZBPos15, 75) then 
                _G.ZBEpicIntroState = 4.5 
                epicZB:GetScripts():AddScript(epicZB, "..\\assets\\Scripts\\EpicIntroZB.lua")
            end
        end
    elseif _G.ZBEpicIntroState == 4.5 then
        if not MoveTo(player, ZBPos2, 75) then 
            _G.ZBEpicIntroState = 5 
        end
    elseif _G.ZBEpicIntroState == 5 then -- Upon hearing zipbomb fuse, QUICK turn to look at zipbomb and pause it's explosion animation
        if not LookTowardsInterpolation(player, ZBView2, 175) then
            
            _G.ZBEpicIntroState = 6 
        end
    elseif _G.ZBEpicIntroState == 6 then -- ShowInfo
        ShowInfoSlowdownCounter = ShowInfoSlowdownCounter + FPSManager.GetDT()

        if ZBRunOnce and (ShowInfoSlowdownCounter < ShowInfoSlowdown) then
            ZBRunOnce = false
            gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetPlay()
            epicZB:GetAnimator():PauseAnimation()
            -- epicZB:GetAudio():SetStop()
        end

        if (ShowInfoSlowdownCounter < ShowInfoSlowdown) then MoveEpicIntroUI(epicIntroUI, 4.42, true, true)
        elseif not MoveEpicIntroUI(epicIntroUI, 0.01, true, true) and
               ShowInfoSlowdownCounter > ShowInfoMinTime then _G.ZBEpicIntroState = 7 end
    elseif _G.ZBEpicIntroState == 7 then -- HideInfo
        if not MoveEpicIntroUI(epicIntroUI, 4, true, false) then 
            _G.ZBEpicIntroState = 0
            _G.FreezePlayerControl = false
            retractBlackBorder = true
            epicZB:GetAnimator():UnpauseAnimation()
            epicZB:GetAudio():SetPlay()
            ShowUI()
            _G.completedEpicZB = true
        end
    end
end

--#endregion


--#region Melissa Intro
function InitMEpicIntro()
    MPlatform = _G.gameStateSys:GetEntity("MelissaEpicTrigger")
    epicM = _G.gameStateSys:GetEntity("EpicMelissa")
end

function SetupMEpicIntro() 
    --print("SetupMEpicIntro()")
    epicIntroUI = _G.gameStateSys:GetEntity("MEpicIntroInfoUI")
    epicIntroUI:GetTransform().mTranslate.x = hideLeftEpicUIPos
    _G.MEpicIntroState = 1
    MPos1 = Vec3.new(2.39, -10.4, 19.4)
    MView1 = Vec3.new(200, 1, 0)
    MViewUpperLimitx = 220
    MViewLowerLimitx = 190
    MZoomVal = 10
    MAddScriptRunOnce = true
    systemManager.ecs:SetDeleteEntity(MPlatform)
    GeneralSetup()
end

function RunMEpicIntro()
    -- if _G.MShotAlready then
    --     epicM:GetTransform().mRotate = Vec3.new(0, 130.013, 0)
    -- end 
    -- Always look at the player when midway through exploding
    Helper.LookAtTarget(epicM)
    --print(_G.MEpicIntroState)

    if _G.MEpicIntroState == 1 then -- Move to start pos
        if not MoveTo(player, MPos1, 100) then
            _G.MEpicIntroState = 2 -- To keep M waiting
        end
    elseif _G.MEpicIntroState == 2 then -- Make player look at front (MView1) (Make melissa scream when ended (attach script to it))
        if MAddScriptRunOnce then
            local x = player:GetTransform().mRotate.x
            if (x > -1) then x = x % 360
            else  x = x % (-360) end
            x = math.abs(x)
            if x > MViewLowerLimitx and x < MViewUpperLimitx then
                MAddScriptRunOnce = false
                epicM:GetScripts():AddScript(epicM, "..\\assets\\Scripts\\EpicIntroM.lua")
            end
        end
        
        if not LookTowardsInterpolation(player, MView1, 175) then _G.MEpicIntroState = 3 end

    elseif _G.MEpicIntroState == 3 then -- Zoom in
        if (not ZoomCamTo(player, MZoomVal, 100)) then 
            _G.MEpicIntroState = 4
        end
    elseif _G.MEpicIntroState == 4 then -- Wait timer for melissa to scream midway (Pause animation and audio)
        if epicM:GetAnimator():GetFrame() > 50 then 
            epicM:GetAnimator():PauseAnimation()
            _G.MEpicIntroState = 5 
        end
    elseif _G.MEpicIntroState == 5 then -- ShowInfo
        ShowInfoSlowdownCounter = ShowInfoSlowdownCounter + FPSManager.GetDT()

        if (ShowInfoSlowdownCounter < ShowInfoSlowdown) then MoveEpicIntroUI(epicIntroUI, 4.42, false, true)
        elseif not MoveEpicIntroUI(epicIntroUI, 0.01, false, true) and
               ShowInfoSlowdownCounter > ShowInfoMinTime then _G.MEpicIntroState = 6 end
    elseif _G.MEpicIntroState == 6 then -- HideInfo
        if not MoveEpicIntroUI(epicIntroUI, 4, false, false) then _G.MEpicIntroState = 7 end
    elseif _G.MEpicIntroState == 7 then -- zoom out and resume game
        if (not ZoomCamTo(player, defaultZoom, 100)) then
            _G.MEpicIntroState = 0
            _G.FreezePlayerControl = false
            retractBlackBorder = true
            epicM:GetAnimator():UnpauseAnimation()
            ShowUI()
            _G.completedEpicM = true
        end
    end
end

--#endregion


----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Helper functions
----------------------------------------------------------------------------
----------------------------------------------------------------------------

--#region Helper Functions

function GeneralSetup()
    insertBlackBorder = true
    _G.FreezePlayerControl = true
    ShowInfoSlowdownCounter = 0
    HideUI()
end

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
        mRotate.x = mRotate.x % (-360) + 360
    end

    local direction = Helper.Normalize(Vec3.new(targetPitchYaw.x - mRotate.x, targetPitchYaw.y - mRotate.y, 0))

    -- local newRotate = Helper.Vec3Add(mRotate, Helper.Scale(direction, speed * FPSManager:GetDT()))
    -- print(newRotate)
    -- _G.phySys:SetRotation(entity, newRotate)
-- print(targetPitchYaw)
print(mRotate)
    local toMove = Helper.Scale(direction, speed * FPSManager:GetDT())
    print(toMove)    
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

-- Return false when done
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

function RemoveEpicScript(Entity)
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
        Entity:GetScripts():DeleteScript(scriptName)
    end
end
--#endregion











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