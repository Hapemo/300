function Start()

end

function Update()
    --Working
    --print("Running from Run.lua !!")
    --systemManager.ecs:NewEntity()
    --systemManager.ecs:DeleteEntity(entity)
    
    --Testing
    generalEntities = systemManager.ecs:GetEntitiesWithGeneral();
    for i = 0, #generalEntities do
        print(generalEntities[i].id)
    end
end
