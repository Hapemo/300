local countObj
function Alive()
	countObj = 0
end

function Update()

	if countObj == 3 then

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

function AddObjective()
	countObj = countObj + 1
end

function RemoveObjective()
	countObj = countObj - 1
end

function GetCountObj()
    return countObj
end