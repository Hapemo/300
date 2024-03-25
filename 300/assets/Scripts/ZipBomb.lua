local vec = Vec3.new()

local aiSys
local phySys
local gameStateSys
local this
local target

local bobbleAngle
local bobbleFrequency
local bobbleIntensity

local triggerExplosionDistance

local state
local explodingTimer = 1.0416 -- based on animation time
local explodingTimerCount = 0

local lineOfSight

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in script!")
    end
    aiSys = systemManager:mAISystem();
    phySys = systemManager:mPhysicsSystem();
    gameStateSys = systemManager:mGameStateSystem();

    this:GetMeshRenderer():SetMesh("ZipBomb_Idle", this) -- Start exploding animation 
    bobbleAngle = 0
    bobbleFrequency = 2
    bobbleIntensity = 0.5
    
    triggerExplosionDistance = 0.5
    
    state = ""

    this:GetAudio():SetPlay()
    target = this:GetAISetting():GetTarget()

    -- Change pathfinding graph to match that of the gamestate
    if (gameStateSys:GetCurrentGameState().mName == "Test") then
        this:GetAISetting().mGraphDataName = "FlyingPath"
    elseif (gameStateSys:GetCurrentGameState().mName == "Test2") then
        this:GetAISetting().mGraphDataName = "FlyingPath2"
    elseif (gameStateSys:GetCurrentGameState().mName == "Test3") then
        this:GetAISetting().mGraphDataName = "FlyingPath3"
    end
end

function Update()

    -- if systemManager:mInputActionSystem():GetButtonDown("Test4") then
    --     this:GetHealthbar().health = this:GetHealthbar().health - 10
    -- end

    --#region Movement
    lineOfSight = aiSys:LineOfSight(this, this:GetAISetting():GetTarget())
    vec = aiSys:GetDirection(this)

    Helper.Scale(vec, 5)

    if lineOfSight then
        local dir = Vec3.new()
        dir = Helper.Normalize(Helper.Vec3Minus(this:GetAISetting():GetTarget():GetTransform().mTranslate, this:GetTransform().mTranslate))
        Helper.SetRealRotate(this, Vec3.new(0, Helper.DirectionToAngle(this, dir), 0))
    else
        Helper.SetRealRotate(this, Vec3.new(0, Helper.DirectionToAngle(this, vec), 0))
    end

    if lineOfSight then 
        -- Add bobbbling here
        bobbleAngle = bobbleAngle + bobbleFrequency
        if bobbleAngle > 360 then bobbleAngle = 0 end
        vec.y = vec.y + math.cos(bobbleAngle/180*math.pi) * bobbleIntensity
        --
    end

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
            --print("kamekazi")
            gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetStop()
            Kamekazi() 
        end
        return
    end

    -- #region Check should explode
    if Helper.Vec3Len(Helper.Vec3Minus(targetPos, thisPos)) < triggerExplosionDistance then kamekazi = true end
    if this:GetHealthbar().health <= 0 then 
        StartExploding()
        if _G.Level3_Monsters ~= nil then 
            if _G.Level3_Monsters == true then 
                _G.number_left_in_level_3 = _G.number_left_in_level_3 - 1
            end
        end
    end


    -- #endregion

end

function Dead()
    this:GetAudio():SetStop()
end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
    if Entity == target then --player id
        StartExploding()
    end
end

function OnContactExit(Entity)

end


function Kamekazi()
    local explosion = systemManager.ecs:NewEntityFromPrefab("Explosion", this:GetTransform().mTranslate)
    Helper.Translate(explosion, Vec3.new(0,1,0))
    systemManager.ecs:SetDeleteEntity(this)
end

function StartExploding()
    state = "EXPLODING"
    --print("exploding")
    gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetPlay()
    this:GetMeshRenderer():SetMesh("Zip_exploding", this) -- Start exploding animation 
end
