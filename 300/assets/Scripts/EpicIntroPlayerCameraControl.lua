-- This script controls the camera movement of the player for epic intro of enemies, uses global variables from EpicIntroManager
local this

-- starting camera direction (835,250)
-- first camera shift for horse
-- second camera shift for horse (-15, 5)

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then print("Entity nil in script!") end
end

function Update()
    local origin = Vec3.new()
    origin.x = 0.1
    LookTowards(origin, 1)
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end





function LookTowards(target, speed)
    local currPos = this:GetTransform().mTranslate
    local currDir = Helper.Vec3Minus(Camera_Scripting.GetTarget(), currPos)
    local targetDir = Helper.Vec3Minus(target, currPos)
    local upVector = Camera_Scripting.Cross(currDir, targetDir)

    -- local newDir = RotateVector(upVector, currDir, math.pi/180 * speed)
    -- local newDir = Camera_Scripting.Rotate(currDir, upVector, math.pi/180 * speed)
    Camera_Scripting.SetTarget(Helper.Vec3Add(newDir, currPos))
end

-- Rotate vector about normal in set amount of angle
function RotateVector(normal, vector, angle)
    local finalVector = Vec3.new()

    local alpha = math.sin(angle)
    local beta = math.cos(angle)
    local gamma = 1-beta
    local a = normal.x
    local b = normal.y
    local c = normal.z

    finalVector.x = vector.x*(gamma*a*a + beta) + vector.y*(gamma*a*b - alpha*c) + vector.z*(gamma*a*c + alpha*b)
    finalVector.y = vector.x*(gamma*a*b + alpha*c) + vector.y*(gamma*b*b + beta) + vector.z*(gamma*b*c - alpha*a)
    finalVector.z = vector.x*(gamma*a*c - alpha*b) + vector.y*(gamma*b*c + alpha*a) + vector.z*(gamma*c*c + beta)

    return finalVector
end

