local timercheck = 0
_G.activated = false
local running = false
local colors = {Vec4.new(0,0.7,1,1),Vec4.new(1,1,0,1),Vec4.new(1,0.4,0,1) }
function Alive()

end

function Update()
    ent = Helper.GetScriptEntity(script_entity.id)
    transform = ent:GetTransform()
    renderer  = ent:GetUIrenderer()

    if(_G.activated == true)then
    renderer.mColor =colors[_G.gunEquipped]
        renderer:SetDegree(0)
        running = true
        timercheck = 0
        _G.activated = false
    end


    if(running == true)then

        timercheck = timercheck +FPSManager.GetDT()*100

            if(timercheck <360)then
                renderer:SetDegree(timercheck)
            else
                _G.gunEquipped =1 
                timercheck = 0
                running = false
            end
    else
        renderer.mColor.w = 0
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
