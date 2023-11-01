local movement = Vec3.new()
local zeroVector = Vec3.new(0,0,0)
local viewVec = Vec3.new()
local viewVecCam = Vec3.new()
local rotationCam = Vec3.new()

local forward = Vec3.new()
local back = Vec3.new()
local left = Vec3.new()
local right = Vec3.new()
local centerscreen = Vec2.new()
local mul = 20.0
local floorCount = 0
local totaltime = 0.0
local isDashing = false;
local speed = 10
local positions = Vec3.new(0,0,10)
local positions_offset = Vec3.new(0,20,0)
local positions_final = Vec3.new()

local e_chroma = 0.06
local d_chroma = 0.02
local e_exposure = 5
local d_exposure = 0.5
local e_texel = 5
local d_texel = 1.9
local e_sampleWeight = 0.6
local d_sampleWeight = 0.8
local e_tint = Vec4.new(1,0,0,0.3)
local d_tint = Vec4.new(1,1,1,1)
local e_fov = 30
local d_fov = 60

-- mouse attributes
local mouse_move = Vec2.new()
local mouse_on = true

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
        dashEffect()
        if (totaltime > 0.1) then
            dashEffectEnd()
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


--region -- dash effect




--endregion --dash effect


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

function dashEffect()
    graphicsSys.mChromaticStrength = e_chroma
    graphicsSys.mAmbientBloomExposure = e_exposure
    graphicsSys.mTexelOffset = e_texel
    graphicsSys.m_GlobalTint = e_tint
    graphicsSys.mSamplingWeight = e_sampleWeight
    Camera_Scripting.SetFov(cameraEntity,e_fov)
end

function dashEffectEnd()
    graphicsSys.mChromaticStrength = d_chroma;
    graphicsSys.mAmbientBloomExposure = d_exposure;
    graphicsSys.mTexelOffset = d_texel;
    graphicsSys.m_GlobalTint = d_tint;
    graphicsSys.mSamplingWeight = d_sampleWeight
    Camera_Scripting.SetFov(cameraEntity,d_fov)
end