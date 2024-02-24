-- The main brain of epic intro for lv 1


_G.gameStateSys = systemManager:mGameStateSystem()
_G.inputMapSys = systemManager:mInputActionSystem()
_G.TrojanHorseStartToLedge = false
_G.TrojanHorseLedgeToFloor = false
_G.TrojanHorseFloorZoomIn = false
_G.TrojanHorseShowInfo = false
_G.TrojanHorseHideInfo = false
_G.TrojanHorseFloorZoomOut = false

_G.activateTrojanHorse = false
_G.activateTrojanSoldier = false
_G.activateMelissa = false
_G.activateILY = false
_G.activateZipBomb = false

function Alive()

end

function Update()
    if _G.activateTrojanHorse then
        RunTrojanHorseEpicIntro()
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


function RunTrojanHorseEpicIntro()
    if _G.TrojanHorsePhase1ToCameraIn then -- Finished landing on the ground, 
        
    end
end

