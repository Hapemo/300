local timer

function Alive()

    -- testing timer countdown for the opacity change in the boss title ui
    timer = 0
    text = Helper.GetScriptEntity(script_entity.id)
    uir = text:GetUIrenderer()
    uir.mColor.w = 0 

end

function Update()

    text = Helper.GetScriptEntity(script_entity.id)
    uir = text:GetUIrenderer()

    timer = timer + FPSManager:GetDT()

    -- when the timer is more than 3s, decrease the opacity of the title
    if(timer >= 3) then
        if(uir.mColor.w > 0) then
            uir.mColor.w = uir.mColor.w - 0.01
        elseif(uir.mColor.w <= 0) then
            uir.mColor.w = 0 -- when it reaches minimum opacity, then cap it at the min
        end
    -- when the timer is less than 2s, increase the opacity of the title
    elseif (timer <= 2) then
        if(uir.mColor.w < 1.0) then
            uir.mColor.w = uir.mColor.w + 0.01
        elseif(uir.mColor.w >= 1.0) then
            uir.mColor.w = 1.0 -- when it reaches maximum opacity, then cap it at the max
        end
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


