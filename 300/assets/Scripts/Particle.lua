movement = Vec3.new(0,0,0)

speed = 5.0
deathtime = 0

function Alive()

end

function Update()
  --  physicsSys = systemManager:mPhysicsSystem()
  --  entity = Helper.GetScriptEntity(script_entity.id)
   -- physicsSys:SetVelocity(entity, direction)
    --positions = cameraEntity:GetTransform().mTranslate

    deathtime = deathtime + FPSManager.GetDT()
    if(deathtime >1.5)then
        entityobj = Helper.GetScriptEntity(script_entity.id)
        systemManager.ecs:SetDeleteEntity(entityobj)
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

