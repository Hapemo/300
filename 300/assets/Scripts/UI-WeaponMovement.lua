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
    --print(transform.mTranslate.z)


    transform.mTranslate.y = position_1.y
    transform.mTranslate.x = position_1.x

    objecto = _G.obj
end

function Update()
    moveTime = moveTime + FPSManager.GetDT()
    ent = Helper.GetScriptEntity(script_entity.id)
    transform = ent:GetTransform()
        if(moveTime >0.5)then
        --print("haaaaaaaaaaaahahahaah")


            if( _G.weaponArray[pCount+1][2] == true)then

                if ( _G.weaponArray[pCount+1][3] >2)then
                    _G.weaponArray[pCount][1] = transform.mTranslate.z
                    _G.weaponArray[pCount][2] = true
                    _G.weaponArray[pCount][3] = 1

                    print(1)
                    systemManager.ecs:SetDeleteEntity(ent)


                else
                    if (_G.weaponArray[pCount+1][1] ==transform.mTranslate.z )then
                        _G.weaponArray[pCount+1][3] =_G.weaponArray[pCount+1][3] +1
                        --print(_G.weaponArray[pCount+1][3].."xxxxxxx")
                        print(2)

                        systemManager.ecs:SetDeleteEntity(ent)
                    else
                        _G.weaponArray[pCount][1] = transform.mTranslate.z
                        _G.weaponArray[pCount][2] = true
                        _G.weaponArray[pCount][3] = 1
                        -- print(pCount.."count")
                        -- print(_G.weaponArray[pCount+1][3].."xxxxxxx")
                        -- print(3)
                        systemManager.ecs:SetDeleteEntity(ent)
                    end
                end

               -- if(_G.weaponArray[pCount+1][1] ==transform.mTranslate.z )



            end
            if (pCount ==8)then
                _G.weaponArray[pCount][1] = transform.mTranslate.z
                _G.weaponArray[pCount][2] = true
                _G.weaponArray[pCount][3] = 1
                systemManager.ecs:SetDeleteEntity(ent)
            end
            pCount = pCount+1
            transform.mTranslate.x = weaponPosition[pCount]
            --print(pCount.." object "..objecto.. " pos "..weaponPosition[pCount])
            moveTime = 0


        end
    
        if (pCount >=8)then
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
