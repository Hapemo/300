--local weaponPosition = {-1700,-1430,-1170,-900,-620,-360,-90,170 }

local weaponPosition = {180.0,-90.0,-360.0,-620.0,-900.0,-1170.0,-1430.0,-1700.0 }
local position_1 = Vec2.new(170,-820)
local pCount =1
local moveTime = 0.0
local ent
local transform
local general
local objecto
function Alive()

    ent = Helper.GetScriptEntity(script_entity.id)
    transform = ent:GetTransform()
    general = ent:GetGeneral()

    transform.mTranslate.y = position_1.y
    transform.mTranslate.x = position_1.x

    objecto = _G.obj
end

function Update()
    moveTime = moveTime + FPSManager.GetDT()

    transform = ent:GetTransform()
        if(moveTime >0.5)then
        --print("haaaaaaaaaaaahahahaah")


            if( _G.weaponArray[pCount+1][2] == true)then
                
                _G.weaponArray[pCount][2] = true
                systemManager.ecs:SetDeleteEntity(ent)

            end
            
            pCount = pCount+1
            transform.mTranslate.x = weaponPosition[pCount]
            --print(pCount.." object "..objecto.. " pos "..weaponPosition[pCount])
            moveTime = 0

        end
    
        if (pCount >8)then
            systemManager.ecs:SetDeleteEntity(ent)
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
