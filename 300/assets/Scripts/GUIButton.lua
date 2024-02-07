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

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    this = Helper.GetScriptEntity(script_entity.id)

    uirend  = this:GetUIrenderer()
    button = this:GetButton()

    gen = this:GetGeneral()

    clickAudioEntity = gameStateSys:GetEntity("UI_Click_Audio")
    clickSFX = clickAudioEntity:GetAudio()

    toggleWeaponGUI= gameStateSys:GetEntity("Tutorial_Toggle_Weapon_GUI")
    toggleWeaponButton = gameStateSys:GetEntity("Toggle_Weapon_Close_Button")
    toggleWeaponGUI_Transform = toggleWeaponGUI:GetTransform()
    toggleWeaponButton_Transform = toggleWeaponButton:GetTransform()

    graphicsSys = systemManager:mGraphicsSystem()
end

function Update()
    if(gen.name == "Toggle_Weapon_Close_Button") then
        -- print("HELLO BUTTON")
        if (button.mIsHover) then
            if (hoverOver == false) then
            -- print("BUTTON HOVER")
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
            graphicsSys:HideCursor(false)
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


