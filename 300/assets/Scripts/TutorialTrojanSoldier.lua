
local self
function Alive()

end

function Update()
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