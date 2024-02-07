
local this
local shootingRangeDoor 
local doorTransform = Vec3.new()
local doorCollider

function Alive()
    -- Get Trigger Box (This Entity - holding this script)
    this = Helper.GetScriptEntity(script_entity.id)
    
    gameStateSys = systemManager:mGameStateSystem()
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

    print("PLAYER ENTERED THE ZONE") 
    doorTransform.mTranslate.y = 5.3
    Helper.SetTranslate(shootingRangeDoor, doorTransform.mTranslate)
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
    if(tagid == 0) then 
        print("PLAYER ENTERED THE ZONE")  
    end
end

function OnContactExit(Entity)

end


