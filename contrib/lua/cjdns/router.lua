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

function RouterFunctions:pingNode(path, timeout)
    local request = {
        q = "RouterModule_pingNode",
        path = path
    }
    if timeout then
        request.timeout = timeout
    end

    local response, err = self.ai:auth(request)
    if not response then
        return nil, err
    elseif response.error then
        return nil, response.error
    elseif response.result then
        if response.result == "timeout" then
            return nil, "timeout"
        else
            return response.ms
        end
    else
        return nil, "bad response format"
    end
end
