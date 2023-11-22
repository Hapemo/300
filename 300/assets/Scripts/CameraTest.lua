vec = Vec3.new(1.5,8,9)
vec2 = Vec2.new(100,100)
function Alive()

end

function Update()
    gameStateSys = systemManager:mGameStateSystem()
    cameraEntity = gameStateSys:GetEntity("Camera")
    Camera_Scripting.SetPosition(cameraEntity, vec)
    Camera_Scripting.RotateCameraView(cameraEntity, vec2)
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
