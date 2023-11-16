local vec = Vec3.new()

local aiSys
local phySys
local this

local bobbleAngle
local bobbleFrequency
local bobbleIntensity

local triggerExplosionDistance

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();

    bobbleAngle = 0
    bobbleFrequency = 2
    bobbleIntensity = 20
    
    triggerExplosionDistance = 10
end

function Update()

    --#region Movement
    vec = aiSys:GetDirection(this)
    this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, vec)

    Helper.Scale(vec, 20)

    -- Add bobbbling here
    bobbleAngle = bobbleAngle + bobbleFrequency
    if bobbleAngle > 360 then bobbleAngle = 0 end
    vec.y = vec.y + math.cos(bobbleAngle/180*math.pi) * bobbleIntensity
    --

    -- If horizontal length is reached, don't move horizontally
    local targetPos = this:GetAISetting():GetTarget():GetTransform().mTranslate
    local thisPos = this:GetTransform().mTranslate
    local x = targetPos.x - thisPos.x
    local z = targetPos.z - thisPos.z
    local distDiff = Helper.Vec2Len(x,z) - this:GetAISetting().mStayAway
    if -10 < distDiff and distDiff < 10 then
        vec.x = 0
        vec.z = 0
    end
    --

    phySys:SetVelocity(this, vec);
    --#endregion
    

    -- #region Check should explode
    
    if Helper.Vec3Len(Helper.Vec3Minus(targetPos, thisPos)) < triggerExplosionDistance then
        Kamekazi()
    end

    -- #endregion

end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
    local tagid = Entity:GetGeneral().tagid
    print("contact enter")
    print(tagid)
    if (tagid == 0) then --player id
        print("Kamekazi")
        Kamekazi()
    end
end

function OnContactExit(Entity)

end


function Kamekazi()
    systemManager.ecs:NewEntityFromPrefab("Explosion", this:GetTransform().mTranslate)
    systemManager.ecs:SetDeleteEntity(this)
end



