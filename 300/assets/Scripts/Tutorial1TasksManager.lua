-- Used in "TriggerTutorialGUI.lua" 
-- Triggered after GUI is done being read.
_G.Tutorial_Phase_1_Activate = false
_G.Tutorial_Phase_2_Activate = false
local debug_once = false -- for debugging
local debug_once_2 = false

-- Used in "Bullet.lua"
-- [Confirm Kill to check the tasks]
_G.killEnemyWithPistol = false
_G.killEnemyWithRevolver = false
_G.killEnemyWithShotGun  = false
_G.killEnemyWithMachineGun = false


-- Everything this script runs for the first time, it'll be false.
function Alive()
    _G.killEnemyWithPistol = false
    _G.killEnemyWithRevolver = false
    _G.killEnemyWithShotGun  = false
    _G.killEnemyWithMachineGun = false
end

function Update()
    if(_G.Tutorial_Phase_1_Activate == true and debug_once == false)  then 
        print("TUTORIAL PHASE 1 ACTIVATED.")
        debug_once = true
    end

    if(_G.killEnemyWithPistol == true and _G.killEnemyWithRevolver == true and _G.killEnemyWithShotGun == true and _G.killEnemyWithMachineGun == true ) then 
        _G.Tutorial_Phase_2_Activate = true
    end

    if(_G.Tutorial_Phase_2_Activate == true and debug_once_2 == false) then 
        print("TUTORIAL PHASE 2 ACTIVATED")
        debug_once_2 = true
    end

    if(_G.killEnemyWithPistol == true) then 
        -- print("PISTOL KILLED SOMEBODDDEH")
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


