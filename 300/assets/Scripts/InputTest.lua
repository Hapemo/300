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
end

function Dead()

end

function OnTriggerEnter(Entity)
    print("From InputTest.lua OnTriggerEnter")
end

function OnTriggerExit(Entity)
    
end

function OnContactEnter(Entity)
    
end

function OnContactExit(Entity)

end
