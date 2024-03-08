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
local d_chroma = 0.005
local e_exposure = 5
local d_exposure = 0.2
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
--_G.mouse_on = true

-- audio attributes
local bulletAudioEntity
local bulletAudioComp

local jumpAudioEntity
local jumpAudioComp

local dashAudioEntity
local dashAudioComp

local machineGunAudioEntity
local machineGunAudioComp

local walkingAudioEntity
local walkingAudioComp

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
local gunDisplaceBackSpeed = 0.01
local gunDisplaceSpeed = 0.01
local gunMaxAcceleration = 100

local gunThreshHold_max_y = -0.2
local gunThreshHold_min_y = -0.5

local gunThreshHold_min_x = 0.4
local gunThreshHold_max_x = 0.6

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
local revolverGunCooldown = 0.7
local shotGunCooldown = 1.5
local machineGunCooldown = 0.2
local pistolCooldown = 0.5

local revolverGunTimer = 0
local shotGunTimer = 0 
local machineGunTimer = 0
local pistolTimer = 0

-- gun states
local gunRecoilState = "IDLE"       -- ["STARTUP", "IDLE" , "MOVING"]
_G.gunEquipped = 0 --"PISTOL"      -- rename this to whatever ["PISTOL (DEFAULT)" , "REVOLVER" , "SHOTGUN" , "MACHINE GUN"]  - 1/31 (pistol new default gun)
local gunHoldState = "NOT HELD"     -- ["NOT HELD" , "HOLDING"]

local pistolShootState = "SHOOTABLE" 
local revolverShootState = "SHOOTABLE"
local shotgunShootState = "SHOOTABLE"   -- ["SHOOTABLE" , "COOLDOWN"]
local machinegunShootState = "SHOOTABLE"
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

local once = false
local up_vector = Vec3.new(0, 1, 0)
--This variable is to be set in another script
--testingSet = 5.0

local world_local_right_vector

local dashX
local dashY
local dashZ
local inittedDash = false

local isTakingDamage = false; -- whether player is in contact with other enemies, thus taking damage
local playerHealthCurrent = 100;
local playerHealthMax = 100;
local playerHealthRegen_Cooldown = 0;
local playerHealthRegen_CooldownMax = 4; -- this is the time it takes for the player to not be damaged to start regenerating health

local healthbarSpawnPos = Vec3.new()
local objectiveBarEmptySpawnPos = Vec3.new()
local healthBarEmptySpawnPos = Vec3.new()
local isuiinit = false
local this

local isGamePaused


local DamageCD =1.1;
local DamageTime = 1.0;

local minExposure = 0.1
local minFilterRadius = 0.001
local maxExposure = 1.2
local maxFilterRadius = 0.05
local dmgAudioEnt 
local dmgAudioComp

_G.FreezePlayerControl = false

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    gameStateSys = systemManager:mGameStateSystem();
    inputMapSys = systemManager:mInputActionSystem();
    physicsSys = systemManager:mPhysicsSystem();
    graphicsSys = systemManager:mGraphicsSystem();
    audioSys    = systemManager:mAudioSystem();
    aiSys    = systemManager:mAISystem();
    cameraEntity = Helper.GetScriptEntity(script_entity.id)
    totaltime = 3.0

    cameraPhysicsComp = cameraEntity:GetRigidBody()
    
    dashui = gameStateSys:GetEntityByScene("dashui" , "UI")

    bulletAudioEntity = gameStateSys:GetEntity("Bullet Shoot" )
    bulletAudioComp = bulletAudioEntity:GetAudio()

    jumpAudioEntity = gameStateSys:GetEntity("Jump" )
    jumpAudioComp = jumpAudioEntity:GetAudio()

    dashAudioEntity = gameStateSys:GetEntity("Dash" )
    dashAudioComp = dashAudioEntity:GetAudio()

    dmgAudioEnt = gameStateSys:GetEntity("DamageAudio" )
    dmgAudioComp = dmgAudioEnt:GetAudio()

    machineGunAudioEntity = gameStateSys:GetEntity("Machine Gun Shoot (LOOP)" )
    machineGunAudioComp = machineGunAudioEntity:GetAudio()
    machineGunAudioComp:UpdateVolume(0.0)
    machineGunAudioComp:SetPlay()

    walkingAudioComp = cameraEntity:GetAudio()
    -- walkingAudioComp:UpdateVolume(0.0)
    walkingAudioComp:SetPlay(0.2)
    
    if(cameraEntity:HasAudio()) then 
        -- print("HAS AUDIO")
    end
    isGamePaused = false

    dashTime = 3.0
    tpTime = 20.0
    teleporter1 = gameStateSys:GetEntity("Teleporter1")
    teleporter2 = gameStateSys:GetEntity("Teleporter2")
    --walkingenemy = gameStateSys:GetEntity("enemy1_walking")
    onTpTime = 0;
    collideWithTP = 0
    originalSamplingWeight = graphicsSys.mSamplingWeight
    tpfin1 = gameStateSys:GetEntity("Fin1")
    tpfin2 = gameStateSys:GetEntity("Fin2")

    -- Gun Stuff --
    gunEntity = gameStateSys:GetEntity("gun")
    gunMeshRenderer = gunEntity:GetMeshRenderer()
    gunInitialTranslate = gunEntity:GetTransform().mTranslate
    gunRotation = gunEntity:GetTransform().mRotate

    -- -- Testing [ParentChildRotateInit]
    gunTransform = gunEntity:GetTransform()
    -- gunTransform:ParentChildRotateInit('x', 30)

    -- Original Gun Position --
    original_translate_x =  gunInitialTranslate.x
    original_translate_y =  gunInitialTranslate.y
    original_translate_z =  gunInitialTranslate.z 
    original_translation.x = gunInitialTranslate.x
    original_translation.y = gunInitialTranslate.y
    original_translation.z = gunInitialTranslate.z

    -- Shotgun Stuff -- ==--==
   

end

function Update()
    -- Player Health System Start -- 
    if isuiinit == false then
        graphicsSys:HideCursor(true)
        print("Calling hide cursor in player update")
        isuiinit = true
    end

    if _G.FreezePlayerControl then 
        return 
    end

    -- healthbar = gameStateSys:GetEntityByScene("Health Bar","Objectives") // Changed to UI scene
    healthbar = gameStateSys:GetEntity("HealthBar", "UI")

    -- Player Health System End -- 
    
    if(graphicsSys.FilterRadius >minFilterRadius )then
        graphicsSys.FilterRadius =graphicsSys.FilterRadius- ((maxFilterRadius - minFilterRadius)*1/60)
    end

    if(graphicsSys.mAmbientBloomExposure > minExposure)then
        graphicsSys.mAmbientBloomExposure = graphicsSys.mAmbientBloomExposure-((maxExposure - minExposure)*1/20)


    end


    -- if (isTakingDamage == false) then -- if not taking damage
    if(DamageCD<=DamageTime)then
            DamageCD = DamageCD + FPSManager.GetDT()

            if(DamageCD < DamageTime-0.6)then
                cameraEntity:GetTransform().mRotate.x = cameraEntity:GetTransform().mRotate.x+math.random(-2,2)
                cameraEntity:GetTransform().mRotate.y = cameraEntity:GetTransform().mRotate.y+math.random(-2,2)
            end
    end

    -- health regen
    if (playerHealthRegen_Cooldown < playerHealthRegen_CooldownMax) then
        playerHealthRegen_Cooldown = playerHealthRegen_Cooldown + FPSManager.GetDT();
    end

    if (playerHealthRegen_Cooldown >= playerHealthRegen_CooldownMax) then
        if (playerHealthCurrent < playerHealthMax) then
            playerHealthCurrent = playerHealthCurrent + 0.5 * FPSManager.GetDT(); -- regen hp
        end
    end

    -- print(playerHealthCurrent/playerHealthMax)
    healthbar:GetUIrenderer():SetSlider(playerHealthCurrent/playerHealthMax);


    if playerHealthCurrent <= 0 then
        gameStateSys:ChangeGameState("LoseMenu")
    end

    -- Player Health System End

    gunTranslate = gunEntity:GetTransform().mTranslate

    -- print("GUNTRANSLATE at the start z: " , gunTranslate.z)
    -- Example: I want to get HP from AITest.lua script (getting walking enemy's hp)
    -- scriptingComp = walkingenemy:GetScripts()
    -- script = scriptingComp:GetScript("../assets/Scripts/AITest.lua")
    -- if script ~= nil then
    --     result = script:RunWithReturnValue_int("GetHP")
    --     print(result)
    -- end

    dt = FPSManager.GetDT()
    -- if(_G.gunEquipped == 1) then -- Revolver
    -- gunTransform:ParentChildRotateUpdate(dt)
    -- end
    if(inputMapSys:GetButtonDown("AddHealth")) then
        playerHealthCurrent = playerHealthCurrent + 20
        if playerHealthCurrent > playerHealthMax then
            playerHealthCurrent = playerHealthMax
        end
    end

    if(inputMapSys:GetButtonDown("MinusHealth")) then
        print("MINUS HEALTH")
        playerHealthCurrent = playerHealthCurrent + 20
        if playerHealthCurrent > playerHealthMax then
            playerHealthCurrent = playerHealthMax
        end
    end
    --if(inputMapSys:GetButtonDown("Mouse")) then
    --    if (_G.mouse_on == true) then
    --        _G.mouse_on = false
    --    else
    --        _G.mouse_on =true
    --    end
    --end

    -- Player view control
    centerscreen = Input:GetCursorCenter()
    mouse_move.x = Input.CursorPos().x - centerscreen.x
    mouse_move.y = Input.CursorPos().y - centerscreen.y
    Input.SetCursorCenter()

    --print("cursorx " .. Input.CursorPos().x .. ", cursory " .. Input.CursorPos().y)
    --print("centerx " .. centerscreen.x .. ", centery " .. centerscreen.y)
    --print("")

    Camera_Scripting.RotateCameraView(cameraEntity, mouse_move)

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

    dashui = gameStateSys:GetEntityByScene("dashui" , "UI")

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
    
    viewVecCam = Camera_Scripting.GetDirection(cameraEntity)
    rotationCam = Camera_Scripting.GetDirection(cameraEntity)
    targetCam = Camera_Scripting.GetTarget(cameraEntity)
    positionCam = Camera_Scripting.GetPosition(cameraEntity) -- works   

    -- Normalize First
    viewVec.x = targetCam.x - positionCam.x 
    viewVec.y = targetCam.y - positionCam.y
    viewVec.z = targetCam.z - positionCam.z

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
            --print("DASH")
          
            e_dashEffect = false
        end
        movement.x = movement.x + (dashX * 50.0)
        movement.z = movement.z + (dashZ * 50.0);
    
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
             
            end
        else
-- Gun Script
       

         

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

            gunJumpTimer = gunJumpTimer + 1

            if (gunJumpTimer > 20) then
                --gunRecoilState = "IDLE" -- will become "IDLE" unless there's a movement button pressed
                gunJumpTimer = 0
            end

            -- walkingAudioComp:UpdateVolume(0.0)
            this:GetAudio():UpdateVolume(0.0)

            if (inputMapSys:GetButton("up")) then
                movement.x = movement.x + (viewVec.x * mul);
                movement.z = movement.z + (viewVec.z * mul);    
                
                -- gun "jumps down" when player moves forward\
                if(gunTranslate.y > gunThreshHold_min_y) then 
                    gunTranslate.y = gunTranslate.y - gunDisplaceSpeed
                end

                this:GetAudio():UpdateVolume(0.2)

                gunRecoilState = "MOVING"
            end

            if (inputMapSys:GetButton("down")) then
                movement.x = movement.x - (viewVec.x * mul);
                movement.z = movement.z - (viewVec.z * mul);
                
                -- gun "jumps up" when player moves forward
                if(gunTranslate.y < gunThreshHold_max_y) then -- limit to how much 
                    gunTranslate.y = gunTranslate.y + gunDisplaceSpeed
                end

                this:GetAudio():UpdateVolume(0.2)

                gunRecoilState = "MOVING"
            end

            if (inputMapSys:GetButton("left")) then
                movement.x = movement.x + (viewVec.z * mul);
                movement.z = movement.z - (viewVec.x * mul);

                -- gun "move rightwards" when player moves left
                if(gunTranslate.x < gunThreshHold_max_x) then 
                    gunTranslate.x = gunTranslate.x + gunDisplaceSpeed
                end

                this:GetAudio():UpdateVolume(0.2)

                gunRecoilState = "MOVING"
    
            end

            if (inputMapSys:GetButton("right")) then
                movement.x = movement.x - viewVec.z * mul;
                movement.z = movement.z + viewVec.x * mul;

                -- gun "moves leftwards" when player moves right
                if(gunTranslate.x > gunThreshHold_min_x) then 
                    gunTranslate.x = gunTranslate.x - gunDisplaceSpeed
                end

                this:GetAudio():UpdateVolume(0.2)

                gunRecoilState = "MOVING"

            end

            if(gameStateSys:GetCurrentGameState().mName == "test" or gameStateSys:GetCurrentGameState().mName == "Test" ) then
                    if (inputMapSys:GetButtonDown("Jump") and math.abs(movement.y) < 3.05) then
                        movement.y = movement.y + 25.0;
                        gunRecoilState = "MOVING"
                        gunJumped = true
                        jumpAudioComp:SetPlay(0.4)
                    end
            elseif(gameStateSys:GetCurrentGameState().mName == "Test2") then
                if (inputMapSys:GetButtonDown("Jump") and math.abs(movement.y) < 3.05) then
                    movement.y = movement.y + 25.0;
                    gunRecoilState = "MOVING"
                    gunJumped = true
                    jumpAudioComp:SetPlay(0.4)
                end
            end

            if(gunJumped == true) then  -- this loop will drop the gun (dip)
                gunRecoilState = "MOVING"
                -- print("CURRENT GUN Y TRANSLATE: " , gunTranslate.y)
                if(gunTranslate.y > gunThreshHold_min_y) then  -- if gun is still higher than desired height
                    gunTranslate.y = gunTranslate.y - gunDisplaceBackSpeed
                    -- print("DECREASING")
                else
                    -- print("DONE :D")
                    gunJumped = false
                end
            end


          
        end

 -- region (Gun Colors)
    if(_G.gunEquipped == 0) then 
        local color_vec4 = Vec4.new(0.5,0.5,0.5,1)
        setBaseGunTexture(color_vec4)
    end

    if(_G.gunEquipped == 1) then 
        local color_vec4 = Vec4.new(0.05, 0.23, 0.7, 1)
        setBaseGunTexture(color_vec4)
    end

    if(_G.gunEquipped == 2) then 
        local color_vec4 = Vec4.new(0.67, 0.65, 0.18, 1)
        setYellowGunTexture(color_vec4)
    end

    if(_G.gunEquipped == 3) then 
        local color_vec4 = Vec4.new(0.65, 0.05, 0.05, 1)
        setRedGunTexture(color_vec4)
    end
-- endregion

        if(inputMapSys:GetButtonDown("Shoot")) then
            gunHoldState = "HOLDING"   -- for machine gun

            if(_G.gunEquipped == 0) then 

                if(pistolTimer == 0) then
                    print("SHOOTING PISTOLS")
                    applyGunRecoil(recoil_speed, 0.5)

                    positions_final.x = positions.x + viewVecCam.x * 3
                    positions_final.y = positions.y + viewVecCam.y * 3
                    positions_final.z = positions.z + viewVecCam.z * 3

                    prefabEntity = systemManager.ecs:NewEntityFromPrefab("Pistol Bullet", positions_final)

                    prefabEntity:GetTransform().mRotate = rotationCam    
                    viewVecCam.x = viewVecCam.x * 100
                    viewVecCam.y = viewVecCam.y * 100
                    viewVecCam.z = viewVecCam.z * 100

                    physicsSys:SetVelocity(prefabEntity, viewVecCam)
                    bulletAudioComp:SetPlay(0.3)
                    
                    pistolTimer = pistolTimer + pistolCooldown

                    pistolShootState = "COOLDOWN"
                end
            end
                


  
        -- print("GUN RECOIL STATE:" , gunRecoilState)
            -- print("GUN EQUIPPED:" , gunEquipped)
            if(_G.gunEquipped == 1 ) then -- REVOLVER
                -- print("SHOOTING REVOLVER")
                if(revolverGunTimer == 0) then 
                    -- print("REVOLVER SHOOTING")
                    applyGunRecoil(recoil_speed, 0.5)   
                    -- <GunAnimation>
                    -- 1. Gun Type 
                    -- 2. Skill Timer (sync it up w the internal skill timer)
                    -- 3. Recoil Angle (how much) - depends on axis set on "parentChildRotateInit()"
                    -- 4. Recoil Speed (how fast)
                    -- 5. Recoil Duration (how long the recoil should last)
                    -- gunTransform:GunAnimation("REVOLVER",_G.skill_duration, 30.0, 1.0, 0.2)  -- Trigger everytime player shoots 
                    -- print("SKILL DURATION" , _G.skill_duration)

                    -- gunRecoilState = "MOVING"

                    -- Shoots Bullet
                    positions_final.x = positions.x + viewVecCam.x
                    positions_final.y = positions.y + viewVecCam.y
                    positions_final.z = positions.z + viewVecCam.z 

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

        if(pistolShootState == "COOLDOWN") then 
            if(pistolTimer > 0) then 
                pistolTimer = pistolTimer - dt
            else 
                pistolTimer = 0
                pistolShootState = "SHOOTABLE"
            end
        end
        -- end of "COOLDOWN" state

        if(inputMapSys:GetButtonDown("Shoot")) then 

        end

        if(_G.gunEquipped == 3) then  --MACHINE GUN
            -- Machine Gun (need to be held down)
            if(gunHoldState == "HOLDING") then 
              
                -- applyGunRecoil(recoil_speed * 0.05 , 0.
                machineGunRecoil()
                machineGunBullets()
                -- gunTransform:GunAnimation("NA", 0 ,  30.0, 1.0, 0.2)
    
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
    end
    
-- end of gun script 

    physicsSys:SetVelocity(cameraEntity, movement)

--endregion

    -- print("AI Count: ")
    -- print(aiSys:GetAICount())

    -- if(aiSys:GetAICount() <= 0) then
    --     gameStateSys:ChangeGameState("WinMenu")
    -- end




    

    -- if(gunShot == true) then
    --     if(gunEquipped == "SHOTGUN") then
    --         shotgun()
    --     end
    -- end

    -- "Cooldown" state
    -- if(gunShootState == "COOLDOWN") then
    --     -- Shotgun
    --     if(gunEquipped == "SHOTGUN") then 
    --         shotGunTimer = shotGunTimer - 1
    --         -- print("SHOTGUN TIMER TILL READY: " ,shotGunTimer)
    --         if(shotGunTimer == 0) then 
    --             gunShootState = "SHOOTABLE"
    --             print("SHOT GUN READY")
    --         end
    --     end
    -- end
    
   

    -- Update the cooldown timer
    machineGunTimer = math.max(0, machineGunTimer - dt)  -- deltaTime is the time since the last fra



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

    if (generalComponent.name == "ILOVEYOU" or generalComponent.name == "Melissa" or generalComponent.name == "TrojanHorse"
    or generalComponent.name == "ZipBomb" or generalComponent.name == "TrojanSoldier" or generalComponent.name == "BigTrojanSoldier")then

        dmgAudioComp:SetPlay(1.0)
        -- print("DAMAGE CD: " , DamageCD)
        -- print("DAMAGE TIME: " , DamageTime)

        if(DamageCD >= DamageTime-0.1)then
            -- if (isTakingDamage == true) then
                playerHealthRegen_Cooldown = 0;
                playerHealthCurrent = playerHealthCurrent - 10; -- take damage

                graphicsSys.FilterRadius = maxFilterRadius
                graphicsSys.mAmbientBloomExposure = maxExposure
               
                -- print("Running Pause Update fromxxxxxxxxxxxxxxxxxxxxxxxx Player.lua")
                DamageCD = 0
                -- isTakingDamage = false
            --end
        end
    end
    -- Player Health System Start --
    -- if (generalComponent.name == "ILOVEYOU") then isTakingDamage = true; end
    -- if (generalComponent.name == "Melissa") then isTakingDamage = true; end
    -- if (generalComponent.name == "TrojanHorse") then isTakingDamage = true; end
    -- if (generalComponent.name == "ZipBomb") then isTakingDamage = true; end
    -- if (generalComponent.name == "TrojanSoldier") then isTakingDamage = true; end
    -- Player Health System End --


    

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

    -- Player Health System Start --
    -- if (generalComponent.name == "ILOVEYOU") then isTakingDamage = false; end
    -- if (generalComponent.name == "Melissa") then isTakingDamage = false; end
    -- if (generalComponent.name == "TrojanHorse") then isTakingDamage = false; end
    -- if (generalComponent.name == "ZipBomb") then isTakingDamage = false; end
    -- if (generalComponent.name == "TrojanSoldier") then isTakingDamage = false; end
    -- Player Health System End --
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

function setBaseGunTexture(color_vec4)
    gunMeshRenderer:SetTexture(MaterialType.DIFFUSE, "Gun_Base_Color")
    gunMeshRenderer:SetColor(color_vec4)
end

function setRedGunTexture(color_vec4)
    gunMeshRenderer:SetTexture(MaterialType.DIFFUSE, "Gun_Base_Color_Red")
    gunMeshRenderer:SetColor(color_vec4)
end

function setYellowGunTexture(color_vec4)
    gunMeshRenderer:SetTexture(MaterialType.DIFFUSE, "Gun_Base_Color_Brown")
    gunMeshRenderer:SetColor(color_vec4)
end
