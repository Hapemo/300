function Alive()
    button = Helper.GetScriptEntity(script_entity.id)
    hoverOver = false
end

function Update()

end

function PauseUpdate()
    if (button:GetButton().mIsHover) then
        if(hoverOver == false) then
            hoverSFX = button:GetAudio()
                hoverSFX:SetPlay(0.2)
            hoverOver = true
        end

        if (button:GetGeneral().name == "SettingsBackButton") then
            button:GetUIrenderer():SetTexture("Back_Hover")
        elseif (button:GetGeneral().name == "SettingsWindowButton") then
            button:GetUIrenderer():SetTexture("Window_Hover")
        elseif (button:GetGeneral().name == "SettingsFSButton") then
            button:GetUIrenderer():SetTexture("FullScreen_Hover")
        elseif (button:GetGeneral().name == "sfxDecrease") then
            button:GetUIrenderer():SetTexture("Vol_Decrease_Hover")
        elseif (button:GetGeneral().name == "sfxIncrease") then
            button:GetUIrenderer():SetTexture("Vol_Increase_Hover")
        elseif (button:GetGeneral().name == "bgmDecrease") then
            button:GetUIrenderer():SetTexture("Vol_Decrease_Hover")
        elseif (button:GetGeneral().name == "bgmIncrease") then
            button:GetUIrenderer():SetTexture("Vol_Increase_Hover")
        end
    else
        hoverOver = false
        if (button:GetGeneral().name == "SettingsBackButton") then
            button:GetUIrenderer():SetTexture("Back_Default")
        elseif (button:GetGeneral().name == "SettingsWindowButton") then
            button:GetUIrenderer():SetTexture("Window_Default")
        elseif (button:GetGeneral().name == "SettingsFSButton") then
            button:GetUIrenderer():SetTexture("FullScreen_Default")
        elseif (button:GetGeneral().name == "sfxDecrease") then
            button:GetUIrenderer():SetTexture("Vol_Decrease_Default")
        elseif (button:GetGeneral().name == "sfxIncrease") then
            button:GetUIrenderer():SetTexture("Vol_Increase_Default")
        elseif (button:GetGeneral().name == "bgmDecrease") then
            button:GetUIrenderer():SetTexture("Vol_Decrease_Default")
        elseif (button:GetGeneral().name == "bgmIncrease") then
            button:GetUIrenderer():SetTexture("Vol_Increase_Default")
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


