local vec = Vec3.new()

local gameStateSys
local this
local state
local explodingTimer = 1.0416 -- based on animation time
local explodingTimerCount = 0

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in script!")
    end
    gameStateSys = systemManager:mGameStateSystem();

    state = ""

    this:GetAudio():SetPlay()
    StartExploding()
end

function Update()
    local dir = Vec3.new()
    dir = Helper.Normalize(Helper.Vec3Minus(this:GetAISetting():GetTarget():GetTransform().mTranslate, this:GetTransform().mTranslate))
    Helper.SetRealRotate(this, Vec3.new(0, Helper.DirectionToAngle(this, dir), 0))
    if _G.ZBEpicIntroState > 5 then return end
    if state == "EXPLODING" then
        explodingTimerCount = explodingTimerCount + FPSManager.GetDT()
        if explodingTimerCount > explodingTimer then 
            --print("kamekazi")
            gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetStop()
            Kamekazi() 
        end
        return
    end
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
    
end

function OnContactExit(Entity)

end


function Kamekazi()
    systemManager.ecs:NewEntityFromPrefab("Explosion", this:GetTransform().mTranslate)
    systemManager.ecs:SetDeleteEntity(this)
end

function StartExploding()
    state = "EXPLODING"
    --print("exploding")
    gameStateSys:GetEntity("ZipBombFuseAudio"):GetAudio():SetPlay()
    this:GetMeshRenderer():SetMesh("Zip_exploding", this) -- Start exploding animation 
end
