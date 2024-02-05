_G.PreObjectivesCounter = 0;
local objective1
local objective2
local objective3
function Alive()
    gameStateSys = systemManager:mGameStateSystem()

    -- objective1 =  gameStateSys:GetEntityByScene("Objectives1" , "testSerialization")
    -- objective2 =  gameStateSys:GetEntityByScene("Objectives2" , "testSerialization")
    -- objective3 =  gameStateSys:GetEntityByScene("Objectives3" , "testSerialization")

    objectives = gameStateSys:GetEntity("Objectives1")
end

function Update()
    --set >= 4 for now as there are 4 'A' enemies in the scene rn
    -- print("OBJECTIVE COUNTER: " , _G.PreObjectivesCounter)
    if _G.PreObjectivesCounter >= 4 then
        --spawn the objectives portals
        print("SPAWN PORTALS")
        objective1 = gameStateSys:GetEntity("Objectives1")
        objective2 = gameStateSys:GetEntity("Objectives2")
        objective3 = gameStateSys:GetEntity("Objectives3")
        local o1offset = Vec3.new()
        local o2offset = Vec3.new()
        local o3offset =Vec3.new()

        o1offset.x =objective1:GetTransform().mTranslate.x
        o1offset.y =-10.5
        o1offset.z =objective1:GetTransform().mTranslate.z


        o2offset.x =objective2:GetTransform().mTranslate.x
        o2offset.y =-10.5
        o2offset.z =objective2:GetTransform().mTranslate.z

        o3offset.x =objective3:GetTransform().mTranslate.x
        o3offset.y =-10.5
        o3offset.z =objective3:GetTransform().mTranslate.z


        

        -- objective1:GetTransform().mTranslate.y = -10.5
        -- objective2:GetTransform().mTranslate.y = -10.5
        -- objective3:GetTransform().mTranslate.y = -10.5

        Helper.SetTranslate(objective1,o1offset)
        Helper.SetTranslate(objective2,o2offset)
        Helper.SetTranslate(objective3,o3offset)
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


