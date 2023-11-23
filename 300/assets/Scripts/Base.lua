_G.obj =1
local position_1 = Vec3.new(180,-820,0)
_G.weaponArray = {{1,false,0},{1,false,0},{1,false,0},{1,false,0},{1,false,0},{1,false,0},{1,false,0},{1,true,1}}
local weaponPosition = {-1700,-1430,-1170,-900,-620,-360,-90,180 }
local dispensor = {1,1,1}
local disCount = 3
local dashui = { }
_G.textureArray  = {"UI-Revolver" , "UI-Shotgun-1223341","UI-Assult-133123" }
_G.textureArray2 = {"UI-Blue","UI-Yellow-s3&321",  "UI-Red"}

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

        if (disCount <3)then
        --table.insert(dispensor,1)
        dispensor[disCount] = math.random(1,3)
        --print(dispensor[disCount])
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

               --local newpos = position_1
                math.randomseed(os.time())
                local randn = math.random(1,3)
                dEntity = systemManager.ecs:NewEntityFromPrefab("UI-Spawned", position_1)
                uirend  = dEntity:GetUIrenderer()
                dtransform = dEntity:GetTransform()
                dtransform.mTranslate.z = randn
                -- uirend:SetTexture(_G.textureArray[dispensor[disCount]])
                uirend:SetTexture(_G.textureArray[randn])
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
        _G.weaponArray[8][3] = 0
        _G.gunEquipped = _G.weaponArray[8][1]
        _G.activated = true
        ArraySystem(8)

    elseif(Input.CheckKey(State.PRESS,Key.KEY_2))then
        _G.weaponArray[7][2] = false
        _G.weaponArray[7][3] = 0
        _G.gunEquipped = _G.weaponArray[7][1]
        _G.activated = true
        ArraySystem(7)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_3))then
        _G.weaponArray[6][2] = false
        _G.weaponArray[6][3] = 0
        _G.gunEquipped = _G.weaponArray[6][1]
        _G.activated = true
        ArraySystem(6)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_4))then
        _G.weaponArray[5][2] = false
        _G.weaponArray[5][3] = 0
        _G.gunEquipped = _G.weaponArray[5][1]
        _G.activated = true
        ArraySystem(5)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_5))then
        _G.weaponArray[4][2] = false
        _G.weaponArray[4][3] = 0
        _G.gunEquipped = _G.weaponArray[4][1]
        _G.activated = true
        ArraySystem(4)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_6))then
        _G.weaponArray[3][2] = false
        _G.weaponArray[3][3] = 0
        _G.gunEquipped = _G.weaponArray[3][1]
        _G.activated = true
        ArraySystem(3)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_7))then
        _G.weaponArray[2][2] = false
        _G.weaponArray[2][3] = 0
        _G.gunEquipped = _G.weaponArray[2][1]
        _G.activated = true
        ArraySystem(2)
    elseif(Input.CheckKey(State.PRESS,Key.KEY_8))then
        _G.weaponArray[1][2] = false
        _G.weaponArray[1][3] = 0
        _G.gunEquipped = _G.weaponArray[1][1]
        _G.activated = true
        ArraySystem(1)
    end
    -- for i = 8, 2 , -1
    -- do

    -- print("ui-".. _G.weaponArray[i][3].."index-"..i)

    -- end
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
    local rearrange = 0
    
     for i = index, 2 , -1
     do
        if( _G.weaponArray[i-1][2] == true)then

                _G.weaponArray[i][1]= _G.weaponArray[i-1][1]
                _G.weaponArray[i][2]= _G.weaponArray[i-1][2]
                _G.weaponArray[i][3]= _G.weaponArray[i-1][3]
                _G.weaponArray[i-1][2] = false
        end
    end

    -- if( rearrange > 0)then

    --     for i = rearrange, 1 , -1
    --     do

    --      --   arrayArrange()

    --     end

    -- end

end

--  function arrayArrange(index)

--     for i = index, 2 , -1
--     do
--         if( _G.weaponArray[i-1][2] == true)then

--                 _G.weaponArray[i][1]= _G.weaponArray[i-1][1]
--                 _G.weaponArray[i][2]= _G.weaponArray[i-1][2]
--                 _G.weaponArray[i][3]= _G.weaponArray[i-1][3]
--                 _G.weaponArray[i-1][2] = false
--         end
--     end

--  end


function DisplayArray()
    for i = 1, 8 , 1
    do
        if(_G.weaponArray[i][2] == true)then
            uirender = dashui[i]:GetUIrenderer()
            uirender:SetTexture(_G.textureArray[_G.weaponArray[i][1]])      
            uirender:SetDegree(0)

        else
            uirender = dashui[i]:GetUIrenderer()
            uirender:SetDegree(360)
        end

    end
end

function random()


end