local this 
local uirend
local button
local gen
local hoverOver

-- Move GUI guide away 
local toggleWeaponGUI
local toggleWeaponButton
local toggleWeaponGUI_Transform
local toggleWeaponButton_Transform

local stackingDamageGUI
local stackingDamageButton 
local stackingDamageGUI_Transform 
local stackingDamageButton_Transform 

local defeatBossGUI
local defeatBossGUIButton
local defeatBossGUI_Transform
local defeatBossGUIButton_Transform

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    this = Helper.GetScriptEntity(script_entity.id)

    uirend  = this:GetUIrenderer()
    button = this:GetButton()

    gen = this:GetGeneral()

    clickAudioEntity = gameStateSys:GetEntity("UI_Click_Audio")
    clickSFX = clickAudioEntity:GetAudio()

    weaponTypesGUI= gameStateSys:GetEntity("GUI_Weapon_Types")
    weaponTypesButton = gameStateSys:GetEntity("GUI_Weapon_Types_Button")
    weaponTypesGUI_Transform = weaponTypesGUI:GetTransform()
    weaponTypesButton_Transform = weaponTypesButton:GetTransform()

    toggleWeaponGUI= gameStateSys:GetEntity("GUI_Weapon_Toggle")
    toggleWeaponButton = gameStateSys:GetEntity("GUI_Weapon_Toggle_Button")
    toggleWeaponGUI_Transform = toggleWeaponGUI:GetTransform()
    toggleWeaponButton_Transform = toggleWeaponButton:GetTransform()

    stackingDamageGUI= gameStateSys:GetEntity("GUI_Stacking_Damage")
    stackingDamageButton = gameStateSys:GetEntity("GUI_Stacking_Damage_Button")
    stackingDamageGUI_Transform = stackingDamageGUI:GetTransform()
    stackingDamageButton_Transform = stackingDamageButton:GetTransform()

    defeatBossGUI = gameStateSys:GetEntity("GUI_Defeat_Boss")
    defeatBossGUIButton = gameStateSys:GetEntity("GUI_Defeat_Boss_Button")
    defeatBossGUI_Transform = defeatBossGUI:GetTransform()
    defeatBossGUIButton_Transform = defeatBossGUIButton:GetTransform()


    graphicsSys = systemManager:mGraphicsSystem()
end

function Update()
    hoverSFX = this:GetAudio()
    if(gen.name == "GUI_Weapon_Types_Button") then
        -- print("HELLO BUTTON")
        if (button.mIsHover) then
            if (hoverOver == false) then
            -- print("BUTTON HOVER")
            hoverSFX:SetPlay(0.2)
            hoverOver = true
            uirend:SetTexture("NextButton_Hover")
            end
        end

        if (button.mIsHover == false) then
            uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end

        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            weaponTypesGUI_Transform .mTranslate.x = -2
            weaponTypesButton_Transform.mTranslate.x = -2
            toggleWeaponGUI_Transform.mTranslate.x = 0 
            toggleWeaponButton_Transform.mTranslate.x = 0.55
            -- graphicsSys:HideCursor(true)
            -- _G.GUITutorialActive = false
        end
    end

    if(gen.name == "GUI_Weapon_Toggle_Button") then
         -- print("HELLO BUTTON")
         if (button.mIsHover) then
            if (hoverOver == false) then
            -- print("BUTTON HOVER")
            hoverSFX:SetPlay(0.2)
            hoverOver = true
            uirend:SetTexture("NextButton_Hover")
            end
        end

        if (button.mIsHover == false) then
            uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end

        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            toggleWeaponGUI_Transform.mTranslate.x = -2
            toggleWeaponButton_Transform.mTranslate.x = -2
            stackingDamageGUI_Transform.mTranslate.x = 0
            stackingDamageButton_Transform.mTranslate.x = 0.55
            -- graphicsSys:HideCursor(true)
            -- _G.GUITutorialActive = false
        end
    end

    if(gen.name == "GUI_Stacking_Damage_Button") then
        
        if (button.mIsHover) then
            if (hoverOver == false) then
            -- print("BUTTON HOVER")
            hoverSFX:SetPlay(0.2)
            hoverOver = true
            uirend:SetTexture("NextButton_Hover")
            end
        end

        if (button.mIsHover == false) then
            uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end

        if (button.mActivated) then
            --print("HELLO BUTTON")
            clickSFX:SetPlay(1.0)
            stackingDamageGUI_Transform.mTranslate.x = -2
            stackingDamageButton_Transform.mTranslate.x = -2
            defeatBossGUI_Transform.mTranslate.x = 0
            defeatBossGUIButton_Transform.mTranslate.x = 0.55
            -- graphicsSys:HideCursor(true)
            -- _G.GUITutorialActive = false
        end
    end

    if(gen.name == "GUI_Defeat_Boss_Button") then
        -- print("HELLO BUTTON")
        if (button.mIsHover) then
            if (hoverOver == false) then
            -- print("BUTTON HOVER")
            hoverSFX:SetPlay(0.2)
            hoverOver = true
            uirend:SetTexture("NextButton_Hover")
            end
        end

        if (button.mIsHover == false) then
            uirend:SetTexture("NextButton_Default")
            hoverOver = false;
        end

        if (button.mActivated) then
            clickSFX:SetPlay(1.0)
            defeatBossGUI_Transform.mTranslate.x = -2
            defeatBossGUIButton_Transform.mTranslate.x = -2
            graphicsSys:HideCursor(true)
            _G.GUITutorialActive = false

            systemManager.ecs:SetDeleteEntity(weaponTypesGUI)
            systemManager.ecs:SetDeleteEntity(weaponTypesButton)
            systemManager.ecs:SetDeleteEntity(toggleWeaponGUI)
            systemManager.ecs:SetDeleteEntity(toggleWeaponButton)

            systemManager.ecs:SetDeleteEntity(stackingDamageGUI)
            systemManager.ecs:SetDeleteEntity(stackingDamageButton)
            systemManager.ecs:SetDeleteEntity(defeatBossGUI)
            systemManager.ecs:SetDeleteEntity(defeatBossGUIButton)
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


