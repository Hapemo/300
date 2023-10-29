movement = Vec3.new()
zeroVector = Vec3.new(0,0,0)
viewVec = Vec3.new()
forward = Vec3.new()
back = Vec3.new()
left = Vec3.new()
right = Vec3.new()
speed = 10

function Alive()
    gameStateSys = systemManager:mGameStateSystem();
    inputMapSys = systemManager:mInputActionSystem();
    physicsSys = systemManager:mPhysicsSystem();
    cameraEntity = Helper.GetScriptEntity(script_entity.id)
end

function Update()
    Camera_Scripting.RotateCameraView(cameraEntity, Input.CursorPos())
    viewVec = Camera_Scripting.GetDirection(cameraEntity)
    viewVec.y = 0;
    viewVec = Helper.Normalize(viewVec)
    movement.x = 0;
    movement.y = cameraEntity:GetRigidBody().mVelocity.y;
    movement.z = 0;

    if (inputMapSys:GetButton("up")) then
        movement.x = movement.x + viewVec.x * speed;
        movement.z = movement.z + viewVec.z * speed;
    end
    if (inputMapSys:GetButton("down")) then
        movement.x = movement.x - viewVec.x * speed;
        movement.z = movement.z - viewVec.z * speed;
    end
    if (inputMapSys:GetButton("left")) then
        movement.x = movement.x + viewVec.z * speed;
        movement.z = movement.z - viewVec.x * speed;
    end
    if (inputMapSys:GetButton("right")) then
        movement.x = movement.x - viewVec.z * speed;
        movement.z = movement.z + viewVec.x * speed;
    end
    physicsSys:SetVelocity(cameraEntity, movement)

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
