-- Cjdns admin module for Lua
-- Written by Philip Horger

local bencode = require "bencode" -- https://bitbucket.org/wilhelmy/lua-bencode/
local djkson  = require "djkson"  -- http://dkolf.de/src/dkjson-lua.fsl/home
local socket  = require "socket"  -- http://w3.impa.br/~diego/software/luasocket/
local sha2    = require "sha2"    -- https://code.google.com/p/sha2/

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
        return nil, "ai:recv > " .. err
    end
    local bencoded, err = bencode.decode(retrieved)
    if bencoded then
        return bencoded
    else
        return nil, "ai:recv > " .. err
    end
end

function AdminInterface:call(request)
    local sock_obj, err = self:send(request)
    if err then
        return nil, "ai:call > " .. err
    end

    return self:recv(sock_obj)
end

function AdminInterface:getCookie()
    local cookie_response, err = self:call({ q = "cookie" })
    if not cookie_response then
        return nil, "ai:getCookie > " .. err
    end
    return cookie_response.cookie
end

function AdminInterface:auth(request)
    local funcname = request.q
    local args = {}
    for k, v in pairs(request) do
        args[k] = v
    end

    -- Step 1: Get cookie
    local cookie, err = self:getCookie()
    if err then
        return nil, err
    end

    -- Step 2: Calculate hash1 (password + cookie)
    local plaintext1 = self.password .. cookie
    local hash1 = sha2.sha256hex(plaintext1)

    -- Step 3: Calculate hash2 (intermediate stage request)
    local request = {
        q      = "auth",
        aq     = funcname,
        args   = args,
        hash   = hash1,
        cookie = cookie
    }
    local plaintext2, err = bencode.encode(request)
    if err then
        return nil, err
    end
    local hash2 = sha2.sha256hex(plaintext2)

    -- Step 4: Update hash in request, then ship it out
    request.hash = hash2
    return self:call(request)
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
