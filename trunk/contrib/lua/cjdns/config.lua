-- Cjdns admin module for Lua
-- Written by Philip Horger

common = require 'cjdns/common'

ConfigFile = {}
ConfigFile.__index = ConfigFile
common.ConfigFile = ConfigFile

function ConfigFile.new(path, no_init)
    local values = {
        path     = path,
        text     = "",
        contents = {},
    }
    setmetatable(values, ConfigFile)

    if (not no_init) then
        values:reload()
    end

    return values
end

function ConfigFile:reload()
    self:read()
    self:parse()
end

function ConfigFile:read(path)
    path = path or self.path

    local f = assert(io.open(self.path, 'r'))
    self.text = assert(f:read("*all"))
    f:close()

    self.text, _ = self.text:gsub("//[^\n]*\n", "") -- Single line comments
    self.text, _ = self.text:gsub("/%*.-%*/", "") -- Multi-line comments
end

function ConfigFile:parse()
    local obj, pos, err = dkjson.decode(self.text)
    assert(err == nil, err) -- If there is an error, die and print it

    self.contents = obj
end

function ConfigFile:save(path)
    -- Saving will obliterate comments, and table output type is inferred by
    -- table contents, since Lua does not distinguish between [] and {}

    path = path or self.path
    local savetext = assert(dkjson.encode(self.contents, { indent = true }))

    local f = assert(io.open(path, 'w'))
    f:write(savetext)
    f:close()
end

function ConfigFile:makeInterface()
    local ai = common.AdminInterface.new({ config = self })
    self:applyToInterface(ai)
    return ai
end

function ConfigFile:applyToInterface(ai)
    local adminstuff = assert(self.contents.admin)
    local bind       = assert(adminstuff.bind)
    local bindsplit  = assert(bind:find(":"))

    ai.password = assert(adminstuff.password)
    ai.host     = bind:sub(0, bindsplit - 1)
    ai.port     = bind:sub(bindsplit + 1)
    ai.config   = self
end
