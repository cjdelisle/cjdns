-- CJDNS admin module for Lua
-- Written by Philip Horger

local bencode = require "bencode" -- https://bitbucket.org/wilhelmy/lua-bencode/
local djkson  = require "djkson"  -- http://dkolf.de/src/dkjson-lua.fsl/home
local socket  = require "socket"  -- http://w3.impa.br/~diego/software/luasocket/

AdminInterface = {}
AdminInterface.__index = AdminInterface

ConfigFile = {}
ConfigFile.__index = ConfigFile

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
    local obj, pos, err = djkson.decode(self.text)
    assert(err == nil, err) -- If there is an error, die and print it

    self.contents = obj
end

function ConfigFile:save(path)
    -- Saving will obliterate comments, and table output type is inferred by
    -- table contents, since Lua does not distinguish between [] and {}

    path = path or self.path
    local savetext = assert(djkson.encode(self.contents, { indent = true }))

    local f = assert(io.open(path, 'w'))
    f:write(savetext)
    f:close()
end

function ConfigFile:makeInterface()
    local ai = AdminInterface.new({ config = self })
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

function AdminInterface.new(properties)
    properties = properties or {}

    properties.host     = properties.host or "127.0.0.1"
    properties.port     = properties.port or 11234
    properties.password = properties.password or nil
    properties.config   = properties.config   or ConfigFile.new("/etc/cjdroute.conf", false)
    properties.timeout  = properties.timeout  or 2

    return setmetatable(properties, AdminInterface)
end


function AdminInterface:getIP()
    return socket.dns.toip(self.host)
end

function AdminInterface:send(object)
    local bencoded, err = bencode.encode(object)
    if err then
        return nil, err
    end

    local sock_obj = assert(socket.udp())
    sock_obj:settimeout(self.timeout)

    local _, err = sock_obj:sendto(bencoded, assert(self:getIP()), self.port)
    if err then
        return nil, err
    end

    return sock_obj
end

function AdminInterface:recv(sock_obj)
    local retrieved, err = sock_obj:receive()
    if not retrieved then
        return nil, err
    end
    return bencode.decode(retrieved)
end

function AdminInterface:call(object)
    local sock_obj, err = self:send(object)
    if err then
        return nil, err
    end

    return self:recv(sock_obj)
end

function AdminInterface:ping()
    local response, err = self:call({q = "ping"})
    if response and response["q"] == "pong" then
        return true
    else
        return false, err
    end
end

return {
    AdminInterface = AdminInterface,
    ConfigFile     = ConfigFile,
}
