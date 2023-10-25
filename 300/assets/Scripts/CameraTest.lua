function Alive()

end

function Update()
    gameStateSys = systemManager:mGameStateSystem()
    cameraEntity = gameStateSys:GetEntity("Camera", "testSerialization")
    cameraComponent = cameraEntity:GetCamera()
    target = cameraComponent.mCamera:target()
    print("X: ", target.x)
    print("Y: ", target.y)
    print("Z: ", target.z)
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
