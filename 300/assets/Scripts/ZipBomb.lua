local vec = Vec3.new()

local aiSys
local phySys
local gameStateSys
local this

local bobbleAngle
local bobbleFrequency
local bobbleIntensity

local triggerExplosionDistance

local state
local explodingTimer = 2
local explodingTimerCount = 0

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in script!")
    end
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();
    gameStateSys = systemManager:mGameStateSystem();

    bobbleAngle = 0
    bobbleFrequency = 2
    bobbleIntensity = 0.5
    
    triggerExplosionDistance = 0.5
    
    state = ""

    this:GetAudio():SetPlay()
end

function Update()

    if systemManager:mInputActionSystem():GetButtonDown("Test4") then
        this:GetHealthbar().health = this:GetHealthbar().health - 10
    end

    --#region Movement
    vec = aiSys:GetDirection(this)
    this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, vec)

    Helper.Scale(vec, 5)

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
    if -0.5 < distDiff and distDiff < 0.5 then
        vec.x = 0
        vec.z = 0
    end
    --

    phySys:SetVelocity(this, vec);
    --#endregion
    
    if state == "EXPLODING" then
        explodingTimerCount = explodingTimerCount + FPSManager.GetDT()
        if explodingTimerCount > explodingTimer then 
            gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetStop()
            Kamekazi() 
        end
        return
    end

    -- #region Check should explode
    local exploding = false
    if Helper.Vec3Len(Helper.Vec3Minus(targetPos, thisPos)) < triggerExplosionDistance then kamekazi = true end
    if this:GetHealthbar().health <= 0 then exploding = true end

    if exploding then 
        state = "EXPLODING"
        gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetPlay()
    end

    -- #endregion

end

function Dead()
    this:GetAudio():SetStop()
end

function OnTriggerEnter(Entity)
    local tagid = Entity:GetGeneral().tagid
    if (tagid == 0) then --player id
        Kamekazi()
    end
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end


function Kamekazi()
    systemManager.ecs:NewEntityFromPrefab("Explosion", this:GetTransform().mTranslate)
    systemManager.ecs:SetDeleteEntity(this)
end

