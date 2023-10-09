totaltimer = 100

function Alive()
    _G.cake = 10000000
    print("hihihihi")
end

function Update()
    --Working
    --systemManager.ecs:NewEntity();

    --Test ScriptStart and ScriptUpdate functions
    --print("FLYYYYYY.lua !!")

    timer = timer - 1

    if (timer == 0) then
        timer = totaltimer
    end
    print(timer)
    print(_G.cake)
end

function Dead()

end
