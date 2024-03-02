local reArray = {8,7,6,5,4,3,2,1}
local index  = 0
local pos = 0
local unrend

function Alive()
    ent = Helper.GetScriptEntity(script_entity.id)
    transform = ent:GetTransform()
    uirend  = ent:GetUIrenderer()
    uirend.mColor.w = 0
    index = (transform.mTranslate.z +2)// 3
    pos = transform.mTranslate.z % 3
    
    if (pos == 0)then
        pos = 3
    end

     --print( index .. "index")
     --print( pos .. "pos")
end

function Update()
    if true then return end
    uirend  = ent:GetUIrenderer()
    if(_G.weaponArray[reArray[index]][2] == true)then
        if(pos <=_G.weaponArray[reArray[index]][3]  )then
            -- if(reArray[index] == 8 )then
            --     print("test")
            -- end
            uirend:SetTexture(_G.textureArray2[_G.weaponArray[reArray[index]][1]])
            uirend.mColor.w = 1

        else
            uirend.mColor.w = 0
        end
    else
        uirend.mColor.w = 0
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
