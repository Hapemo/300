movement = Vec3.new()
zeroVector = Vec3.new(0,0,0)
viewVec = Vec3.new()
viewVecCam = Vec3.new()
rotationCam = Vec3.new()

forward = Vec3.new()
back = Vec3.new()
left = Vec3.new()
right = Vec3.new()
mul = 20.0
floorCount = 0
totaltime = 0.0
isDashing = false;
speed = 10
positions = Vec3.new(0,0,10)
positions_offset = Vec3.new(0,0,0)
positions_final = Vec3.new()

function Alive()
    gameStateSys = systemManager:mGameStateSystem();
    inputMapSys = systemManager:mInputActionSystem();
    physicsSys = systemManager:mPhysicsSystem();
    cameraEntity = Helper.GetScriptEntity(script_entity.id)
    totaltime = 3.0

end

function Update()
    totaltime = totaltime + 0.016;
    positions = cameraEntity:GetTransform().mTranslate
    Camera_Scripting.RotateCameraView(cameraEntity, Input.CursorPos())
    viewVec = Camera_Scripting.GetDirection(cameraEntity)
    viewVecCam = Camera_Scripting.GetDirection(cameraEntity)
    rotationCam = Camera_Scripting.GetDirection(cameraEntity)
    viewVec.y = 0;
    viewVec = Helper.Normalize(viewVec)

    movement.x = 0;
    movement.y = cameraEntity:GetRigidBody().mVelocity.y
    movement.z = 0;

    if (isDashing) then
        if (totaltime > 0.1) then
            isDashing = false
            totaltime = 0
        else
            movement.x = movement.x + (viewVec.x * 300.0)
            movement.z = movement.z + (viewVec.z * 300.0);
        end
    else 
        if (inputMapSys:GetButtonDown("Dash")) then
            if (totaltime > 3.0) then
                totaltime = 0
                isDashing = true
            end
        else
            if (inputMapSys:GetButton("up")) then
                movement.x = movement.x + (viewVec.x * mul);
                movement.z = movement.z + (viewVec.z * mul);
            end
            if (inputMapSys:GetButton("down")) then
                movement.x = movement.x - (viewVec.x * mul);
                movement.z = movement.z - (viewVec.z * mul);
            end
            if (inputMapSys:GetButton("left")) then
                movement.x = movement.x + (viewVec.z * mul);
                movement.z = movement.z - (viewVec.x * mul);
            end
            if (inputMapSys:GetButton("right")) then
                movement.x = movement.x - viewVec.z * mul;
                movement.z = movement.z + viewVec.x * mul;
            end
            if (floorCount > 0) then
                if (inputMapSys:GetButtonDown("Jump")) then
                    movement.y = movement.y + 50.0;
                end
            end
        end
    end

    physicsSys:SetVelocity(cameraEntity, movement)

    -- positions_offset.x =  viewVec.x *10
    -- positions_offset.y =  viewVec.y *10
    -- positions_offset.z =  viewVec.z *10

    -- positions_final.x = positions.x + positions_offset.x
    -- positions_final.y = positions.y + positions_offset.y
    -- positions_final.z = positions.z + positions_offset.z
    
    if(inputMapSys:GetButtonDown("Shoot")) then
        prefabEntity = systemManager.ecs:NewEntityFromPrefab("bullet", positions)
        rotationCam.x = rotationCam.z *360
        rotationCam.y = rotationCam.x *0
        rotationCam.z = rotationCam.z *0
        prefabEntity:GetTransform().mRotate = rotationCam    
        viewVecCam.x = viewVecCam.x*1
        viewVecCam.y=viewVecCam.y *1
        viewVecCam.z=viewVecCam.z *1

        physicsSys:SetVelocity(prefabEntity, viewVecCam)
    end




end

function Dead()

end

function OnTriggerEnter(Entity)
    generalComponent = Entity:GetGeneral()
    tagid = generalComponent.tagid
    if (tagid == 3) then
        floorCount = floorCount + 1;
    end
end

function OnTriggerExit(Entity)
    generalComponent = Entity:GetGeneral()
    tagid = generalComponent.tagid
    if (tagid == 3) then
        floorCount = floorCount - 1;
    end
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end
