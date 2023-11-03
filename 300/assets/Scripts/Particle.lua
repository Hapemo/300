movement = Vec3.new(0,0,0)

speed = 5.0
deathtime = 0

spawned = true
deaths = {0.3,0.4,0.5,0.6}
randomdeath = 1

function Alive()

end

function Update()

    if(spawned == true)then
        randomdeath = math.random(1,4)
    end
        
  --  physicsSys = systemManager:mPhysicsSystem()
  --  entity = Helper.GetScriptEntity(script_entity.id)
   -- physicsSys:SetVelocity(entity, direction)
    --positions = cameraEntity:GetTransform().mTranslate

    deathtime = deathtime + FPSManager.GetDT()
    if(deathtime >deaths[randomdeath])then
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

