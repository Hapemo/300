_G.teachWeaponToggle = false
local weaponToggleOnce = false



function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    WeaponTypesGUI= gameStateSys:GetEntity("GUI_Weapon_Types")
    WeaponTypesButton = gameStateSys:GetEntity("GUI_Weapon_Types_Button")
    WeaponTypesGUI_Transform = WeaponTypesGUI:GetTransform()
    WeaponTypesButton_Transform =  WeaponTypesButton:GetTransform()

    O_GUI =  gameStateSys:GetEntity("GUI_Override")
    O_transform = O_GUI:GetTransform()

end 

function Update()

    -- [1] Check if this tutorial aid has activated
    if(_G.teachWeaponToggle == true and weaponToggleOnce == false) then 
        print("WEAPON TOGGLE TRUE")
        _G.teachWeaponToggle = false
        weaponToggleOnce = true

        -- Logic (move the GUI into view)
        WeaponTypesGUI_Transform.mTranslate.x = 0
        WeaponTypesButton_Transform.mTranslate.x = 0.55

        _G.Tutorial_Phase_1_Activate = true
        _G.GUITutorialActive = true
        -- graphicsSys:HideCursor(false)
        -- Helper.setTranslate(toggleWeaponGUI, toggleWeaponGUI_Transform.mTranslate)
        -- Helper.setTranslate(toggleWeaponGUI, toggleWeaponGUI_Transform.mTranslate)
    end

    if(_G.Tutorial_Boss_Defeated == true) then 
        O_transform.mTranslate.x = 0
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


