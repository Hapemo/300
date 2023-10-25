function Alive()

end

function Update()
    
end

function Dead()

end

function OnTriggerEnter(Entity)
    print("From CollideTest.lua OnTriggerEnter")
end

function OnTrigger(Entity)
    print(Entity)
end

function OnTriggerExit(Entity)
    print("OnTriggerExit")
end

function OnContactEnter(Entity)
end

function OnContact(Entity)
    print("From CollideTest.lua")
end

function OnContactExit(Entity)

end
