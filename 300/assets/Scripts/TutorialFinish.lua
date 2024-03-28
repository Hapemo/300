function Alive()

end

function Update()
    print("hello")
end

function Dead()

end

function OnTriggerEnter(Entity)
        gameStateSys = systemManager:mGameStateSystem()
        inputMapSys = systemManager:mInputActionSystem()
        
            x = gameStateSys:GetEntity("TransitionHelper", "Transition") 
            y = x:GetScripts()
            z = y:GetScript("../assets/Scripts/Transition.lua")

            if z ~= nil then
                z:SetNextGameStateHelper("SetNextGameState", "Test")
                --z:RunFunctionWithParam("SetNextGameState", "Test2")
                z:RunFunction("StartTransition")
            end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


