function Alive()

end

function Update()

    --Audio Sample Test
    audioSys = systemManager:mAudioSystem();
    --audioSys:PlayAudio("BGM", AudioType.AUDIO_BGM, 0.5);
    --audioSys:MuteSFX();
end

function Dead()

end

function OnTriggerEnter(Entity)
    print("From Fly.lua OnTriggerEnter")
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)

end

function OnContactExit(Entity)

end
