local destroyHeight = -10 -- height to destroy the object
local target = Vec3.new(0,0,0)

function Alive()
end

function Update()
    gameStateSys = systemManager:mGameStateSystem()
    ent = Helper.GetScriptEntity(script_entity.id)
    transform = ent:GetTransform()

    transform.mTranslate.y = transform.mTranslate.y - 1.0
    
    if(transform.mTranslate.y < destroyHeight)then
        systemManager.ecs:SetDeleteEntity(ent)
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


