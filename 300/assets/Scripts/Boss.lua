local this

-- Ground slam variables
local groundSlamDirection
local groundSlamPosition = Vec3.new()
local currentGroundSlamResetTimer = 0
local maxGroundSlamResetTimer = 3

-- Tentative Random Boss State CHanger
local currentBossStateTimer = 0
local maxBossStateTimer = 3

-- Boss states
local state = 0
-- 1. Summon enemies
-- 2. Ground slam: Boss swings arms and slams the ground, spawning a ground slam area that damages player
-- 3. Shoot projectiles

function Alive()

end

function Update()

    -- Tentative random switcher between boss states, replace with HP after other states implemented. 100% HP Left = Phase 1, 66% HP Left = Phase 2, 33% HP Left = Phase 3
    currentBossStateTimer = currentBossStateTimer + FPSManager.GetDT()
    if currentBossStateTimer >= maxBossStateTimer then
        state = math.random(1, 3)
        currentBossStateTimer = 0
    end

    state = 2 -- For testing only to force state 2, delete afterwards

    if state == 1 then
    end

    if state == 2 then

        -- Timer to set intervals between ground slams
        currentGroundSlamResetTimer = currentGroundSlamResetTimer + FPSManager.GetDT()

        if currentGroundSlamResetTimer >= maxGroundSlamResetTimer then

            -- Pick which direction to ground slam in 
            groundSlamDirection = math.random(1, 3)

            -- Ground slam right (from boss perspective)
            if groundSlamDirection == 1 then
                groundSlamPosition.x = -15
                groundSlamPosition.y = -6
                groundSlamPosition.z = 25
            end

            -- Ground slam front (from boss perspective)
            if groundSlamDirection == 2 then
                groundSlamPosition.x = 0
                groundSlamPosition.y = -6
                groundSlamPosition.z = 25
            end

            -- Ground slam left (from boss perspective)
            if groundSlamDirection == 3 then
                groundSlamPosition.x = 15
                groundSlamPosition.y = -6
                groundSlamPosition.z = 25
            end

            -- TODO: Play arm swinging animation before spawning ground slam object

            roundSlam = systemManager.ecs:NewEntityFromPrefab("GroundSlamObject", groundSlamPosition)
            currentGroundSlamResetTimer = 0 -- Reset ground slam timer

        end

    end

    if state == 3 then
    end

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