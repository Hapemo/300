local gunEntity
local gunTranslate
local gunRotation

local originalTranslation
local updateTranslation

local original_translate_x 
local original_translate_y
local original_translate_z

local playerEntity
local playerRigidBody

-- local gunSway = Vec3.new()

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    inputMapSys = systemManager:mInputActionSystem()
    physicsSys = systemManager:mPhysicsSystem()
    gunEntity = Helper.GetScriptEntity(script_entity.id)
    gunTranslate = gunEntity:GetTransform().mTranslate
    gunRotation = gunEntity:GetTransform().mRotation

    -- Original Gun Position --
    original_translate_x = gunTranslate.x
    original_translate_y = gunTranslate.y
    original_translate_z = gunTranslate.z 

    -- Player Velocity --
    playerEntity = gameStateSys:GetEntity("Camera", "testSerialization")
    playerRigidBody = playerEntity:GetRigidBody().mVelocity

end

function Update()

    -- if(gunTranslate.y ~= original_y) then
    --     gunTranslate.y = gunTranslate.y 
    --     print("NOT THE SAME")
    -- end

    if(inputMapSys:GetButtonDown("up")) then
        print("UP")
        print("RIGID BODY" , playerRigidBody.z)
        gunTranslate.y = gunTranslate.y + playerRigidBody.z
        -- gunSway.x = 
        print("GUN: ", gunTranslate.y)
        print("ORIGINAL: " , original_translate_y)
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end
