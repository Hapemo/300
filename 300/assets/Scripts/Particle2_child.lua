movement = Vec3.new(0,0,0)

speed = 5.0
deathtime = 0
spawned = true
deaths = {3111,2111,4111,5111}
randomdeath = 1
local eTranslate = Vec3.new()
local thisEnt
local direction
direction = Vec3.new()

function Alive()
    direction.x = math.random(-5,5)
    direction.y = math.random(-5,5)
    direction.z = math.random(-5,5)
    -- colors.x = direction.x/5
    -- colors.y = direction.y/5
    -- colors.z = direction.z/5
    direction.x =( direction.x/10)*50
    direction.y = (direction.y/10)*50
    direction.z = (direction.z/10)*50
    thisEnt = Helper.GetScriptEntity(script_entity.id)
end

function Update()
    
    if(spawned == true)then
        randomdeath = math.random(1,6)
    end

    dt = FPSManager.GetDT()
    eTranslate = thisEnt:GetTransform().mTranslate
    eTranslate.x = dt * direction.x + eTranslate.x
    eTranslate.y = dt * direction.y + eTranslate.y
    eTranslate.z = dt * direction.z + eTranslate.z
    deathtime = deathtime + dt
    if(deathtime > randomdeath)then
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

