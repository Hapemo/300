_G.teachWeaponToggle = false
local weaponToggleOnce = false

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    toggleWeaponGUI= gameStateSys:GetEntity("Tutorial_Toggle_Weapon_GUI")
    toggleWeaponGUI_Transform = toggleWeaponGUI:GetTransform()
end 

function Update()

    -- [1] Check if this tutorial aid has activated
    if(_G.teachWeaponToggle == true and weaponToggleOnce == false) then 
        print("WEAPON TOGGLE TRUE")
        _G.teachWeaponToggle = false
        weaponToggleOnce = true

        -- Logic (move the GUI into view)
        toggleWeaponGUI_Transform.mTranslate.x = 0
        Helper.setTranslate(toggleWeaponGUI, toggleWeaponGUI_Transform.mTranslate)
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


