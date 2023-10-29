vec2 = Vec2.new()
function Alive()
    gameStateSys = systemManager:mGameStateSystem();
    cameraEntity = gameStateSys:GetEntity("Camera", "testSerialization")
    print(cameraEntity.id)
end

function Update()
    vec2 = Input.CursorPos()
    Camera_Scripting.RotateCameraView(cameraEntity, vec2)
    print(vec2.x)
    print(vec2.y)
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
