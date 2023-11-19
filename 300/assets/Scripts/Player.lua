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
local mul = 10.0
local floorCount = 0
local dashTime = 0.0
local isDashing = false;
local speed = 10
local positions = Vec3.new(0,0,10)
local positions_offset = Vec3.new(0,20,0)
local positions_final = Vec3.new()
local dashTime = 0.0


local e_chroma = 0.06
local d_chroma = 0.02
local e_exposure = 5
local d_exposure = 0.5
local e_texel = 5
local d_texel = 1.9
local e_sampleWeight = 0.8
local d_sampleWeight = 0.6
local e_tint = Vec4.new(1,0,0,0.3)
local d_tint = Vec4.new(1,1,1,1)
local d_tint_result = Vec4.new(0,0,0,0)
local e_fov = 50
local d_fov = 60

local e_dashEffect = true

local t = 0
local t2 = 0
local tinc = 0.1
-- mouse attributes
local mouse_move = Vec2.new()
local mouse_on = true

-- audio attributes
local bulletAudioEntity
local bulletAudioComp

local jumpAudioEntity
local jumpAudioComp

local dashAudioEntity
local dashAudioComp

local machineGunAudioEntity
local machineGunAudioComp

--physics
local cameraPhysicsComp

--gun
local gunEntity
local gunTranslate = Vec3.new()
local gunRotation = Vec3.new()

local original_translate_x
local original_translate_y
local original_translate_z
local original_translation = Vec3.new()

local gunJumpTimer = 0
local gunDisplaceBackSpeed = 0.015
local gunDisplaceSpeed = 0.015
local gunMaxAcceleration = 100

local gunThreshHold_max_y = -0.15
local gunThreshHold_min_y = -0.9

local gunThreshHold_min_x = 0.15 
local gunThreshHold_max_x = 0.9

-- Recoil Stuff
local recoil_distance = 1.0
local recoil_speed = 1.0
local max_recoil_distance_z = 0.2

local bullet_scale = Vec3.new()

local shotGunTimer = 0 
local shotGunCooldown = 50
local machineGunTimer = 0
local machineGunCooldown = 0.2

-- gun states
local gunRecoilState = "IDLE"             -- ["IDLE" , "MOVING"]
local gunEquipped = "REVOLVER"      -- rename this to whatever ["REVOLVER" , "SHOTGUN" , "MACHINE GUN"]
local gunHoldState = "NOT HELD"     -- ["NOT HELD" , "HOLDING"]
local gunShootState = "SHOOTABLE"   -- ["SHOOTABLE" , "COOLDOWN"]
local gunShot = false


-- end gun states

local fadeOutTimer = 0.0
local fadeOutDuration = 5.0
local dt
local teleporter1
local teleporter2
local tpfin1
local tpfin2
local playertpoffset = Vec3.new()

local cameraEntity

local tpTime
local onTpTime
local collideWithTP
local originalSamplingWeight
local dashui = {}
local dashrender= {}

local tpcolor = Vec4.new(0, 0, 0, 1)

function Alive()
    gameStateSys = systemManager:mGameStateSystem();
    inputMapSys = systemManager:mInputActionSystem();
    physicsSys = systemManager:mPhysicsSystem();
    graphicsSys = systemManager:mGraphicsSystem();
    audioSys    = systemManager:mAudioSystem();
    cameraEntity = Helper.GetScriptEntity(script_entity.id)
    totaltime = 3.0

    cameraPhysicsComp = cameraEntity:GetRigidBody()
    
    dashui = gameStateSys:GetEntity("UI1", "testSerialization")

    bulletAudioEntity = gameStateSys:GetEntity("Bullet Shoot" , "testSerialization")
    bulletAudioComp = bulletAudioEntity:GetAudio()

    jumpAudioEntity = gameStateSys:GetEntity("Jump" , "testSerialization")
    jumpAudioComp = jumpAudioEntity:GetAudio()

    dashAudioEntity = gameStateSys:GetEntity("Dash" , "testSerialization")
    dashAudioComp = dashAudioEntity:GetAudio()

    machineGunAudioEntity = gameStateSys:GetEntity("Machine Gun Shoot (LOOP)" , "testSerialization")
    machineGunAudioComp = machineGunAudioEntity:GetAudio()
    machineGunAudioComp:UpdateVolume(0.0)
    machineGunAudioComp:SetPlay()

    dashTime = 3.0
    tpTime = 20.0
    teleporter1 = gameStateSys:GetEntity("Teleporter1", "testSerialization")
    teleporter2 = gameStateSys:GetEntity("Teleporter2", "testSerialization")
    --walkingenemy = gameStateSys:GetEntity("enemy1_walking", "testSerialization")
    onTpTime = 0;
    collideWithTP = 0
    originalSamplingWeight = graphicsSys.mSamplingWeight
    tpfin1 = gameStateSys:GetEntity("Fin1", "testSerialization")
    tpfin2 = gameStateSys:GetEntity("Fin2", "testSerialization")

    -- Gun Stuff --
    gunEntity = gameStateSys:GetEntity("gun", "testSerialization")
    gunTranslate = gunEntity:GetTransform().mTranslate
    gunRotation = gunEntity:GetTransform().mRotate

    -- Original Gun Position --
    original_translate_x = gunTranslate.x
    original_translate_y = gunTranslate.y
    original_translate_z = gunTranslate.z 
    original_translation.x = gunTranslate.x
    original_translation.y = gunTranslate.y
    original_translation.z = gunTranslate.z


    
    

end

function Update()

    -- Example: I want to get HP from AITest.lua script (getting walking enemy's hp)
    -- scriptingSys = systemManager:mScriptingSystem();
    -- scriptingComp = walkingenemy:GetScripts()
    -- script = scriptingComp:GetScript("../assets/Scripts/AITest.lua")
    -- if script ~= nil then
    --     result = script:RunWithReturnValue_int("GetHP")
    --     print(result)
    -- end

    dt = FPSManager.GetDT()
    
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
        -- print("cursorx "..Input.CursorPos().x)
        -- print("cursory "..Input.CursorPos().y)


        -- print("cursorx "..Input.CursorPos().x)
        -- print("cursory "..Input.CursorPos().y)

        -- print("luax "..centerscreen.x)
        -- print("luay "..centerscreen.y)

        Camera_Scripting.RotateCameraView(cameraEntity, mouse_move)
        Input.SetCursorCenter()

    end

--endregion



--region -- Player movements
    -- use '.' to reference variable
    tpTime = tpTime + FPSManager.GetDT();

    meshtp1 = teleporter1:GetMeshRenderer()
    meshtp2 = teleporter2:GetMeshRenderer()
    meshtp3 = tpfin1:GetMeshRenderer()
    meshtp4 = tpfin2:GetMeshRenderer()

    tpcolor.x = 1.0 - (tpTime / 20.1)
    tpcolor.y = tpTime / 20.1
    meshtp1:SetColor(tpcolor)
    meshtp2:SetColor(tpcolor)
    meshtp3:SetColor(tpcolor)
    meshtp4:SetColor(tpcolor)

    dashrender = dashui:GetUIrenderer()
    if (dashTime > 3.0) then
        dashrender:SetDegree(0)
    else
        dashrender:SetDegree(360 - (dashTime / 3.0) * 360)
    end

    if (tpTime > 20.0) then
        if (collideWithTP > 0) then
            onTpTime = onTpTime + FPSManager.GetDT()
            graphicsSys.mSamplingWeight = graphicsSys.mSamplingWeight + FPSManager.GetDT()
            if (graphicsSys.mSamplingWeight > 2.5) then
                graphicsSys.mSamplingWeight = 2.5
            end
            if (onTpTime > 0.8) then
                if (collideWithTP == 1) then
                    playertpoffset.x = cameraEntity:GetTransform().mTranslate.x - teleporter1:GetTransform().mTranslate.x;
                    playertpoffset.y = cameraEntity:GetTransform().mTranslate.y - teleporter1:GetTransform().mTranslate.y;
                    playertpoffset.z = cameraEntity:GetTransform().mTranslate.z - teleporter1:GetTransform().mTranslate.z;
                    playertpoffset.x = playertpoffset.x + teleporter2:GetTransform().mTranslate.x;
                    playertpoffset.y = playertpoffset.y + teleporter2:GetTransform().mTranslate.y;
                    playertpoffset.z = playertpoffset.z + teleporter2:GetTransform().mTranslate.z;

                    Helper.SetTranslate(cameraEntity, playertpoffset)
                    tpTime = 0
                    onTpTime = 0
                elseif (collideWithTP == 2) then
                    playertpoffset.x = cameraEntity:GetTransform().mTranslate.x - teleporter2:GetTransform().mTranslate.x;
                    playertpoffset.y = cameraEntity:GetTransform().mTranslate.y - teleporter2:GetTransform().mTranslate.y;
                    playertpoffset.z = cameraEntity:GetTransform().mTranslate.z - teleporter2:GetTransform().mTranslate.z;
                    playertpoffset.x = playertpoffset.x + teleporter1:GetTransform().mTranslate.x;
                    playertpoffset.y = playertpoffset.y + teleporter1:GetTransform().mTranslate.y;
                    playertpoffset.z = playertpoffset.z + teleporter1:GetTransform().mTranslate.z;

                    Helper.SetTranslate(cameraEntity, playertpoffset)
                    tpTime = 0
                    onTpTime = 0
                end
            end
        else
            graphicsSys.mSamplingWeight = graphicsSys.mSamplingWeight - FPSManager.GetDT()
            if (graphicsSys.mSamplingWeight < originalSamplingWeight) then
                graphicsSys.mSamplingWeight = originalSamplingWeight
        end
    end
    else
        graphicsSys.mSamplingWeight = graphicsSys.mSamplingWeight - FPSManager.GetDT()
        if (graphicsSys.mSamplingWeight < originalSamplingWeight) then
            graphicsSys.mSamplingWeight = originalSamplingWeight
        end
    end

    dashTime = dashTime + FPSManager.GetDT();
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
   
        if(e_dashEffect == true)then
            dashEffect()
            dashAudioComp:SetPlay(0.4)
          
            e_dashEffect = false
        end
        movement.x = movement.x + (viewVec.x * 50.0)
        movement.z = movement.z + (viewVec.z * 50.0);
    
        if (dashTime >= 0.1) then

            dashTime = 0
            isDashing = false
           
        end

        --dashEffectEnd()
    else 
        if(e_dashEffect == false)then
            dashEffectEnd()
        
            if( tinc >=0)then
                tinc = tinc +0.02
             end


            if( t>=1)then
                tinc = 0.1
                t = 0
                e_dashEffect = true
            else
                t = t +tinc 
            end
        end
-- Toggle Weapons
        if(inputMapSys:GetButtonDown("Shotgun")) then 
            print("Swapping to shotgun")
            gunEquipped = "SHOTGUN"
        end
        if(inputMapSys:GetButtonDown("Revolver")) then 
            print("Swapping to revolver")
            gunEquipped = "REVOLVER"
        end
        if(inputMapSys:GetButtonDown("Machine Gun")) then 
            print("Swapping to machine gun")
            gunEquipped = "MACHINE GUN"
        end
   
-- end of Toggle Weapons
        

        if (inputMapSys:GetButtonDown("Dash")) then
            if (dashTime > 3.0) then
                dashTime = 0
                isDashing = true
             
            end
        else
-- Gun Script
            -- print("CURRENT GUN STATE:", gunState) -- uncomment to debug
            -- 'y' : vertical-axis
            if (gunRecoilState == "IDLE") then
                local displacement_x = gunTranslate.x - original_translation.x 
                local displacement_y = gunTranslate.y - original_translation.y
                --print("GUN DISPLACED: " , displacement_x , displacement_y)
                local displacement_z = 0
                
                local acceleration_x = gunMaxAcceleration * displacement_x * displacement_x
                local acceleration_y = gunMaxAcceleration * (1.0 - displacement_y * displacement_y)
                local acceleration_z = gunMaxAcceleration * displacement_z * displacement_z

                -- print("ACCELERATION Y:" , acceleration_y)

                local velocity_x = acceleration_x * dt
                local velocity_y = acceleration_y * dt
                local velocity_z = acceleration_z * dt
                
                -- print("VELOCITY Y:" , velocity_y)
                
                -- Account for "vertical" axis
                if(gunTranslate.y ~= original_translate_y) then
                    if((gunTranslate.y > original_translate_y)) then -- it should go up 
                       -- print("Gun is displaced higher than its original translation")
                       gunTranslate.y = gunTranslate.y - gunDisplaceBackSpeed 
                    end
                
                    
                    if(gunTranslate.y < original_translate_y) then
                       -- print("Gun is displaced lower than its original translation")
                        gunTranslate.y = gunTranslate.y + gunDisplaceBackSpeed 
                    end
                end

                -- Account for "horizontal" axis
                if(gunTranslate.x ~= original_translate_x) then 
                    if(gunTranslate.x < original_translate_x) then  -- left to original
                        gunTranslate.x = gunTranslate.x + gunDisplaceBackSpeed 
                    end

                    if(gunTranslate.x > original_translate_x) then  -- right to original
                        gunTranslate.x = gunTranslate.x - gunDisplaceBackSpeed 
                    end

                    --print("HORIZONTAL AXIS CHANGED")
                end

                -- Account for "recoil"
                if(gunTranslate.z ~= original_translate_z) then 
                    gunTranslate.z = gunTranslate.z - gunDisplaceBackSpeed
                    --print("GUN TRANSLATE: " , gunTranslate.z)
                    if(gunTranslate.z < original_translate_z) then
                        gunTranslate.z = original_translate_z
                        --print("SNAPBACK")
                    end
                end
            end 

            if(gunRecoilState == "JUMP") then 
                print("GUN STATE: JUMP")
                gunTranslate.y = gunTranslate.y - gunDisplaceBackSpeed 
                gunJumpTimer = gunJumpTimer + 1

                if (gunJumpTimer > 20) then
                    gunRecoilState = "IDLE" -- will become "IDLE" unless there's a movement button pressed
                    gunJumpTimer = 0
                end
            -- elseif (gunRecoilState == "MOVING") then
            --     -- Gun Recoil (outside of state check)
            --     print("WTF")
            --     if(gunTranslate.z + recoil_distance <= original_translate_z + recoil_distance) then
            --         gunTranslate.z = gunTranslate.z + recoil_distance -- recoil
            --         print("1")
            --     else 
            --         gunTranslate.z = original_translate_z 
            --         gunTranslate.z = gunTranslate.z + recoil_distance -- recoil
            --         print("2")
            --     end
            else
                 gunRecoilState = "IDLE"
            end 
           
            if (inputMapSys:GetButton("up")) then
                movement.x = movement.x + (viewVec.x * mul);
                movement.z = movement.z + (viewVec.z * mul);    

                -- gun "jumps down" when player moves forward\
                if(gunTranslate.y > gunThreshHold_min_y) then 
                    gunTranslate.y = gunTranslate.y - gunDisplaceSpeed
                end

                gunRecoilState = "MOVING" 

                -- print("GUN MOVES", movement.z * 0.001) 
                -- print("GUN TRANSLATE (Y): " , gunTranslate.y)
                -- print("ORIGINAL TRANLSATE (Y): ", original_translate_y)
            end
            if (inputMapSys:GetButton("down")) then
                movement.x = movement.x - (viewVec.x * mul);
                movement.z = movement.z - (viewVec.z * mul);
                
                -- gun "jumps up" when player moves forward
                if(gunTranslate.y < gunThreshHold_max_y) then -- limit to how much 
                    gunTranslate.y = gunTranslate.y + gunDisplaceSpeed

                end

                gunRecoilState = "MOVING"
            end
            if (inputMapSys:GetButton("left")) then
                movement.x = movement.x + (viewVec.z * mul);
                movement.z = movement.z - (viewVec.x * mul);

                -- gun "move rightwards" when player moves left
                if(gunTranslate.x < gunThreshHold_max_x) then 
                    gunTranslate.x = gunTranslate.x + gunDisplaceSpeed
                    

                end

                gunRecoilState = "MOVING"
    
            end
            if (inputMapSys:GetButton("right")) then
                movement.x = movement.x - viewVec.z * mul;
                movement.z = movement.z + viewVec.x * mul;

                -- gun "moves leftwards" when player moves right
                if(gunTranslate.x > gunThreshHold_min_x) then 
                    gunTranslate.x = gunTranslate.x - gunDisplaceSpeed

                    -- if(gunDisplaceSpeedModifier < gunDisplaceSpeedFactorLimit) then 
                    --     gunDisplaceSpeedModifier = gunDisplaceSpeedModifier + 0.01
                    -- end
                end

                gunRecoilState = "MOVING"

            end
            if (floorCount > 0) then
                if (inputMapSys:GetButtonDown("Jump")) then
                    movement.y = movement.y + 50.0;
                    gunRecoilState = "JUMP"
                    jumpAudioComp:SetPlay(0.4)
                end
            end
        end
-- end of gun script 
    end

    physicsSys:SetVelocity(cameraEntity, movement)
   -- print("VELOCITY" , movement.x , movement.y, movement.z)
    
    --cameraPhysicsComp.mVelocity = movement
    --print("VELOCITY FROM PHYSICS" , cameraPhysicsComp.mVelocity.x , cameraPhysicsComp.mVelocity.y , cameraPhysicsComp.mVelocity.z)


--endregion



--region -- Player Shooting
    --print("GUN SHOOT STATE:" , gunShootState)
    --print("SHOTGUN TIMER:", shotGunTimer)
    if(inputMapSys:GetButtonDown("Shoot")) then
        gunHoldState = "HOLDING"   -- for machine gun

       -- print("GUN RECOIL STATE:" , gunRecoilState)
        
        if(gunEquipped == "REVOLVER") then 
            applyGunRecoil()

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
            bulletAudioComp:SetPlay(0.1)

        end

        if(gunEquipped == "SHOTGUN") then 
            -- This one controls bullet 
            if(gunShootState == "SHOOTABLE") then
                shotgun()
                gunShootState = "COOLDOWN"  -- will trigger (won't go into this loop until cooldown loop ends)
                shotGunTimer = shotGunTimer + shotGunCooldown -- add cooldown once

                bulletAudioComp:SetPlay(0.1)
            end
        end
    end

    if(inputMapSys:GetButtonUp("Shoot")) then 
        -- print("STOPPED PRESSING")
        gunHoldState = "NOT HELD"
    end

    local alternate_audio = false
    if(gunEquipped == "MACHINE GUN") then  
        -- Machine Gun (need to be held down)
        if(gunHoldState == "HOLDING") then 
          
            machineGunRecoil()

            machineGun()

            if(machineGunTimer <= 0) then
                bulletAudioComp:SetPlay(0.1)
                machineGunTimer = machineGunCooldown  -- Set the cooldown timer
           end
           machineGunTimer = math.max(0, machineGunTimer - dt)  -- deltaTime is the time since the last fra
        end
    end

    -- Update the cooldown timer
    machineGunTimer = math.max(0, machineGunTimer - dt)  -- deltaTime is the time since the last fra


    -- print("GUN HOLD STATE: " , gunHoldState)

    -- if(gunEquipped == "MACHINE GUN") then 
    --     positions_final.x = positions.x + viewVecCam.x * 5
    --     positions_final.y = positions.y + viewVecCam.y * 5
    --     positions_final.z = positions.z + viewVecCam.z * 5

    --     --print("POSITION:" , positions_final.x , positions_final.y ,positions_final.z )

    --     bulletEntity = systemManager.ecs:NewEntityFromPrefab("bullet", positions_final)

    --     rotationCam.x = rotationCam.z *360
    --     rotationCam.y = rotationCam.x *0
    --     rotationCam.z = rotationCam.z *0
    --     bulletEntity:GetTransform().mRotate = rotationCam    
    --     viewVecCam.x = viewVecCam.x* 50
    --     viewVecCam.y=viewVecCam.y * 50
    --     viewVecCam.z=viewVecCam.z * 50

    --     physicsSys:SetVelocity(bulletEntity, viewVecCam)
    -- end

    -- "Cooldown" state
    if(gunShootState == "COOLDOWN") then
        -- Shotgun
        if(gunEquipped == "SHOTGUN") then 
            shotGunTimer = shotGunTimer - 1
            --print("SHOTGUN TIMER TILL READY: " ,shotGunTimer)
            if(shotGunTimer == 0) then 
                gunShootState = "SHOOTABLE"
                print("SHOT GUN READY")
            end
        end
        -- Machine Gun
        -- if(gunEquipped == "MACHINE GUN") then 
        --     machineGunTimer = machineGunTimer - 1
        --     --print("SHOTGUN TIMER TILL READY: " ,shotGunTimer)
        --     if(machineGunTimer == 0) then 
        --         gunShootState = "SHOOTABLE"
        --         print("SHOT GUN READY")
        --     end
        -- end

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

    if (tagid == 1) then
        -- play attack anim
    end 

    if (tagid == 5) then
        if (generalComponent.name == teleporter1:GetGeneral().name) then
            collideWithTP = 1
        elseif (generalComponent.name == teleporter2:GetGeneral().name) then
            collideWithTP = 2
        end
    end
end

function OnTriggerExit(Entity)
    generalComponent = Entity:GetGeneral()
    tagid = generalComponent.tagid
    if (tagid == 3) then
        floorCount = floorCount - 1;
    end
    if (tagid == 5) then
        collideWithTP = 0
        onTpTime = 0
    end
end

function OnContactEnter(Entity)
end

function OnContactExit(Entity)

end

function dashEffect()
    graphicsSys.mChromaticOffset = e_chroma
    graphicsSys.mAmbientBloomExposure = e_exposure
   -- graphicsSys.mTexelOffset = e_texel
   -- graphicsSys.m_GlobalTint = e_tint*t
    graphicsSys.mSamplingWeight = e_sampleWeight



    Camera_Scripting.SetFov(cameraEntity,e_fov)
end

function dashEffectEnd()
    graphicsSys.mChromaticOffset = e_chroma + (d_chroma-e_chroma )*t
    graphicsSys.mAmbientBloomExposure = e_exposure+ (d_exposure-e_exposure)*t
    graphicsSys.mTexelOffset = e_texel+ (d_texel-e_texel)*t
    graphicsSys.mSamplingWeight = e_sampleWeight+ (d_sampleWeight -e_sampleWeight)*t
    Camera_Scripting.SetFov(cameraEntity,e_fov+ (d_fov-e_fov)*t)
       -- graphicsSys.m_GlobalTint = d_tint


    -- graphicsSys.mChromaticStrength = d_chroma + (e_chroma-d_chroma )*t
    -- graphicsSys.mAmbientBloomExposure = d_exposure+ (e_exposure-d_exposure)*t
    -- graphicsSys.mTexelOffset = d_texel+ (e_texel-d_texel)*t
    -- graphicsSys.mSamplingWeight = d_sampleWeight+ (e_sampleWeight -d_sampleWeight)*t
    -- Camera_Scripting.SetFov(cameraEntity,d_fov+ (d_fov-e_fov)*t)
end

function shotgun()
    local bullet_speed_modifier = 20
    local number_of_bullets = 5

    applyGunRecoil()

    -- Account for "recoil"
    if(gunTranslate.z ~= original_translate_z) then 
        gunTranslate.z = gunTranslate.z - gunDisplaceBackSpeed
        if(gunTranslate.z < original_translate_z) then
            gunTranslate.z = original_translate_z
        end
    end

    --print("SHOOTING SHOTGUN (no of pellets)" , number_of_bullets)

    for i = 1, number_of_bullets , 1 
    do 
        -- Apply Randomization 
        randomAngleDegreesX = math.random(-5,5)
        randomAngleDegreesY = math.random(-1,1)
        randomAngleDegreesZ = math.random(-5,5)

        -- Rotating the "Camera Direction" vector about the Z-axis (NORMALIZED)
        rotatedVelocity_X = rotateVectorX(viewVec,randomAngleDegreesX)
        rotatedVelocity_XY = rotateVectorY(rotatedVelocity_X, randomAngleDegreesY)
        rotatedVelocity_XYZ = rotateVectorY(rotatedVelocity_XY, randomAngleDegreesZ)
        
        --print("XYZ: " , rotatedVelocity_XYZ.x , rotatedVelocity_XYZ.y,  rotatedVelocity_XYZ.z)

        -- Starting Position of bullet 
        positions_final.x = positions.x +  rotatedVelocity_XYZ.x * 2 -- 'positions' - camera's translate
        positions_final.y = positions.y +  rotatedVelocity_XYZ.y * 2 
        positions_final.z = positions.z +  rotatedVelocity_XYZ.z * 2 

        -- Scaling Randomnized "Rotated Vector"
        rotatedVelocity_XYZ.x =  rotatedVelocity_XYZ.x * bullet_speed_modifier
        rotatedVelocity_XYZ.y =  rotatedVelocity_XYZ.y * bullet_speed_modifier
        rotatedVelocity_XYZ.z =  rotatedVelocity_XYZ.z * bullet_speed_modifier

        bulletPrefab = systemManager.ecs:NewEntityFromPrefab("bullet", positions_final)

        -- Scaling Down (Shotgun pellets)
        original_scale = bulletPrefab:GetTransform().mScale 
        bulletPrefab:GetTransform().mScale.x = original_scale.x / 3
        bulletPrefab:GetTransform().mScale.y = original_scale.y / 3
        bulletPrefab:GetTransform().mScale.z = original_scale.z / 3

        -- Rotation of bullets
        rotationCam.x = rotationCam.z *360
        rotationCam.y = rotationCam.x *0
        rotationCam.z = rotationCam.z *0
        bulletPrefab:GetTransform().mRotate = rotationCam    

        -- print("ROTATED VECTOR:" , rotatedVelocity_XY.x, rotatedVelocity_XY.y, rotatedVelocity_XY.z)
        
        --print("ROTATED VECTOR:" , rotatedVelocity_X.x, rotatedVelocity_X.y, rotatedVelocity_X.z)

        --print("CREATING SHOTGUN BULLET")
        physicsSys:SetVelocity(bulletPrefab, rotatedVelocity_XYZ)
    end
end 

function machineGun()
    -- print("HI SHOOTING MACHINE GUN")

    positions_final.x = positions.x + viewVecCam.x*6
    positions_final.y = positions.y + viewVecCam.y*6
    positions_final.z = positions.z + viewVecCam.z*6  

    prefabEntity = systemManager.ecs:NewEntityFromPrefab("bullet", positions_final)
    rotationCam.x = rotationCam.z *360
    rotationCam.y = rotationCam.x *0
    rotationCam.z = rotationCam.z *0
    prefabEntity:GetTransform().mRotate = rotationCam    
    viewVecCam.x = viewVecCam.x*50
    viewVecCam.y=viewVecCam.y *50
    viewVecCam.z=viewVecCam.z *50

    -- Scaling Down (Machine Gun Pellets)
    original_scale = prefabEntity:GetTransform().mScale 
    prefabEntity:GetTransform().mScale.x = original_scale.x / 5
    prefabEntity:GetTransform().mScale.y = original_scale.y / 5
    prefabEntity:GetTransform().mScale.z = original_scale.z / 5

    print("HI")

    physicsSys:SetVelocity(prefabEntity, viewVecCam)

end

function rotateVectorX(vector, angleInDegrees)
    angleInRadians = math.rad(angleInDegrees)

    cosTheta = math.cos(angleInRadians)
    sinTheta = math.sin(angleInRadians)

    local rotatedVector = Vec3:new()
    rotatedVector.x = vector.x
    rotatedVector.y = cosTheta * vector.y - sinTheta * vector.z
    rotatedVector.z = sinTheta * vector.y + cosTheta * vector.z

    return rotatedVector
end

function rotateVectorY(vector, angleInDegrees)
    angleInRadians = math.rad(angleInDegrees)

    cosTheta = math.cos(angleInRadians)
    sinTheta = math.sin(angleInRadians)

    local rotatedVector = Vec3:new()
    rotatedVector.x = cosTheta * vector.x + sinTheta * vector.z
    rotatedVector.y = vector.y
    rotatedVector.z = -sinTheta * vector.x + cosTheta * vector.z

    return rotatedVector
end


function rotateVectorZ(vector, angleInDegrees)
    angleInRadians = math.rad(angleInDegrees)

    cosTheta = math.cos(angleInRadians)
    sinTheta = math.sin(angleInRadians)

    local rotatedVector = Vec3:new()
    rotatedVector.x = cosTheta * vector.x - sinTheta * vector.y
    rotatedVector.y = sinTheta * vector.x + cosTheta * vector.y
    rotatedVector.z = vector.z

    return rotatedVector
end

function applyGunRecoil() 
    -- print("GUN TRANSLATE: " , gunTranslate.z)
    if(gunTranslate.z + recoil_distance <= original_translate_z + recoil_distance) then
        gunTranslate.z = gunTranslate.z + recoil_distance -- recoil
        --print("1")
    else 
        gunTranslate.z = original_translate_z 
        gunTranslate.z = gunTranslate.z + recoil_distance -- recoil
        --print("2")
    end
end

function machineGunRecoil()
    if((gunTranslate.z < original_translate_z + recoil_distance) and 
        (gunTranslate.z < original_translate_z + max_recoil_distance_z)) then 
        gunTranslate.z = gunTranslate.z + recoil_speed * dt  -- incorporate dt
        print("RECOIL SPEED: " , recoil_speed)
        print("dt", dt)
    else 
        gunTranslate.z = math.min(gunTranslate.z, original_translate_z + max_recoil_distance_z) -- this makes sure it does not surpass the limit.
    end
end

