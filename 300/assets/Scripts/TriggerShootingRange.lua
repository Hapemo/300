
local this
local shootingRangeDoor 
local doorTransform = Vec3.new()
local doorCollider

function Alive()
    -- Get Trigger Box (This Entity - holding this script)
    this = Helper.GetScriptEntity(script_entity.id)
    
    gameStateSys = systemManager:mGameStateSystem()
    graphicsSys = systemManager:mGraphicsSystem()
    shootingRangeDoor = gameStateSys:GetEntity("ShootingRangeDoor")
    doorTransform = shootingRangeDoor:GetTransform()
    doorCollider = shootingRangeDoor:GetBoxCollider()
    -- print("TRIGGER SHOOTING RANGE INIT")
end

function Update()
    
end

function Dead()

end

function OnTriggerEnter(Entity)

    -- print("PLAYER ENTERED THE ZONE") 
    -- Shutting Entrance Logic (into Shooting Range)
    doorTransform.mTranslate.y = 5.09
    Helper.SetTranslate(shootingRangeDoor, doorTransform.mTranslate)

    -- Trigger GUI
    _G.teachWeaponToggle = true -- interacts with logic in "TriggerTutorialGUI.lua" attached to [Entity - "TutorialGUIController"]

    if(_G.Tutorial_Phase_1_Activate == false) then 
        graphicsSys:HideCursor(false)
    end
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
    if(tagid == 0) then 
        -- print("PLAYER ENTERED THE ZONE")  
    end
end

function OnContactExit(Entity)

end


