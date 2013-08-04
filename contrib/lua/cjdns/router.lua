-- Cjdns admin module for Lua
-- Written by Philip Horger

common = require 'cjdns/common'

RouterFunctions = {}
RouterFunctions.__index = RouterFunctions
common.RouterFunctions = RouterFunctions

function RouterFunctions.new(ai, config)
    properties = {
        ai     = ai,
        config = config or ai.config
    }

    return setmetatable(properties, RouterFunctions)
end

function RouterFunctions:lookup(address)
    local response, err = self.ai:auth({
        q = "RouterModule_lookup",
        address = address
    })
    if not response then
        return nil, err
    elseif response.error ~= "none" then
        return nil, response.error
    elseif response.result then
        return response.result
    else
        return nil, "bad response format"
    end
end
