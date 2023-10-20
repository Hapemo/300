Helper = {}

Helper.test = 5

function Helper.printTest()
    print("HELPER WORKS!") 
end

function Helper.GetScriptEntity(id)
    entity = Entity.new(id)
    return entity
end
