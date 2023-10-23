Helper = {}

Helper.test = 5

function Helper.printTest()
    print("HELPER WORKS!") 
end

-- Make entity obj of entity that is attached to script
function Helper.GetScriptEntity(id)
    entity = Entity.new(id)
    return entity
end

-- Make AudioSource object
function Helper.CreateAudioSource(Entity)
    audioEntity = AudioSource.new();
    audioEntity:GetAudio(Entity);
    return audioEntity
end
