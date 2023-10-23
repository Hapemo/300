function Alive()

end

function Update()
    -- Get entity attached to script
    entity = Helper.GetScriptEntity(script_entity.id)
    if entity == nil then
        print("Entity nil in script!")
    end

    -- Get other entity by name
    gameStateSys = systemManager:mGameStateSystem();
    Light = gameStateSys:GetEntity("Light", "Test1")
    rigidBodyComponent = Light:AddRigidBody();

    -- audioEntity = AudioSource.new();
    -- get the entity (either by calling the getEntityByName function or script_entity created)
    -- audioEntity2 = AudioSource.new();
    -- get the entity (either by calling the getEntityByName function or script_entity created)

    -- CrossFadeAudio();
    -- audioEntity:GetAudio(entity);
    -- audioEntity:IsSoundAttached();
    -- audioEntity:AttachSound("farm_ambience.wav");
    -- audioEntity:IsPlaying();
    -- audioEntity:Play();
    -- audioEntity:Pause();
    -- audioEntity:Stop();
    -- audioEntity:Mute();
    -- audioEntity:SetVolume(0.3);
    -- audioEntity:SetIsLoop(1);
    -- audioSys = systemManager:mAudioSystem();
    -- audioSys:LoadAudioFromDirectory("../assets\\Audio")
end

function Dead()

end
