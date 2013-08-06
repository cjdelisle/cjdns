-- Cjdns admin module for Lua
-- Written by Philip Horger

common = require 'cjdns/common'

UDPInterface = {}
UDPInterface.__index = UDPInterface
common.UDPInterface = UDPInterface

function UDPInterface.new(ai, config)
    properties = {
        ai     = ai,
        config = config or ai.config
    }

    return setmetatable(properties, UDPInterface)
end

function UDPInterface:newBind(address)
    local response, err = self.ai:auth({
        q = "UDPInterface_new",
        bindAddress = address
    })
    if not response then
        return nil, err
    elseif response.error ~= "none" then
        return nil, response.error
    elseif response.interfaceNumber then
        return response.interfaceNumber
    else
        return nil, "bad response format"
    end
end

function UDPInterface:beginConnection(pubkey, addr, password, interface)
    local request = {
        q = "UDPInterface_beginConnection",
        publicKey = pubkey,
        address   = addr,
        password  = password
    }
    if interface then
        request.interfaceNumber = interface
    end

    local response, err = self.ai:auth(request)
    if not response then
        return nil, err
    elseif response.error == "none" then
        -- Unfortunately, no real success indicator either.
        return "No error"
    else
        return nil, response.error
    end
end
