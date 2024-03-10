
--entities
local bossEntity
local cameraEntity


local attacktimer = 0
local attackinterval = 3


local animationtimer =0
local STATE =  -1 --------------------- CHANGE THIS FOR INTRO-----------------------

-- screen shake
local Quakeintervalcd = 0
local QuakeLimit = 2
local QuakeLimit2 = 2
local quaketimer = 0
local magnitude = Vec2.new(-1.0,1.0)
local quakinterv = 0.03
local this


function Alive()
    bossEntity = gameStateSys:GetEntity("BossLevel2")
    cameraEntity = gameStateSys:GetEntity("Camera")
    this = Helper.GetScriptEntity(script_entity.id)
end

function Update()
    
    if STATE == 0 then
        attacktimer = attacktimer + FPSManager.GetDT()
        if attacktimer > attackinterval then
            -- play roar
            --bossEntity:GetMeshRenderer():SetMesh("Boss_Roar", bossEntity)
            quaketimer= quaketimer+FPSManager.GetDT()
            if(quaketimer <1)then
                    cameraEntity:GetTransform().mRotate.x = cameraEntity:GetTransform().mRotate.x+math.random(magnitude.x,magnitude.y)
                    cameraEntity:GetTransform().mRotate.y = cameraEntity:GetTransform().mRotate.y+math.random(magnitude.x,magnitude.y)

            else
                quaketimer =0
                attacktimer = 0
            end
            
        end
    end

    -- print(this:GetHealthbar().health)

    if this:GetHealthbar().health <= 0 then
        STATE = 1
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


