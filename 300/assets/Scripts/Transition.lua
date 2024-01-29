local name = ""
local startedTransition = false

local minFilterRadius
local maxFilterRadius
local minExposure
local maxExposure

local changeStateTimer
local startStateTimer
local timeToNextState

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    graphicsSys = systemManager:mGraphicsSystem()

    startedTransition = false

    changeStateTimer = 0.0
    startStateTimer = 0.0
    timeToNextState = 2.0

    minExposure = 0.1
    minFilterRadius = 0.001
    maxExposure = 5.0
    maxFilterRadius = 0.35

    graphicsSys.FilterRadius = maxFilterRadius
    graphicsSys.mAmbientBloomExposure = maxExposure
end

function Update()
    if startStateTimer < timeToNextState and startedTransition == false then
        startStateTimer = startStateTimer + FPSManager.GetDT()
        graphicsSys.FilterRadius = (1.0 - (startStateTimer / timeToNextState)) * (maxFilterRadius - minFilterRadius) + minFilterRadius
        graphicsSys.mAmbientBloomExposure = (1.0 - (startStateTimer / timeToNextState)) * (maxExposure - minExposure) + minExposure
        if graphicsSys.FilterRadius < minFilterRadius then
            graphicsSys.FilterRadius = minFilterRadius
        end
        if graphicsSys.mAmbientBloomExposure < minExposure then
            graphicsSys.mAmbientBloomExposure = minExposure
        end
    end
    if startedTransition == true then
        changeStateTimer = changeStateTimer + FPSManager.GetDT() 
        graphicsSys.FilterRadius = (changeStateTimer / timeToNextState) * (maxFilterRadius - minFilterRadius) + minFilterRadius
        graphicsSys.mAmbientBloomExposure = (changeStateTimer / timeToNextState) * (maxExposure - minExposure) + minExposure

        if changeStateTimer >= timeToNextState then
            gameStateSys:ChangeGameState(name)
        end
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

function StartTransition()
    if startedTransition == true then return end
    startedTransition = true
end

function SetNextGameState(str)
    name = str
end