test = 5.05

function Alive()

end

function Update()
    
end

function Dead()

end

function OnTriggerEnter(Entity)
    print("OnTriggerEnter")
end

function OnTrigger(Entity)
    print(Entity)
end

function OnTriggerExit(Entity)
    print("OnTriggerExit")
end
