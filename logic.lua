-- logic.lua
local time_acc = 0
local current = {r = 1.0, g = 0.0, b = 0.0}
local target = {r = 0.0, g = 1.0, b = 0.0}
local start_col = {r = 1.0, g = 0.0, b = 0.0}

local function lerp(a, b, t)
    return a + (b - a) * t
end

function GetNextColor(dt)
    time_acc = time_acc + dt
    
    if time_acc >= 1.0 then
        time_acc = 0
        start_col.r, start_col.g, start_col.b = target.r, target.g, target.b
        
        -- Новый рандомный цвет
        target.r = math.random()
        target.g = math.random()
        target.b = math.random()
    end
    
    current.r = lerp(start_col.r, target.r, time_acc)
    current.g = lerp(start_col.g, target.g, time_acc)
    current.b = lerp(start_col.b, target.b, time_acc)
    
    return current.r, current.g, current.b
end
