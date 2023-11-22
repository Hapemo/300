local timer
local this
local maxTime
local eScale = Vec3.new()
local eTranslate = Vec3.new()
local originalTranslate = Vec3.new()
local originalTranslateX
local originalTranslateY
local originalTranslateZ
local originalScale = Vec3.new()
local scalePerSec
local finalTranslate = Vec3.new()
local amplitude
local period
local max
function Alive()
    --lifespan
    maxTime = 5.0
    timer = 0.0

    --randomness
    max = 2.0
    amplitude = math.random() + math.random(0, max)
    period = math.random() + math.random(0, max + 1.0)
    
    --transform
    this = Helper.GetScriptEntity(script_entity.id)
    originalTranslate = this:GetTransform().mTranslate
    originalTranslateX = originalTranslate.x
    originalTranslateY = originalTranslate.y
    originalTranslateZ = originalTranslate.z
    originalScale = this:GetTransform().mScale
    
    --scale changes per second
    scalePerSec = originalScale.x / maxTime
end

function Update()
    timer = FPSManager.GetDT() + timer
    
    --random movement
    eTranslate = this:GetTransform().mTranslate
    finalTranslate.x = originalTranslateX + amplitude * math.sin(period * timer)
    finalTranslate.y = originalTranslateY + amplitude * math.sin(period * timer) 
    finalTranslate.z = originalTranslateZ + amplitude * math.sin(period * timer) 
    eTranslate.x = finalTranslate.x
    eTranslate.y = finalTranslate.y
    eTranslate.z = finalTranslate.z

    
    --scale decrement
    scaleDecrement = FPSManager.GetDT() * scalePerSec
    eScale = this:GetTransform().mScale
    eScale.x = eScale.x - scaleDecrement
    eScale.y = eScale.y - scaleDecrement
    eScale.z = eScale.z - scaleDecrement

    if (eScale.x < 0) then
        systemManager.ecs:SetDeleteEntity(this)
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end
