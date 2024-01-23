local i_pos = Vec3.new(-360,-590,0);
local e_pos= Vec3.new(-1700,-590,0);
local timercheck = 0
local indexing = 1
local poschecker = {-360,-625,-895,-1150,-1430,-1700}



function Alive()

end

function Update()
    ent = Helper.GetScriptEntity(script_entity.id)
    transform = ent:GetTransform()
    renderer  = ent:GetUIrenderer()
    
    timercheck = timercheck +FPSManager.GetDT()


    if(timercheck > 0.2 and timercheck <0.4)then

        --_G.gunEquipped
        renderer.mColor.w =0

        if( indexing >=6)then
            indexing = 1
        end

    elseif(timercheck >0.4)then
        transform.mTranslate.x = poschecker[indexing]

        renderer.mColor.w =1
        indexing = indexing+1

        timercheck = 0
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
