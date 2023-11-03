vec2 = Vec2.new()
function Alive()
end

function Update()
    
    inputMapSys = systemManager:mInputActionSystem();
    --Input Map Test
    if (inputMapSys:GetButtonUp("up")) then
        print("yes done")
    elseif (inputMapSys:GetKeyDown(Key.KEY_SPACE)) then
        print ("space key down")
    end

    vec2 = Input.CursorPos();
    print(vec2.x)
    print(vec2.y)
end

function Dead()

end

function OnTriggerEnter(Entity)
    
end

function OnTrigger(Entity)
    
end

function OnTriggerExit(Entity)
    
end
