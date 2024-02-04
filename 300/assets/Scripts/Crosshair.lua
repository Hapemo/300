local d_position = Vec2.new(-102,102)
local m_position = Vec2.new(-202,202)
local de_speed = 0.05
local add_speed = 1
local ent
local inputMapSys
local transform
local general

function Alive()
    ent = Helper.GetScriptEntity(script_entity.id)
    inputMapSys = systemManager:mInputActionSystem();
end

function Update()


    transform = ent:GetTransform()
    general = ent:GetGeneral()


    if(inputMapSys:GetButton("Shoot")) then
        
        if  (general.name == "crosshair-1")then
            if(transform.mTranslate.x > -0.1)then
                transform.mTranslate.x =  transform.mTranslate.x -de_speed
                transform.mTranslate.y =  transform.mTranslate.y +de_speed
            end
        elseif (general.name == "crosshair-2")then
            if(transform.mTranslate.x < 0.1)then
                transform.mTranslate.x =  transform.mTranslate.x +de_speed
                transform.mTranslate.y =  transform.mTranslate.y +de_speed
            end

        elseif (general.name == "crosshair-3")then
            if(transform.mTranslate.x < 0.1)then
                transform.mTranslate.x =  transform.mTranslate.x +de_speed
                transform.mTranslate.y =  transform.mTranslate.y -de_speed
            end
        elseif (general.name == "crosshair-4")then
            if(transform.mTranslate.x > -0.1)then
                transform.mTranslate.x =  transform.mTranslate.x -de_speed
                transform.mTranslate.y =  transform.mTranslate.y -de_speed
            end
        end
    else
        if  (general.name == "crosshair-1")then
            if(transform.mTranslate.x  < -0.1)then
                transform.mTranslate.x =  transform.mTranslate.x +de_speed
                transform.mTranslate.y =  transform.mTranslate.y -de_speed
            end

        elseif (general.name == "crosshair-2")then
            if(transform.mTranslate.x > 0.1)then
                transform.mTranslate.x =  transform.mTranslate.x -de_speed
                transform.mTranslate.y =  transform.mTranslate.y -de_speed
            end
        elseif (general.name == "crosshair-3")then
            if(transform.mTranslate.x > 0.1)then
                transform.mTranslate.x =  transform.mTranslate.x -de_speed
                transform.mTranslate.y =  transform.mTranslate.y +de_speed
            end
        elseif (general.name == "crosshair-4")then
            if(transform.mTranslate.x < -0.1)then
                transform.mTranslate.x =  transform.mTranslate.x +de_speed
                transform.mTranslate.y =  transform.mTranslate.y +de_speed
            end
        end

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
