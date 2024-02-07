_G.teachWeaponToggle = false
local weaponToggleOnce = false


function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    toggleWeaponGUI= gameStateSys:GetEntity("Tutorial_Toggle_Weapon_GUI")
    toggleWeaponButton = gameStateSys:GetEntity("Toggle_Weapon_Close_Button")
    toggleWeaponGUI_Transform = toggleWeaponGUI:GetTransform()
    toggleWeaponButton_Transform = toggleWeaponButton:GetTransform()
end 

function Update()

    -- [1] Check if this tutorial aid has activated
    if(_G.teachWeaponToggle == true and weaponToggleOnce == false) then 
        print("WEAPON TOGGLE TRUE")
        _G.teachWeaponToggle = false
        weaponToggleOnce = true

        -- Logic (move the GUI into view)
        toggleWeaponGUI_Transform.mTranslate.x = 0
        toggleWeaponButton_Transform.mTranslate.x = 0.55

        _G.Tutorial_Phase_1_Activate = true

        -- Helper.setTranslate(toggleWeaponGUI, toggleWeaponGUI_Transform.mTranslate)
        -- Helper.setTranslate(toggleWeaponGUI, toggleWeaponGUI_Transform.mTranslate)
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


