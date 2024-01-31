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



    gameStateSys = systemManager:mGameStateSystem()

    dd = gameStateSys:GetEntity("P8", "UI")
    transform = dd:GetTransform()
    position_1.x = transform.mTranslate.x
    position_1.y = transform.mTranslate.y

    

    dashui1 = gameStateSys:GetEntity("P8", "UI")
    weaponPosition[1] = dashui1:GetTransform().mTranslate.x
    dashui2 = gameStateSys:GetEntity("P7", "UI")
    weaponPosition[2] = dashui2:GetTransform().mTranslate.x
    dashui3 = gameStateSys:GetEntity("P6", "UI")
    weaponPosition[3] = dashui3:GetTransform().mTranslate.x
    dashui4 = gameStateSys:GetEntity("P5", "UI")
    weaponPosition[4] = dashui4:GetTransform().mTranslate.x
    dashui5 = gameStateSys:GetEntity("P4", "UI")
    weaponPosition[5] = dashui5:GetTransform().mTranslate.x
    dashui6 = gameStateSys:GetEntity("P3", "UI")
    weaponPosition[6] = dashui6:GetTransform().mTranslate.x
    dashui7 = gameStateSys:GetEntity("P2", "UI")
    weaponPosition[7] = dashui7:GetTransform().mTranslate.x

    dashui8 = gameStateSys:GetEntity("P1", "UI")
    weaponPosition[8] = dashui8:GetTransform().mTranslate.x
    -- print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
    -- print(dashui:GetTransform().mTranslate.x)

    -- dashui[2] = gameStateSys:GetEntity("P7", "UI")
    -- dashui[3] = gameStateSys:GetEntity("P6", "UI")
    -- dashui[4] = gameStateSys:GetEntity("P5", "UI")
    -- dashui[5] = gameStateSys:GetEntity("P4", "UI")
    -- dashui[6] = gameStateSys:GetEntity("P3", "UI")
    -- dashui[7] = gameStateSys:GetEntity("P2", "UI")
    -- dashui[8] = gameStateSys:GetEntity("P1", "UI")


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
        if(pCount < 8)then
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
        else

            if( _G.weaponArray[8][2] == false )then
                _G.weaponArray[8][1] = transform.mTranslate.z
                _G.weaponArray[8][2] = true
                _G.weaponArray[8][3] = 1
                systemManager.ecs:SetDeleteEntity(ent)
            end   
        end

 
            
        pCount = pCount+1
        transform.mTranslate.x = weaponPosition[pCount]
        --print(pCount.." object "..objecto.. " pos "..weaponPosition[pCount])
        moveTime = 0
        -- else
        --     if( _G.weaponArray[8][2] == true)then
        --         _G.weaponArray[8][1] = transform.mTranslate.z
        --         _G.weaponArray[8][2] = true
        --         _G.weaponArray[8][3] = 1
        --         systemManager.ecs:SetDeleteEntity(ent)
        --     end
        --     systemManager.ecs:SetDeleteEntity(ent)
        -- end
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
