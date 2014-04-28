-- Cjdns admin module for Lua
-- Written by Philip Horger

common = require 'cjdns/common'

UtilFunctions = {}
UtilFunctions.__index = UtilFunctions
common.UtilFunctions = UtilFunctions

function UtilFunctions.new(ai, config)
    properties = {
        ai     = ai,
        config = config or ai.config
    }

    return setmetatable(properties, UtilFunctions)
end

function UtilFunctions:ping()
    local response, err = self.ai:call({q = "ping"})
    if response and response["q"] == "pong" then
        return true
    else
        return false, err
    end
end

function UtilFunctions:memory()
    local response, err = self.ai:auth({q = "memory"})
    if response and response.bytes then
        return response.bytes
    else
        return false, err
    end
end
