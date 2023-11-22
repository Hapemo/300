local timer;
local scale;
local currScale;
local this;
local eScale = Vec3.new()
function Alive()
    timer = 0.0;
    currScale = 1.0;
    scale = 20.0;
    this = Helper.GetScriptEntity(script_entity.id)
end

function Update()
    timer = FPSManager.GetDT() + timer;
    currScale = scale * math.sin(5.0 * timer);
    eScale = this:GetTransform().mScale
    eScale.x = currScale;
    eScale.y = currScale;
    eScale.z = currScale;

    if (currScale < 0) then
        systemManager.ecs:SetDeleteEntity(this)
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
