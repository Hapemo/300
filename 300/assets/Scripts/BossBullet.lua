local bulletObject
local bulletLifeTime = 0
local bulletDeathTime = 1000

function Alive()
    bulletObject = Helper.GetScriptEntity(script_entity.id)
end

function Update()
    while bulletObject ~= nil do 
        --print("THERE IS A BULLET OBJECT")
         bulletLifeTime = bulletLifeTime + 1
         -- print("BULLET LIFETIME: " , bulletLifeTime)
         if(bulletLifeTime > bulletDeathTime) then
             systemManager.ecs:SetDeleteEntity(bulletObject)
             -- print("DELETING BULLET")
         end
         break
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


