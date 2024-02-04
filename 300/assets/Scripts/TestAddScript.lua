function Alive()

end

function Update()
    gameStateSys = systemManager:mGameStateSystem();

    cameraEntity = gameStateSys:GetEntity("CreditsButton")
    if cameraEntity == nil then
        print("Entity nil in script!")
    end

    cameraEntity:GetScripts():AddScript(cameraEntity, "../assets\\Scripts\\TestAddScript.lua")
    cameraEntity:GetScripts():AddScript(cameraEntity, "../assets\\Scripts\\Print.lua")
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


