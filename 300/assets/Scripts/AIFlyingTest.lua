local vec = Vec3.new()

local deathAudioSource
local aiSys
local phySys
local this

local bobbleAngle
local bobbleFrequency
local bobbleIntensity

-- for example you want to reference out hp variable to another script
--local hp = 100

-- function GetHP()
--     return hp
-- end

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
    local x = this:GetAISetting():GetTarget():GetTransform().mTranslate.x - this:GetTransform().mTranslate.x
    local z = this:GetAISetting():GetTarget():GetTransform().mTranslate.z - this:GetTransform().mTranslate.z
    local distDiff = Helper.Vec2Len(x,z) - this:GetAISetting().mStayAway
    if -10 < distDiff and distDiff < 10 then
        vec.x = 0
        vec.z = 0
    end
    --
    phySys:SetVelocity(this, vec);
    --#endregion

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

end

function OnContactExit(Entity)

end
