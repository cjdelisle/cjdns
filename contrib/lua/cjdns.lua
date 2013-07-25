-- Cjdns admin module for Lua
-- Written by Philip Horger

local bencode = require "bencode" -- https://bitbucket.org/wilhelmy/lua-bencode/
local djkson  = require "djkson"  -- http://dkolf.de/src/dkjson-lua.fsl/home
local socket  = require "socket"  -- http://w3.impa.br/~diego/software/luasocket/

AdminInterface = {}
AdminInterface.__index = AdminInterface

function AdminInterface.new(host, port, password, configpath)
    local values = {
        host       = host,
        port       = port,
        password   = password,
        configpath = configpath,
    }
    return setmetatable(values, AdminInterface)
end

function getConfigJSON(path)
    local f = assert(io.open(path, 'r'))
    local config = assert(f:read("*all"))
    f:close()

    config, _ = config:gsub("//[^\n]*\n", "")
    return config
end

function AdminInterface.fromConfig(path)
    config = getConfigJSON(path)

    local obj, pos, err = djkson.decode(config)
    assert(err == nil, err) -- If there is an error, die and print it

    local ai_table   = {}
    local adminstuff = assert(obj.admin)
    local bind       = assert(adminstuff.bind)
    local bindsplit  = assert(bind:find(":"))

    ai_table.password   = assert(adminstuff.password)
    ai_table.host       = bind:sub(0, bindsplit - 1)
    ai_table.port       = bind:sub(bindsplit + 1)
    ai_table.configpath = path

    return setmetatable(ai_table, AdminInterface)
end

function AdminInterface:getIP()
    local ip = socket.dns.toip(self.host)
    assert(ip)
    return ip
end

function AdminInterface:send(object)
    local bencoded = assert(bencode.encode(object))
    local sock_obj = assert(socket.udp())
    assert(sock_obj:sendto(bencoded, self:getIP(), self.port))
    return sock_obj
end

function AdminInterface:recv(sock_obj)
    return assert(bencode.decode(sock_obj:receive()))
end

function AdminInterface:call(object)
    local sock_obj = self:send(object)
    return self:recv(sock_obj)
end

function AdminInterface:ping()
    local response = self:call({q = "ping"})
    return (response["q"] == "pong")
end

return {
    AdminInterface = AdminInterface,
    getConfigJSON  = getConfigJSON,
}
