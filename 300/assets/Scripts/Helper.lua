Helper = {}

Helper.test = 5

function Helper.printTest()
    print("HELPER WORKS!") 
end

-- Make entity obj of entity that is attached to script
function Helper.GetScriptEntity(id)
    entity = Entity.new(id)
    return entity
end

-- Make AudioSource object
function Helper.CreateAudioSource(Entity)
    audioEntity = AudioSource.new();
    audioEntity:GetAudio(Entity);
    return audioEntity
end

function Helper.Translate(Entity, Vec3)
    transformComponent = Entity:GetTransform()
    transformComponent.mTranslate.x = transformComponent.mTranslate.x + Vec3.x
    transformComponent.mTranslate.y = transformComponent.mTranslate.y + Vec3.y
    transformComponent.mTranslate.z = transformComponent.mTranslate.z + Vec3.z
    vec = Vec3.new()
    vec.x = transformComponent.mTranslate.x;
    vec.y = transformComponent.mTranslate.y;
    vec.z = transformComponent.mTranslate.z;
    physicsSys = systemManager:mPhysicsSystem();
    physicsSys:SetPosition(Entity, vec);
end

function Helper.Rotate(Entity, Vec3)
    transformComponent = Entity:GetTransform()
    transformComponent.mRotate.x = transformComponent.mRotate.x + Vec3.x
    transformComponent.mRotate.y = transformComponent.mRotate.y + Vec3.y
    transformComponent.mRotate.z = transformComponent.mRotate.z + Vec3.z
    vec = Vec3.new()
    vec.x = transformComponent.mRotate.x;
    vec.y = transformComponent.mRotate.y;
    vec.z = transformComponent.mRotate.z;
    physicsSys = systemManager:mPhysicsSystem();
    physicsSys:SetRotation(Entity, vec);
end
