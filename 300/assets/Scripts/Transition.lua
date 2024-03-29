local name = ""
local onAlive
local currAlpha

local startedTransition 

-- local transition_audio
local reverse_transition
local this  -- transition_audio

local transition_in_once = false
local transition_out_once = false

function Alive()
    gameStateSys = systemManager:mGameStateSystem()
    graphicsSys = systemManager:mGraphicsSystem()   
    onAlive = true
    currAlpha = 1.0
    speed = 0.01
    startedTransition = false

    this = Helper.GetScriptEntity(script_entity.id)
    reverse_transition = gameStateSys:GetEntityByScene("TransitionOut" , "Transition")
end

function Update()
    if onAlive == true then
        background = gameStateSys:GetEntity("TransitionBackground")
        if transition_in_once == false then 
            reverse_transition:GetAudio():SetPlay(1.0)
            transition_in_once = true
        end
        currAlpha = currAlpha - speed
        if currAlpha < 0 then
            onAlive = false
            currAlpha = 0.0
        end
        background:GetUIrenderer().mColor = Vec4.new(1.0,1.0,1.0, currAlpha)
    end

    if startedTransition == true and onAlive == false then
        if transition_out_once == false then 
            this:GetAudio():SetPlay(1.0)
            transition_out_once = true
        end
        background = gameStateSys:GetEntity("TransitionBackground")
        currAlpha = currAlpha + speed
        if currAlpha > 1.0 then
            currAlpha = 1.0
            gameStateSys:ChangeGameState(name)
        end
        background:GetUIrenderer().mColor = Vec4.new(1.0,1.0,1.0, currAlpha)
    end
end

function DialogueUpdate()
    Update()
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

function StartTransition()
    startedTransition = true
end

function SetNextGameState(str)
    name = str
end