local boss_entity 
_G.bossHP_healthbar_comp = nil

local this -- hp bar
local initonce
local gameStateSys

local deathtimer =0

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    boss_entity = gameStateSys:GetEntityByScene("Boss", "BossStuff")

    this = Helper.GetScriptEntity(script_entity.id)

    -- if(this ~= nil) then 
    --     print("NOT NULL")
    -- end
    _G.bossHP_healthbar_comp= boss_entity:GetHealthbar()

    if(_G.bossHP_healthbar_comp ~= nil) then 
        print("BOSS has healthbar")
        print("BOSS HP: " , _G.bossHP_healthbar_comp.health)
    end
    initonce = false

    -- hide the objective ui
    objectiveprogressbarempty = gameStateSys:GetEntity("CapturingObjectiveEmpty" , "UI")
    objectiveprogressbarempty:GetUIrenderer().mColor.w = 0.0
end

function Update()
    this:GetUIrenderer():SetSlider(_G.bossHP_healthbar_comp.health / _G.bossHP_healthbar_comp.maxHealth)

    if(_G.bossHP_healthbar_comp.health < 0) then 

        if initonce == false then
            initonce = true

            boss_entity:GetMeshRenderer():SetMesh("Boss_Death", boss_entity)


            -- boss_entity:GetAnimator():SetFrame(2)

        end



        if( boss_entity:GetAnimator():IsEndOfAnimation())then
            boss_entity:GetAnimator():PauseAnimation()

            boss_entity:GetTransform().mScale.x =0
            boss_entity:GetTransform().mScale.y =0
            boss_entity:GetTransform().mScale.z =0
            controllerL2 = gameStateSys:GetEntity("DialogueControllerLevel3")
            controllerL2Scripts = controllerL2:GetScripts()
            controllerL2Script = controllerL2Scripts:GetScript("../assets/Scripts/ DialogueControllerLevel3.lua")
        

            _G.bossDeath = true
            

            
            
        end

        if( _G.bossDeath == true)then
            deathtimer = deathtimer + FPSManager.GetDT()
        end

        if( deathtimer>2)then

            if controllerL2Script ~= nil then
                controllerL2Script:RunFunction("FinishBoss")
            end
        end


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


