-- spawn size = 16
local spawnsize = 8
local timer =0
local y = -1.2
local posArray = {{20,27},{20,6},{20,-11},{1,-11},{-20,-10},{-20,7.5},{-20,27},{1.5,27}}
local spawnobjc = {"blueweapon","redweapon","yellowweapon"}
local randn
local position_1 = Vec3.new(0,-1,0)

function Alive()

    math.randomseed(os.time())
end


function Update()
    timer = timer +  FPSManager.GetDT()

    if(timer > 5)then

        randn = math.random(1,8)
        SpawnerInstance(posArray[randn][1],posArray[randn][2])
        -- SpawnerInstance(20,27)
        timer = 0
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



function SpawnerInstance(x,y)
    local rands = math.random(-spawnsize,spawnsize)
    position_1.x = x +rands
    rands = math.random(-spawnsize,spawnsize)
    position_1.z = y+rands
   
    
    randn = math.random(1,3)
    systemManager.ecs:NewEntityFromPrefab(spawnobjc[randn], position_1)
end


