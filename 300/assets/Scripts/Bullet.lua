viewVec = Vec3.new(0,0,0)
movement = Vec3.new(0,0,0)
spawn = true
timer = 0
direction = Vec3.new(10,0,0)
colors = Vec4.new(0,0,0,1)
positions = Vec3.new(0,0,0)


color1 = Vec4.new(1,0,0,1)
color2 = Vec4.new(0,1,0,1)
color3 = Vec4.new(0,0,1,1)
color4 = Vec4.new(0,1,1,1)
color5 = Vec4.new(1,0,1,1)
color6 = Vec4.new(1,1,0,1)
color7 = Vec4.new(1,1,1,1)

allcolor = {Vec4.new(1,0,0,1),Vec4.new(0,1,0,1),Vec4.new(0,0,1,1),Vec4.new(0,1,1,1),Vec4.new(1,0,1,1),Vec4.new(1,1,0,1),Vec4.new(1,1,1,1)}

local entityobj


local bullethitEntity
local bullethitcomp
local bullethitAudioSource


function Alive()
    
end

function Update()

    gameStateSys = systemManager:mGameStateSystem()
    cameraEntity = gameStateSys:GetEntity("Camera", "testSerialization")
    positions = cameraEntity:GetTransform().mTranslate


    viewVec = Camera_Scripting.GetDirection(cameraEntity)
    physicsSys = systemManager:mPhysicsSystem()

    bullethitEntity = gameStateSys:GetEntity("Bullet Hit" , "testSerialization")
    bullethitcomp   = bullethitEntity:GetAudio()
    bullethitAudioSource = Helper.CreateAudioSource(bullethitEntity)


    -- movement.x= movement.x+movement.x
    --   movement.x = movement.x+ viewVec.x *0     
    --   movement.y = movement.y+ viewVec.y *0   
    --   movement.z = movement.z+ viewVec.z *0

    --   print(viewVec.x)
    --   print(viewVec.y)
    --   print(viewVec.z)
      
   
    
end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()
    entityobj = Helper.GetScriptEntity(script_entity.id)

    tagid = generalComponent.tagid
    if (tagid == 1) then
        Entity:GetTransform().mScale.x = Entity:GetTransform().mScale.x * 0.9
        Entity:GetTransform().mScale.y = Entity:GetTransform().mScale.y * 0.9
        Entity:GetTransform().mScale.z = Entity:GetTransform().mScale.z * 0.9

        bullethitAudioSource:Play()
        bullethitAudioSource:SetVolume(1.0)

        
        systemManager.ecs:SetDeleteEntity(entityobj)
    end

    if (tagid == 3) then
        --floorCount = floorCount + 1;
        -- gameStateSys = systemManager:mGameStateSystem();
        -- gameStateSys:DeleteEntity(entityobj)
        systemManager.ecs:SetDeleteEntity(entityobj)
    end
    if (tagid == 4) then
        --floorCount = floorCount + 1;
        -- gameStateSys = systemManager:mGameStateSystem();
        -- gameStateSys:DeleteEntity(entityobj)
        systemManager.ecs:SetDeleteEntity(entityobj)
    end


    if (tagid == 1) then
        for i = 7, 1, -1
        do
            spawned(i)
        end
        systemManager.ecs:SetDeleteEntity(entityobj)
    end

end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end


function spawned(value)

    direction.x = math.random(-5,5)
    direction.y = math.random(-5,5)
    direction.z = math.random(-5,5)
    
    total = (direction.x +direction.y +direction.z)
    -- colors.x = direction.x/5
    -- colors.y = direction.y/5
    -- colors.z = direction.z/5
    direction.x =( direction.x/total)*15
    direction.y = (direction.y/total)*15
    direction.z = (direction.z/total)*15
    positions = entityobj:GetTransform().mTranslate

    prefabEntity = systemManager.ecs:NewEntityFromPrefab("parti",positions )   

    meshSys = prefabEntity:GetMeshRenderer()
    meshSys:SetColor(allcolor[value])
    physicsSys = systemManager:mPhysicsSystem()
    physicsSys:SetVelocity(prefabEntity, direction)
end