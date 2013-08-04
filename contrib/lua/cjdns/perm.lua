-- Cjdns admin module for Lua
-- Written by Philip Horger

common = require 'cjdns/common'

Permanence = {}
Permanence.__index = Permanence
common.Permanence = Permanence

-- Use Permanence to apply a change to the config and the live process
-- simultaneously. So most stuff, you'll do through this.

-- Right now, there are no functions that affect config, so it has no
-- special overrides. Using it is effectively equal to using
-- AdminInterface directly.

function Permanence.new(ai, config)
    properties = {
        ai     = ai,
        config = config or ai.config,

        util   = ai.util,
        router = ai.router
    }
    return setmetatable(properties, Permanence)
end
