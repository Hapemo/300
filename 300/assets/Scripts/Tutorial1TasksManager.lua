-- Used in "TriggerTutorialGUI.lua" 
-- Triggered after GUI is done being read.
_G.Tutorial_Phase_1_Activate = false
_G.Tutorial_Boss_Defeated = false
_G.Tutorial_Done = false
local debug_once = false -- for debugging


-- Everything this script runs for the first time, it'll be false.
function Alive()
    
end

function Update()
    if(_G.Tutorial_Phase_1_Activate == true and debug_once == false)  then 
        print("TUTORIAL PHASE 1 ACTIVATED.")
        debug_once = true
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


