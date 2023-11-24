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

--local constants = {
--    test = 1,
--    working = 2.4,
--}
--constants = Helper.protect(constants)

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

local gunJumped = false -- for gun animation

-- Recoil Stuff
local recoil_distance = 0.5
local recoil_speed = 15
local mg_recoil_speed = 2
-- local recoil_speed_SG = 30.0
-- local recoil_speed_MG = 1.0
-- local recoil_speed_R = 15.0
local max_recoil_distance_z = 0.1

local bullet_scale = Vec3.new()

-- Cooldown in seconds (between bullets)
local revolverGunCooldown = 1
local shotGunCooldown = 1.5
local machineGunCooldown = 0.2

local revolverGunTimer = 0
local shotGunTimer = 0 
local machineGunTimer = 0


-- gun states
local gunRecoilState = "IDLE"       -- ["STARTUP", "IDLE" , "MOVING"]
_G.gunEquipped = 1 --"REVOLVER"      -- rename this to whatever ["REVOLVER" , "SHOTGUN" , "MACHINE GUN"]
local gunHoldState = "NOT HELD"     -- ["NOT HELD" , "HOLDING"]

local revolverShootState = "SHOOTABLE"
local shotgunShootState = "SHOOTABLE"   -- ["SHOOTABLE" , "COOLDOWN"]
local machinegunShootState = "SHOOTABLE"
local gunShot = false

local up_vector = Vec3.new()


local dashEntity
local dashAudioComp

local fadeOutTimer = 0.0
local fadeOutDuration = 5.0
local dt
--local teleporter1
--local teleporter2
-- local tpfin1
-- local tpfin2
local tutTeleporter
local skiptutTeleporter
local box2Spawn
local box3Spawn
local playertpoffset = Vec3.new()

local cameraEntity

local tpTime
--local onTpTime
local collideWithTP
local originalSamplingWeight
-- local dashui = {}
--local dashrender= {}

-- local tpcolor = Vec4.new(0, 0, 0, 1)

local inputMapSys = {}

-- _G.tester = 0
-- testing = 0

-- testingSet = Vec3.new();
-- testingSet.x = 5;
-- testingSet.y = 6;
-- testingSet.z = 6;

--Set when collide with teleporter to go next box
local travelBox = false
local wasdTranslate

TutBox = {
   FIRST = 1,
   SECOND = 2,
   THIRD = 3,
   FOURTH = 4,
   FIFTH = 5,
   NONE = 6,
   GAME = 7
}

local tutState = TutBox.FIRST

function Alive()

    gameStateSys = systemManager:mGameStateSystem();
    inputMapSys = systemManager:mInputActionSystem();
    physicsSys = systemManager:mPhysicsSystem();
    graphicsSys = systemManager:mGraphicsSystem();
    audioSys    = systemManager:mAudioSystem();
    cameraEntity = Helper.GetScriptEntity(script_entity.id)
    totaltime = 3.0

    -- -- audioComp = cameraEntity:GetAudio()
    -- dashui = gameStateSys:GetEntity("UI1")

    dashui = gameStateSys:GetEntityByScene("dashui" , "tutorialUI")

    bulletAudioEntity = gameStateSys:GetEntity("BulletShoot")
    bulletAudioComp = bulletAudioEntity:GetAudio()

    jumpAudioEntity = gameStateSys:GetEntity("Jump")
    jumpAudioComp = jumpAudioEntity:GetAudio()

    dashAudioEntity = gameStateSys:GetEntity("Dash")
    dashAudioComp = dashAudioEntity:GetAudio()

    dashTime = 3.0
    tpTime = 20.0
    -- teleporter1 = gameStateSys:GetEntity("Teleporter1")
    -- teleporter2 = gameStateSys:GetEntity("Teleporter2")
    --walkingenemy = gameStateSys:GetEntity("enemy1_walking")
    --onTpTime = 0;
    collideWithTP = 0
    originalSamplingWeight = graphicsSys.mSamplingWeight
    -- tpfin1 = gameStateSys:GetEntity("Fin1")
    -- tpfin2 = gameStateSys:GetEntity("Fin2")

    tutTeleporter = gameStateSys:GetEntity("Tutorial")
    tutTeleporter2 = gameStateSys:GetEntity("Tutorial2")
    tutTeleporter3 = gameStateSys:GetEntity("Game")
    skiptutTeleporter = gameStateSys:GetEntity("SkipTutorial")
    box2Spawn = gameStateSys:GetEntity("Box2Spawn")
    box3Spawn = gameStateSys:GetEntity("Box3Spawn")
    wasd = gameStateSys:GetEntity("WASD")
    jumpText = gameStateSys:GetEntity("JUMPtext")
    zxcText = gameStateSys:GetEntity("ZXCtext")

    -- Gun Stuff --
    gunEntity = gameStateSys:GetEntity("gun")
    gunInitialTranslate = gunEntity:GetTransform().mTranslate
    gunRotation = gunEntity:GetTransform().mRotate

    -- Original Gun Position --
    original_translate_x =  gunInitialTranslate.x
    original_translate_y =  gunInitialTranslate.y
    original_translate_z =  gunInitialTranslate.z 
    original_translation.x = gunInitialTranslate.x
    original_translation.y = gunInitialTranslate.y
    original_translation.z = gunInitialTranslate.z
end

function Update()
    gunTranslate = gunEntity:GetTransform().mTranslate
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

    if tutState == TutBox.SECOND then
        if travelBox == true then
            Helper.SetTranslate(cameraEntity, box2Spawn:GetTransform().mTranslate)
            travelBox = false
            wasdTranslate = wasd:GetTransform().mTranslate
            wasdTranslate.y = 750
        end
    elseif tutState == TutBox.THIRD then
	    if travelBox == true then
	        Helper.SetTranslate(cameraEntity, box3Spawn:GetTransform().mTranslate)
            travelBox = false
            jumpTextTranslate = jumpText:GetTransform().mTranslate
            jumpTextTranslate.y = 2000
            zxcTextTranslate = zxcText:GetTransform().mTranslate
            zxcTextTranslate.y = 750
    	end
    elseif tutState == TutBox.GAME then
	    if travelBox == true then
            gameStateSys:ChangeGameState("Test")
    	end
    end

--endregion



--region -- Player movements
    -- use '.' to reference variable
    tpTime = tpTime + FPSManager.GetDT();

    --meshtp1 = teleporter1:GetMeshRenderer()
    --meshtp2 = teleporter2:GetMeshRenderer()
    --meshtp3 = tpfin1:GetMeshRenderer()
    --meshtp4 = tpfin2:GetMeshRenderer()

    -- tpcolor.x = 1.0 - (tpTime / 20.1)
    -- tpcolor.y = tpTime / 20.1
    -- meshtp1:SetColor(tpcolor)
    -- meshtp2:SetColor(tpcolor)
    -- meshtp3:SetColor(tpcolor)
    -- meshtp4:SetColor(tpcolor)

    dashui = gameStateSys:GetEntityByScene("dashui" , "tutorialUI")

   dashrender = dashui:GetUIrenderer()
    if (dashTime > 3.0) then
        dashrender:SetDegree(0)
        print("came here!")
    else
        dashrender:SetDegree(360 - (dashTime / 3.0) * 360)
        print("came here2")
    end

    -- if (tpTime <= 20.0) then  
    --     graphicsSys.mSamplingWeight = graphicsSys.mSamplingWeight - FPSManager.GetDT()
    --     if (graphicsSys.mSamplingWeight < originalSamplingWeight) then
    --         graphicsSys.mSamplingWeight = originalSamplingWeight
    --     end
    -- end
    -- else
    --     graphicsSys.mSamplingWeight = graphicsSys.mSamplingWeight - FPSManager.GetDT()
    --     if (graphicsSys.mSamplingWeight < originalSamplingWeight) then
    --         graphicsSys.mSamplingWeight = originalSamplingWeight
    --     end
    -- end

    dashTime = dashTime + FPSManager.GetDT();
    positions = cameraEntity:GetTransform().mTranslate
    
    viewVec = Camera_Scripting.GetDirection(cameraEntity)
    viewVecCam = Camera_Scripting.GetDirection(cameraEntity)
    rotationCam = Camera_Scripting.GetDirection(cameraEntity)
    viewVec = Helper.Normalize(viewVec)

    movement.x = 0;
    movement.y = cameraEntity:GetRigidBody().mVelocity.y
    movement.z = 0;

    if (isDashing) then
        if inittedDash == false then
            dashX = viewVec.x
            dashY = viewVec.y
            dashZ = viewVec.z
            inittedDash = true
        end

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
            inittedDash = false
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
            _G.gunEquipped = 2 --"SHOTGUN"
        end
        if(inputMapSys:GetButtonDown("Revolver")) then 
            print("Swapping to revolver")
            _G.gunEquipped = 1 --"REVOLVER"
        end
        if(inputMapSys:GetButtonDown("Machine Gun")) then 
            print("Swapping to machine gun")
            _G.gunEquipped = 3 --"MACHINE GUN"
        end
-- end of Toggle Weapons
        

        if (inputMapSys:GetButtonDown("Dash")) then
            if (dashTime > 3.0) then
                dashTime = 0
                isDashing = true
                if tutState == TutBox.SECOND then
                    wasdTranslate = wasd:GetTransform().mTranslate
                    wasdTranslate.y = 2000
                    jumpTextTranslate = jumpText:GetTransform().mTranslate
                    jumpTextTranslate.y = 750
                end
            end
        else
        
-- region (snapback)
            -- print("GUN RECOIL STATE: ", gunRecoilState)
            if (gunRecoilState == "IDLE") then
                
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
            end 

            -- Recoil Snapback
            if(gunTranslate.z ~= original_translate_z) then 
                gunTranslate.z = gunTranslate.z - gunDisplaceBackSpeed
                --print("GUN TRANSLATE (NOT IDLE): " , gunTranslate.z)
            
                if(gunTranslate.z < original_translate_z) then
                    gunTranslate.z = original_translate_z
                    --print("SNAPBACK TO (NOT IDLE)" , gunTranslate.z)
                end
            end
-- endregion (snapback)

 -- Must be before any state change
            if(gunRecoilState ~= "SHOOTING") then
                gunRecoilState = "IDLE"
            end
-- endregion 

            if (inputMapSys:GetButton("up")) then
                movement.x = movement.x + (viewVec.x * mul);
                movement.z = movement.z + (viewVec.z * mul);    
             
                -- gun "jumps down" when player moves forward\
                if(gunTranslate.y > gunThreshHold_min_y) then 
                    gunTranslate.y = gunTranslate.y - gunDisplaceSpeed
                end

                gunRecoilState = "MOVING"
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
                end

                gunRecoilState = "MOVING"

            end
            if (floorCount > 0) then
                if (inputMapSys:GetButtonDown("Jump")) then
                    movement.y = movement.y + 25.0;
                    jumpAudioComp:SetPlay(0.4)
                end
            end
        end

        -- if (fadeOutTimer < fadeOutDuration) then 
        --     local volume = audioComp.mVolume - (fadeOutTimer / fadeOutDuration)
        --     walkingAudioSource:SetVolume(volume)
        --     fadeOutTimer = fadeOutTimer + dt
        --     print("FADE OUT TIMER: ", fadeOutTimer)
        -- end
    end

    physicsSys:SetVelocity(cameraEntity, movement)


--endregion



--region -- Player Shooting
    if(inputMapSys:GetButtonDown("Shoot")) then
        gunHoldState = "HOLDING"   -- for machine gun

    -- print("GUN RECOIL STATE:" , gunRecoilState)
        -- print("GUN EQUIPPED:" , gunEquipped)
        if(_G.gunEquipped == 1 ) then -- REVOLVER
        
            if(revolverGunTimer == 0) then 
                -- print("REVOLVER SHOOTING")
                
                applyGunRecoil(recoil_speed, 0.5)

                -- gunRecoilState = "MOVING"

                -- Shoots Bullet
                positions_final.x = positions.x + viewVecCam.x*3
                positions_final.y = positions.y + viewVecCam.y*3
                positions_final.z = positions.z + viewVecCam.z*3  

                prefabEntity = systemManager.ecs:NewEntityFromPrefab("Revolver Bullet", positions_final)
                -- rotationCam.x = rotationCam.x *0
                -- rotationCam.y = rotationCam.y *0
                -- rotationCam.z = rotationCam.z *0


                prefabEntity:GetTransform().mRotate = rotationCam    
                viewVecCam.x = viewVecCam.x*100
                viewVecCam.y=viewVecCam.y *100
                viewVecCam.z=viewVecCam.z *100

                physicsSys:SetVelocity(prefabEntity, viewVecCam)
                bulletAudioComp:SetPlay(0.3)

                revolverGunTimer = revolverGunTimer + revolverGunCooldown
                -- print("GUN TIMER:" ,revolverGunTimer)
                
                revolverShootState = "COOLDOWN"
            end
        end 

        -- print("TRANSLATE: " , gunTranslate.z)

        if(_G.gunEquipped == 2) then -- SHOTGUN
            if(shotGunTimer == 0) then 

                moreAccurateShotgun(10)
                applyGunRecoil(recoil_speed * 0.5, 0.1)

                shotGunTimer = shotGunTimer + shotGunCooldown


                shotgunShootState = "COOLDOWN"

                bulletAudioComp:SetPlay(0.3)
            end
        end
    end

    -- "COOLDOWN" state
    if(shotgunShootState == "COOLDOWN") then 
        if(shotGunTimer > 0) then 
            shotGunTimer = shotGunTimer - dt
            -- print("SHOTGUN COOLDOWN: " , shotGunTimer)
        else 
            shotGunTimer = 0
            shotgunShootState = "SHOOTABLE"
            -- print("SHOTGUN IS READY!")
        end
    end

    if(revolverShootState == "COOLDOWN") then 
        if(revolverGunTimer > 0) then 
            revolverGunTimer = revolverGunTimer - dt
        else 
            revolverGunTimer = 0
            revolverShootState = "SHOOTABLE"
        end 
    end
    -- end of "COOLDOWN" state

    -- Machine Gun
    if(_G.gunEquipped == 3) then  --MACHINE GUN
        -- Machine Gun (need to be held down)
        if(gunHoldState == "HOLDING") then 
          
            -- applyGunRecoil(recoil_speed * 0.05 , 0.
            machineGunRecoil()
            machineGunBullets()

            if(machineGunTimer <= 0) then
                bulletAudioComp:SetPlay(0.3)
                machineGunTimer = machineGunCooldown  -- Set the cooldown timer
           end
           machineGunTimer = math.max(0, machineGunTimer - dt)  -- deltaTime is the time since the last fra
        end
    end

    if(inputMapSys:GetButtonUp("Shoot")) then
        gunHoldState = "NOT HELD"   -- for machine gun
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
        print(generalComponent.name)
        floorCount = floorCount + 1;
    end

    if (tagid == 4) then
	if (isDashing) then
		isDashing = false
        movement.x = 0;
        movement.y = 0;
        movement.z = 0;
	end
    end

    if (tagid == 5) then
        print(generalComponent.name)
        -- if (generalComponent.name == teleporter1:GetGeneral().name) then
        --     collideWithTP = 1
        -- elseif (generalComponent.name == teleporter2:GetGeneral().name) then
        --     collideWithTP = 2
        -- end

        if (generalComponent.name == skiptutTeleporter:GetGeneral().name) then
            gameStateSys:ChangeGameState("Test")
        elseif (generalComponent.name == tutTeleporter:GetGeneral().name) then
            tutState = TutBox.SECOND
            travelBox = true
        elseif (generalComponent.name == tutTeleporter2:GetGeneral().name) then
            tutState = TutBox.THIRD
            travelBox = true
        elseif (generalComponent.name == tutTeleporter3:GetGeneral().name) then
            tutState = TutBox.GAME
            travelBox = true
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
        -- collideWithTP = 0
        -- onTpTime = 0
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

function shotgunbullets(number_of_bullets)
    local bullet_speed_modifier = 20
    -- local number_of_bullets = 5

    -- print("IN SHOTGUN")
    -- print("TRANSLATE: " , gunTranslate.z)

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

        gunTranslate.z = original_translate_z -- some weird behaviour (changes this z value to a very big value)
        -- print("WTF" , gunTranslate.z)
        
    end
end 

function moreAccurateShotgun(num_of_bullets) 

    local bullet_speed = 50

    -- 1. Get my local right vector
    up_vector.x = 0 
    up_vector.y = 1
    up_vector.z = 0

    local local_right_vector = Vec3.new()
    local world_true_up_vector = Vec3.new()
    local final_vector = Vec3.new()
    
    local_right_vector = Camera_Scripting.Cross(viewVec, up_vector) -- get vector pointing to rightwards of player
    world_true_up_vector = Camera_Scripting.Cross(local_right_vector, viewVec)
   
    for i = 0 , num_of_bullets , 1 do
        spreadRange = math.random(-1,1)
        spreadRange = spreadRange / 10

        randomAngleinDegree = math.random(0,360)

       -- print("SPREAD RANGE:" , spreadRange)
        world_true_up_vector.x = spreadRange * local_right_vector.x
        world_true_up_vector.y = spreadRange * local_right_vector.y
        world_true_up_vector.z = spreadRange * local_right_vector.z

        final_vector.x = viewVec.x + world_true_up_vector.x
        final_vector.y = viewVec.y + world_true_up_vector.y
        final_vector.z = viewVec.z + world_true_up_vector.z

        -- Starting Position of bullet 
        positions_final.x = positions.x +  final_vector.x * 2-- 'positions' - camera's translate
        positions_final.y = positions.y +  final_vector.y * 2
        positions_final.z = positions.z +  final_vector.z * 2

        final_vector.x = (viewVec.x + world_true_up_vector.x) * bullet_speed
        final_vector.y = (viewVec.y + world_true_up_vector.y) * bullet_speed
        final_vector.z = (viewVec.z + world_true_up_vector.z) * bullet_speed

        -- Randomnized Rotation
        final_vector = Camera_Scripting.Rotate(final_vector, viewVec, randomAngleinDegree)

        bulletPrefab = systemManager.ecs:NewEntityFromPrefab("Shotgun Bullet", positions_final)

        -- Scaling Down (Shotgun pellets)
        original_scale = bulletPrefab:GetTransform().mScale 
        bulletPrefab:GetTransform().mScale.x = original_scale.x / 3
        bulletPrefab:GetTransform().mScale.y = original_scale.y / 3
        bulletPrefab:GetTransform().mScale.z = original_scale.z / 3
        


        physicsSys:SetVelocity(bulletPrefab, final_vector)
    end
end 



function machineGunBullets()
    -- print("HI SHOOTING MACHINE GUN")

    positions_final.x = positions.x + viewVecCam.x*6
    positions_final.y = positions.y + viewVecCam.y*6
    positions_final.z = positions.z + viewVecCam.z*6  

    prefabEntity = systemManager.ecs:NewEntityFromPrefab("Machine Gun Bullet", positions_final)
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

    physicsSys:SetVelocity(prefabEntity, viewVecCam)

end

function crossProduct(v1 ,v2)
    local x = v1[2]*v2[3] - v1[3]*v2[2]
    local y = v1[3]*v2[1] - v1[1]*v2[3]
    local z = v1[1]*v2[2] - v1[2]*v2[1]
    
    return {x, y, z}
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

function applyGunRecoil(recoil_speed, max_recoil_distance_z) 
    gunTranslate = gunEntity:GetTransform().mTranslate -- some weird bug again

    -- print("MAX RECOIL: " , max_recoil_distance_z)
    local distance_travelled_this_frame = recoil_speed * dt
    -- print("DISTANCE TRAELLED:" ,  distance_travelled_this_frame)
    
    if(gunTranslate.z < original_translate_z + distance_travelled_this_frame) and
        (gunTranslate.z < original_translate_z + max_recoil_distance_z) then
        gunTranslate.z = gunTranslate.z + distance_travelled_this_frame -- recoil
        -- print("DISTANCE TRAVELLED (1): " , gunTranslate.z)
    else -- if it extends over the limit 
        gunTranslate.z = max_recoil_distance_z -- recoil
        -- print("DISTANCE TRAVELLED (2): " , gunTranslate.z)
    end
end


function machineGunRecoil()
    
    if((gunTranslate.z < original_translate_z + recoil_distance) and 
        (gunTranslate.z < original_translate_z + max_recoil_distance_z)) then 
        gunTranslate.z = gunTranslate.z + mg_recoil_speed * dt  -- incorporate dt
        -- print("RECOIL SPEED: " , recoil_speed_MG)
        -- print("dt", dt)
    else 
        gunTranslate.z = math.min(gunTranslate.z, original_translate_z + max_recoil_distance_z) -- this makes sure it does not surpass the limit.
    end
end
