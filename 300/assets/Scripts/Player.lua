movement = Vec3.new()
zeroVector = Vec3.new(0,0,0)
viewVec = Vec3.new()
viewVecCam = Vec3.new()
rotationCam = Vec3.new()

forward = Vec3.new()
back = Vec3.new()
left = Vec3.new()
right = Vec3.new()
centerscreen = Vec2.new()
mul = 20.0
floorCount = 0
totaltime = 0.0
isDashing = false;
speed = 10
positions = Vec3.new(0,0,10)
positions_offset = Vec3.new(0,20,0)
positions_final = Vec3.new()

-- mouse attributes
mouse_move = Vec2.new()
mouse_on = true

function Alive()
    gameStateSys = systemManager:mGameStateSystem();
    inputMapSys = systemManager:mInputActionSystem();
    physicsSys = systemManager:mPhysicsSystem();
    graphicsSys = systemManager:mGraphicsSystem();
    cameraEntity = Helper.GetScriptEntity(script_entity.id)
    totaltime = 3.0
end

function Update()

--region -- player camera
    if(inputMapSys:GetButtonDown("Mouse")) then
        if (mouse_on == true) then
            mouse_on = false
        else
            mouse_on =true
        end
    end

    if  (mouse_on)    then

        centerscreen = Input:GetCursorCenter()
        mouse_move.x = Input.CursorPos().x - centerscreen.x
        mouse_move.y = Input.CursorPos().y - centerscreen.y
        print("cursorx "..Input.CursorPos().x)
        print("cursory "..Input.CursorPos().y)

        print("luax "..centerscreen.x)
        print("luay "..centerscreen.y)

        Camera_Scripting.RotateCameraView(cameraEntity, mouse_move)
        Input.SetCursorCenter()

    end

--endregion



--region -- Player movements
    -- use '.' to reference variable
    totaltime = totaltime + 0.016;
    positions = cameraEntity:GetTransform().mTranslate
    
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


--endregion



--region -- Player Shooting

    if(inputMapSys:GetButtonDown("Shoot")) then
        
        positions_final.x = positions.x + viewVecCam.x*5
        positions_final.y = positions.y + viewVecCam.y*5
        positions_final.z = positions.z + viewVecCam.z*5  

        prefabEntity = systemManager.ecs:NewEntityFromPrefab("bullet", positions_final)
        rotationCam.x = rotationCam.z *360
        rotationCam.y = rotationCam.x *0
        rotationCam.z = rotationCam.z *0
        prefabEntity:GetTransform().mRotate = rotationCam    
        viewVecCam.x = viewVecCam.x*100
        viewVecCam.y=viewVecCam.y *100
        viewVecCam.z=viewVecCam.z *100

        physicsSys:SetVelocity(prefabEntity, viewVecCam)
    end

--endregion

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
