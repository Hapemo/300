_G.PreObjectivesCounter = 0;
local objective1
local objective2
local objective3
function Alive()
    gameStateSys = systemManager:mGameStateSystem()
end

function Update()
    --set >= 4 for now as there are 4 'A' enemies in the scene rn
    -- print("OBJECTIVE COUNTER: " , _G.PreObjectivesCounter)
    --if _G.PreObjectivesCounter >= 4 then

    if(_G.completedEpicTH == true and _G.completedEpicTS == true and _G.completedEpicILY == true) then
        gameStateSys = systemManager:mGameStateSystem();
        testScriptEntity = gameStateSys:GetEntity("Controller")
        TestScripts = testScriptEntity:GetScripts()
        testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")
        objCount = testScript:ReturnValueInt("GetCountObj")
            TestScripts = testScriptEntity:GetScripts()
            testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")
            objCount = testScript:ReturnValueInt("GetCountObj")
            TestScripts = testScriptEntity:GetScripts()
            testScript = TestScripts:GetScript("../assets/Scripts/ObjectivesController.lua")
            objCount = testScript:ReturnValueInt("GetCountObj")

        --spawn the objectives portals
        -- print("SPAWN PORTALS")
        objective1 = gameStateSys:GetEntity("Objectives1")
        objective2 = gameStateSys:GetEntity("Objectives2")
        objective3 = gameStateSys:GetEntity("Objectives3")

        local o1offset = Vec3.new()
        local o2offset = Vec3.new()
        local o3offset =Vec3.new()
        -- use the counter to raise the objective platforms
        if testScript ~= nil then
            if objCount == 3 then
                o1offset.x = objective1:GetTransform().mTranslate.x
                o1offset.y = -10.5
                o1offset.z = objective1:GetTransform().mTranslate.z
                Helper.SetTranslate(objective1,o1offset)
            end
            if objCount == 2 then
                o2offset.x = objective2:GetTransform().mTranslate.x
                o2offset.y = -10.5
                o2offset.z = objective2:GetTransform().mTranslate.z
                Helper.SetTranslate(objective2,o2offset)
            end
            if objCount == 1 then
                o3offset.x = objective3:GetTransform().mTranslate.x
                o3offset.y = -10.5
                o3offset.z = objective3:GetTransform().mTranslate.z
                Helper.SetTranslate(objective3,o3offset)
            end
        end
    end

    -- objective1:GetTransform().mTranslate.y = -10.5
    -- objective2:GetTransform().mTranslate.y = -10.5
    -- objective3:GetTransform().mTranslate.y = -10.5

    -- Helper.SetTranslate(objective1,o1offset)
    -- Helper.SetTranslate(objective2,o2offset)
    -- Helper.SetTranslate(objective3,o3offset)
    --end
    -- objective3:GetTransform().mTranslate.y = -10.5

    -- Helper.SetTranslate(objective1,o1offset)
    -- Helper.SetTranslate(objective2,o2offset)
    -- Helper.SetTranslate(objective3,o3offset)
    --end
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


