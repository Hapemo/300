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
-- local walkingAudioSource
-- local audioComp

-- local bulletshootEntity
-- local bulletshootComp
-- local bulletshootAudioSource

-- local dashEntity
-- local dashComp
-- local dashAudioSource

-- local jumpEntity
-- local jumpComp
-- local jumpAudioSource

local dashEntity
local dashAudioComp

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

    -- audioComp = cameraEntity:GetAudio()
    dashui = gameStateSys:GetEntity("UI1", "testSerialization")
    -- walkingAudioSource = Helper.CreateAudioSource(cameraEntity)

    dashEntity = gameStateSys.GetEntity("Dash" , "testSerialization")
    dashAudio = dashEntity:GetAudio()

    dashAudio:Play(1.0f)
    dashAudio:Pause()
    dashAudio:Resume()
    dashAudio:Stop()
    dashAudio:UpdateVolume(0.5f)
    -- bulletshootEntity = gameStateSys:GetEntity("Bullet Shoot", "testSerialization")
    -- bulletshootComp = bulletshootEntity:GetAudio()
    -- bulletshootAudioSource = Helper.CreateAudioSource(bulletshootEntity)

    -- dashEntity = gameStateSys:GetEntity("Dash", "testSerialization")
    -- dashComp = dashEntity:GetAudio()
    -- dashAudioSource = Helper.CreateAudioSource(dashEntity)

    -- jumpEntity = gameStateSys:GetEntity("Jump", "testSerialization")
    -- jumpComp = jumpEntity:GetAudio()
    -- jumpAudioSource = Helper.CreateAudioSource(jumpEntity)
    -- jumpAudioSource:GetAudio(jumpEntity)

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
        -- dashAudioSource:Play()
        -- dashAudioSource:SetVolume(0.2)
        if(e_dashEffect == true)then
            dashEffect()
          
            e_dashEffect = false
        end
        movement.x = movement.x + (viewVec.x * 300.0)
        movement.z = movement.z + (viewVec.z * 300.0);
    
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

        

        if (inputMapSys:GetButtonDown("Dash")) then
            if (dashTime > 3.0) then
                dashTime = 0
                isDashing = true
             
            end
        else
        

            -- else
            --     if (fadeOutTimer < fadeOutDuration) then 
            --         local volume = audioComp.mVolume - (fadeOutTimer / fadeOutDuration)
            --         walkingAudioSource:SetVolume(volume)
            --         fadeOutTimer = fadeOutTimer + dt
            --         print("FADE OUT TIMER: ", fadeOutTimer)
            --     end
            -- end

            if (inputMapSys:GetButton("up")) then
                movement.x = movement.x + (viewVec.x * mul);
                movement.z = movement.z + (viewVec.z * mul);    
                -- print("Volume: " , audioComp.mVolume)
              
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
                    -- jumpAudioSource:Play()
                    -- jumpAudioSource:SetVolume(0.1)
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

        -- bulletshootAudioSource:Play()
        -- bulletshootAudioSource:SetVolume(0.5)
   

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
    graphicsSys.mChromaticStrength = e_chroma
    graphicsSys.mAmbientBloomExposure = e_exposure
    graphicsSys.mTexelOffset = e_texel
   -- graphicsSys.m_GlobalTint = e_tint*t
    graphicsSys.mSamplingWeight = e_sampleWeight



    Camera_Scripting.SetFov(cameraEntity,e_fov)
end

function dashEffectEnd()
    graphicsSys.mChromaticStrength = e_chroma + (d_chroma-e_chroma )*t
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