-- Cjdns admin module for Lua
-- Written by Philip Horger

common = require 'cjdns/common'

AdminInterface = {}
AdminInterface.__index = AdminInterface
common.AdminInterface = AdminInterface

function AdminInterface.new(properties)
    properties = properties or {}

    properties.host     = properties.host or "127.0.0.1"
    properties.port     = properties.port or 11234
    properties.password = properties.password or nil
    properties.config   = properties.config   or common.ConfigFile.new("/etc/cjdroute.conf", false)
    properties.timeout  = properties.timeout  or 2

    properties.util     = common.UtilFunctions.new(properties)
    properties.router   = common.RouterFunctions.new(properties)
    properties.udp      = common.UDPInterface.new(properties)
    properties.perm     = common.Permanence.new(properties)

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
