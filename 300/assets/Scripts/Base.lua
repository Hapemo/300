_G.obj =1
local position_1 = Vec3.new(180,-820,0)
_G.weaponArray = {{0,false},{0,false},{0,false},{0,false},{0,false},{0,false},{0,false},{1,true}}
local weaponPosition = {-1700,-1430,-1170,-900,-620,-360,-90,180 }
local dispensor = {}
local disCount = 3
local dashui = { }

local spawnTime = 0
local dispenseTime = 0
function Alive()
    ent = Helper.GetScriptEntity(script_entity.id)
    gameStateSys = systemManager:mGameStateSystem()
    inputMapSys = systemManager:mInputActionSystem()
    dashui[1] = gameStateSys:GetEntity("P8", "UI")
    dashui[2] = gameStateSys:GetEntity("P7", "UI")
    dashui[3] = gameStateSys:GetEntity("P6", "UI")
    dashui[4] = gameStateSys:GetEntity("P5", "UI")
    dashui[5] = gameStateSys:GetEntity("P4", "UI")
    dashui[6] = gameStateSys:GetEntity("P3", "UI")
    dashui[7] = gameStateSys:GetEntity("P2", "UI")
    dashui[8] = gameStateSys:GetEntity("P1", "UI")
end

function Update()

    -- for simulating random orb picked up
    spawnTime = spawnTime +  FPSManager.GetDT()
    if(spawnTime > 1)then

        if (disCount <2)then
        table.insert(dispensor,1)
        disCount = disCount+1
        spawnTime =0
        end
    end    

    if (disCount >0) then
        dispenseTime = dispenseTime +  FPSManager.GetDT()
        --print(dispenseTime)
        --print("discount".. disCount)
        if(_G.weaponArray[1][2] == true)then
            disCount =0
            dispenseTime = 0
        else
            if(dispenseTime > 1)then
               -- print("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")
                dEntity = systemManager.ecs:NewEntityFromPrefab("UI-Spawned", position_1)
                -- _G.obj = _G.obj+1

                disCount = disCount-1
                dispenseTime = 0
            end
        end
    else
        dispenseTime =0
    end



    -- 3 = HOLD
    --49 = keycode for number '0'
    if(Input.CheckKey(State.PRESS,Key.KEY_1))then
        _G.weaponArray[8][2] = false
        ArraySystem(8)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_2))then
        _G.weaponArray[7][2] = false
        ArraySystem(7)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_3))then
        _G.weaponArray[6][2] = false
        ArraySystem(6)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_4))then
        _G.weaponArray[5][2] = false
        ArraySystem(5)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_5))then
        _G.weaponArray[4][2] = false
        ArraySystem(4)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_6))then
        _G.weaponArray[3][2] = false
        ArraySystem(3)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_7))then
        _G.weaponArray[2][2] = false
        ArraySystem(2)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_8))then
        _G.weaponArray[1][2] = false
        ArraySystem(1)
    end

    -- if(Input.CheckKey(State.HOLD,Key.KEY_1))then
    --     if(_G.weaponArray[1][2] == false)then
    --         --print("richmondisgay")
    --         dashrender = dashui[3]:GetUIrenderer()
    --         dashrender:SetDegree(360)
    --     end
    -- end

    DisplayArray()

end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end

function ArraySystem(index)

     for i = index, 2 , -1
     do

        if( _G.weaponArray[i-1][2] == true)then
        --print("richmondisgay"..i)

             _G.weaponArray[i][1]= _G.weaponArray[i-1][1]
             _G.weaponArray[i][2]= _G.weaponArray[i-1][2]
            _G.weaponArray[i-1][2] = false


        end
     end
end

function DisplayArray()
    for i = 1, 8 , 1
    do
        if(_G.weaponArray[i][2] == true)then
            uirender = dashui[i]:GetUIrenderer()
            uirender:SetDegree(0)

        else
            uirender = dashui[i]:GetUIrenderer()
            uirender:SetDegree(360)
        end

    end
end

function random()


end