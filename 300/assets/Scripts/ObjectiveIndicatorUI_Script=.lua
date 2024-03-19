local currentCounter = 0.0
local multiplier = 3.0

local originalScaleX = 0.0
local originalScaleY = 0.0
_G.ObjectiveIndicatorUI_Texture = "default"

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    transform = this:GetTransform()
    originalScaleX = transform.mScale.x
    originalScaleY = transform.mScale.y
end

function Update()
    this = Helper.GetScriptEntity(script_entity.id)
    transform = this:GetTransform()
    currentCounter = currentCounter + FPSManager:GetDT()

    local sinewave = math.sin(currentCounter * multiplier) * 0.1 + 0.9

    --print(_G.ObjectiveIndicatorUI_Texture)
    if(_G.ObjectiveIndicatorUI_Texture == "Installing")
    then
        transform.mScale.x = (originalScaleX / 2) * sinewave
        transform.mScale.y = originalScaleY * sinewave
    else
        transform.mScale.x = originalScaleX * sinewave
        transform.mScale.y = originalScaleY * sinewave
    end
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


