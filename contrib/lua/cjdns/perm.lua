-- Cjdns admin module for Lua
-- Written by Philip Horger

common = require 'cjdns/common'

Permanence = {}
Permanence.__index = Permanence
common.Permanence = Permanence

-- Use Permanence to apply a change to the config and the live process
-- simultaneously.

function Permanence.new(ai, config)
    properties = {
        ai     = ai,
        config = config or ai.config
    }
    return setmetatable(properties, Permanence)
end

function Permanence:ping()
    return self.ai:ping()
end

function Permanence:memory()
    return self.ai:memory()
end

function Permanence:RouterModule_lookup(address)
    return self.ai:RouterModule_lookup(address)
end
