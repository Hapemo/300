vec = Vec3.new(3,4,5)

function Alive()

end

function Update()

    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end

    physicsSys = systemManager:mPhysicsSystem()
    physicsSys:SetRotation(entity, vec)
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
