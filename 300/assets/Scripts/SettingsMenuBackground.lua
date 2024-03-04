function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    inputMapSys = systemManager:mInputActionSystem()
    graphicssys = systemManager:mGraphicsSystem()
    settingsBgmFull = gameStateSys:GetEntity("SettingsBgmFull")
    bgmDecrease = gameStateSys:GetEntity("bgmDecrease")
    bgmIncrease = gameStateSys:GetEntity("bgmIncrease")
    sfxDecrease = gameStateSys:GetEntity("sfxDecrease")
    sfxIncrease = gameStateSys:GetEntity("sfxIncrease")
end

function Update()
    -- --print("CAMEEEEEEEEE")
    -- --find range
    -- -- minRange = bgmSlider:GetTransform().mTranslate.x - bgmSlider:GetTransform().mScale.x
    -- -- maxRange = bgmSlider:GetTransform().mTranslate.x + bgmSlider:GetTransform().mScale.x
    -- -- change according to scene's set position
    -- range = (-0.61 - 0.2) * -1
    -- print("Mos x pos", Input.CursorPos().x)
    -- print(bgmSlider:GetTransform().mTranslate.x)
    -- --if (inputMapSys:GetButtonDown("Shoot")) then
    --     if (bgmSlider:GetButton().mActivated) then
    --         print("YOOOOOOOOO")
    --         -- 0.61 changes according to the set position
    --         settingsBgmFull:GetUIrenderer():SetSlider(bgmSlider:GetTransform().mTranslate.x + 0.61 / range);
    --         bgmSlider:GetTransform().mTranslate.x = Input.CursorPos().x
    --     end
    -- --end

    if (bgmIncrease:GetButton().mActivated) then
        settingsBgmFull:GetUIrenderer():SetSlider();
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


