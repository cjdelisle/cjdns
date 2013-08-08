-- Cjdns admin module for Lua
-- Written by Philip Horger

common = require 'cjdns/common'

UDPInterface = {}
UDPInterface.__index = UDPInterface
common.UDPInterface = UDPInterface

function UDPInterface.new(ai, config, ptype)
    properties = {
        ai     = ai,
        config = config or ai.config,
        ptype  = ptype or "ai"
    }

    return setmetatable(properties, UDPInterface)
end

function UDPInterface:call(name, args)
    local func = self[name .. "_" .. self.ptype]
    return func(self, unpack(args))
end

function UDPInterface:newBind(...)
    return self:call("newBind", arg)
end

function UDPInterface:beginConnection(...)
    return self:call("beginConnection", arg)
end

function UDPInterface:newBind_ai(address)
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

function UDPInterface:newBind_config(address)
    local udpif       = self.config.contents.interfaces.UDPInterface
    local new_interface = {
        bind = address,
        connectTo = {}
    }
    table.insert(udpif, new_interface)
    return (#udpif - 1), new_interface
end

function UDPInterface:newBind_perm(...)
    return
        self:newBind_config(unpack(arg)),
        self:newBind_ai(unpack(arg))
end

function UDPInterface:beginConnection_ai(pubkey, addr, password, interface)
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

function UDPInterface:beginConnection_config(pubkey, addr, password, interface)
    local udpif       = self.config.contents.interfaces.UDPInterface
    local connections = udpif[(interface or 0) + 1].connectTo
    local this_conn   = {
        password  = password,
        publicKey = pubkey
    }
    connections[addr] = this_conn
    return this_conn -- allows adding metadata fields afterwards
end

function UDPInterface:beginConnection_perm(...)
    return
        self:beginConnection_config(unpack(arg)),
        self:beginConnection_ai(unpack(arg))
end
