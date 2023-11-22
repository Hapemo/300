local vec = Vec3.new()

local aiSys
local phySys
local this

local bobbleAngle
local bobbleFrequency
local bobbleIntensity

local targetPos
local thisPos

local AttackSpeed
local AttackTimer
local ShotSpeed

local inLineOfSight

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in script!")
    end
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();

    bobbleAngle = 0
    bobbleFrequency = 2
    bobbleIntensity = 10

    AttackSpeed = 1
    AttackTimer = 0
    ShotSpeed = 100
end

function Update()

    targetPos = this:GetAISetting():GetTarget():GetTransform().mTranslate
    thisPos = this:GetTransform().mTranslate
    inLineOfSight = aiSys:LineOfSight(this, this:GetAISetting():GetTarget())

    ILOVEYOUMovement()

    ILOVEYOUAttack()

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


function ILOVEYOUMovement() 
    --#region Movement
    vec = aiSys:GetDirection(this)
    this:GetTransform().mRotate.y = Helper.DirectionToAngle(this, vec)

    Helper.Scale(vec, 20)

    -- Add bobbling here
    bobbleAngle = bobbleAngle + bobbleFrequency
    if bobbleAngle > 360 then bobbleAngle = 0 end
    vec.y = vec.y + math.cos(bobbleAngle/180*math.pi) * bobbleIntensity
    --

    if inLineOfSight then
        -- If horizontal length is reached, don't move horizontally
        local x = targetPos.x - thisPos.x
        local z = targetPos.z - thisPos.z
        local distDiff = Helper.Vec2Len(x,z) - this:GetAISetting().mStayAway
        if -10 < distDiff and distDiff < 10 then
            vec.x = 0
            vec.z = 0
        end
    end
    --
    phySys:SetVelocity(this, vec);
    --#endregion
end


function ILOVEYOUAttack()
    if not inLineOfSight then return end -- No line of sight, don't attack
    AttackTimer = AttackTimer + FPSManager.GetDT()
    if AttackTimer > AttackSpeed then
        Shoot()
        AttackTimer = 0
    end
end

function Shoot()
    local bullet = systemManager.ecs:NewEntityFromPrefab("EnemyBullet", thisPos)
    -- local bulletVec = Helper.Scale(Helper.Normalize(Helper.Vec3Minus(targetPos, thisPos)), ShotSpeed)
    -- phySys:SetVelocity(bullet, bulletVec)
    -- aiSys:SetPredictiveVelocity(bullet, this:GetAISetting():GetTarget(), ShotSpeed)
    aiSys:PredictiveShootPlayer(bullet, ShotSpeed, 30, 50)
end


