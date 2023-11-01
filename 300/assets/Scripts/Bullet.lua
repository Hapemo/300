viewVec = Vec3.new(0,0,0)
movement = Vec3.new(0,0,0)

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
    entityobj = Helper.GetScriptEntity(script_entity.id)
    generalComponent = Entity:GetGeneral()
    tagid = generalComponent.tagid
    if (tagid == 1) then
        Entity:GetTransform().mScale.x = Entity:GetTransform().mScale.x * 0.9
        Entity:GetTransform().mScale.y = Entity:GetTransform().mScale.y * 0.9
        Entity:GetTransform().mScale.z = Entity:GetTransform().mScale.z * 0.9

        bullethitAudioSource:Play()
        bullethitAudioSource:SetVolume(0.2)

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
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end
