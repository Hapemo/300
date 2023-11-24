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

function Helper.protect(tbl)
    return setmetatable({}, {
        __index = tbl,
        __newindex = function(t, key, value)
            error("attempting to change constant " ..
                   tostring(key) .. " to " .. tostring(value), 2)
        end
    })
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

function Helper.SetTranslate(Entity, Vec3)
    transformComponent = Entity:GetTransform()
    transformComponent.mTranslate.x = Vec3.x
    transformComponent.mTranslate.y = Vec3.y
    transformComponent.mTranslate.z = Vec3.z
    vec = Vec3.new()
    vec.x = transformComponent.mTranslate.x;
    vec.y = transformComponent.mTranslate.y;
    vec.z = transformComponent.mTranslate.z;
    physicsSys = systemManager:mPhysicsSystem();
    physicsSys:SetPosition(Entity, vec);
end

function Helper.SetRotate(Entity, Vec3)
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

function Helper.Normalize(Vec3)
    magnitude = (Vec3.x ^ 2 + Vec3.y ^ 2 + Vec3.z ^ 2) ^ 0.5
    Vec3.x = Vec3.x / magnitude
    Vec3.y = Vec3.y / magnitude
    Vec3.z = Vec3.z / magnitude
    return Vec3
end

function Helper.Vec3Minus(v1, v2)
    local v = Vec3.new()
    v.x = v1.x - v2.x
    v.y = v1.y - v2.y
    v.z = v1.z - v2.z
    return v
end

function Helper.Scale(Vec3, scale)
    Vec3.x = Vec3.x * scale
    Vec3.y = Vec3.y * scale
    Vec3.z = Vec3.z * scale
    return Vec3
end

function Helper.Vec3Len(vec)
    local len = math.sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z)
    return len
end

function Helper.Vec2Len(x, y)
    local len = math.sqrt(x * x + y * y)
    return len
end

function Helper.DirectionToAngle(entity, vec)
    if (Helper.Vec3Len(vec) == 0) then return entity:GetTransform().mRotate.y end

    local firstvec = Vec3.new()
    local secondvec = Vec3.new()
    firstvec.x = 0.0
    firstvec.y = 0.0
    firstvec.z = 1.0
    secondvec.x = vec.x
    secondvec.y = 0
    secondvec.z = vec.z

    local dotDivideMag = vec.z / math.sqrt(vec.x * vec.x + vec.z * vec.z)
    local radians = math.acos(dotDivideMag)
    local degree = radians * 180.0 / 3.141596
    if (vec.x < 0) then
        degree = 360.0 - degree
    end

    if degree ~= degree then return entity:GetTransform().mRotate.y end

    return degree
end

function Helper.CreateSphereParticle(vec)
    systemManager.ecs:NewEntityFromPrefab("SphereParticle", vec)   
end
function Helper.SetFloatVarFromOtherScript_Scene(entityName, sceneName, scriptName, varName, varValue)
    gameStateSys = systemManager:mGameStateSystem()
    e = gameStateSys:GetEntity(entityName , sceneName)
    scriptingComp = e:GetScripts()
    script = scriptingComp:GetScript(scriptName)
    script:SetValueFloat(varName, varValue)
end

function Helper.CreateTrailParticle(vec)
    systemManager.ecs:NewEntityFromPrefab("TrailParticle", vec)
end