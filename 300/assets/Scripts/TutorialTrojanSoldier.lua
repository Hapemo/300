
local this -- this object
local healthComponent
local generalComponent

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    generalComponent = this:GetGeneral()
    healthComponent = this:GetHealthbar()
    -- print("THIS IS CALLED" , generalComponent.name)
end

function Update()
    if(healthComponent ~= nil) then 
        -- print("NOT NIL")
        if(healthComponent.health <= 0) then 
            -- print("HELLO NO HEALTH LIAO")
            systemManager.ecs:SetDeleteEntity(this)
            -- _G.one_instance = false
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

function KillMe()
    gameStateSys = systemManager:mGameStateSystem()
    self = Helper.GetScriptEntity(script_entity.id)
    testScriptEntity = gameStateSys:GetEntity("Objectives")
    TestScripts = testScriptEntity:GetScripts()
    testScript = TestScripts:GetScript("../assets/Scripts/TutorialObjectives.lua")
    if testScript ~= nil then
        testScript:RunFunction("EnemyIsKilled")
    end
    systemManager.ecs:SetDeleteEntity(self)
end