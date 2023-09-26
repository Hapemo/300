function Alive()

end

function Start()

end

function Update()
    --Working
    --print("Running from Run.lua !!")
    --systemManager.ecs:NewEntity()
    --systemManager.ecs:DeleteEntity(entity)
    
    --Testing
    generalEntities = systemManager.ecs:GetGeneralComponent();
    for i = 0, #generalEntities do
        print(generalEntities[i].id, generalEntities[i])
    end
end

function Exit()

end

function Dead()

end
