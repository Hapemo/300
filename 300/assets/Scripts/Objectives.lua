
local this

-- for objectives bar
local objectivesComplete = 100
local progress
local isInZone = false

function Alive()
    this = Helper.GetScriptEntity(script_entity.id)
    progress = 0
    -- print("Progress is", progress)
end

function Update()
    if (isInZone == true) then
        if (progress < objectivesComplete) then
            progress = progress + 1
            print("Current progress =", progress)
        end
    end

    if (progress == objectivesComplete) then
        print("Objectives complete!")
    end
end

function Dead()

end

function OnTriggerEnter(Entity)
    local tagid = Entity:GetGeneral().tagid  
    print("tagid = ", tagid)
    if (tagid == 0) then -- if colliding with player
        isInZone = true
    end

    -- TODO: Turn all enemies hostiles
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity) 
    local tagid = Entity:GetGeneral().tagid      
    if (tagid == 0) then -- if colliding with player
        isInZone = false
    end

    -- TODO: Non hostile if not near, progress depletes?
end
