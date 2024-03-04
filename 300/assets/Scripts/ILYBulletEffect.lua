local vec = Vec3.new()

local aiSys
local phySys
local gameStateSys

local this

_G.ILYBulletHitPlayer = false

local alphaValue = 0
local startAlphaValue = 140
local speed = 120

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    if this == nil then
        print("Entity nil in script!")
    end

end

function Update()
    if _G.ILYBulletHitPlayer then 
        print("HIT PLAYER")
        _G.ILYBulletHitPlayer = false 
        alphaValue = startAlphaValue
    end

    if alphaValue > 0 then
        print(alphaValue)
        alphaValue = alphaValue - speed*FPSManager.GetDT()
        if alphaValue < 0 then alphaValue = 0 end
        this:GetUIrenderer().mColor.w = alphaValue/255
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

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end

