function Alive()

end

function Update()
    -- Get entity attached to script
    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end
    if entity:HasAudio() == false then
        entityAudio = entity:AddAudio();
    end

    -- Get other entity by name
    gameStateSys = systemManager:mGameStateSystem();
    floorEntity = gameStateSys:GetEntity("Floor", "Test1")
    if entity:HasAudio() == false then
        floorAudio = floorEntity:AddAudio();
    end

    -- Audio Test
    -- Create Audio Source
    entityAudioSource = Helper.CreateAudioSource(entity)
    floorAudioSource = Helper.CreateAudioSource(floorEntity)

    -- CrossFadeAudio();
    -- audioEntity:IsSoundAttached();
    entityAudioSource:AttachSound("farm_ambience.wav");
    -- audioEntity:IsPlaying();
    -- audioEntity:Pause();
    -- audioEntity:Stop();
    -- audioEntity:Mute();
    entityAudioSource:SetVolume(0.5);
    entityAudioSource:SetIsLoop(1);

    if Input.CheckKey(State.HOLD, Key.KEY_C) then
        entityAudioSource:Play();
    elseif Input.CheckKey(State.HOLD, Key.KEY_X) then
        entityAudioSource:Pause();
    elseif Input.CheckKey(State.HOLD, Key.KEY_Z) then
        entityAudioSource:Unpause();
    elseif Input.CheckKey(State.HOLD, Key.KEY_V) then
        entityAudioSource:Stop();
    end
    --audioSys = systemManager:mAudioSystem();
    --audioSys:LoadAudioFromDirectory("../assets\\Audio")
end

function Dead()

end
