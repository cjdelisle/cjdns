-- CJDNS admin module for Lua
-- Written by Philip Horger

require "bencode"
--require "json"
require "socket"

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

--function AdminInterface.fromConfig(path)
--end

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
    AdminInterface = AdminInterface
}
