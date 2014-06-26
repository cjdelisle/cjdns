-- Cjdns admin module for Lua
-- Written by Philip Horger

bencode = require "bencode" -- https://bitbucket.org/wilhelmy/lua-bencode/
dkjson  = require "dkjson"  -- http://dkolf.de/src/dkjson-lua.fsl/home
socket  = require "socket"  -- http://w3.impa.br/~diego/software/luasocket/
sha2    = require "sha2"    -- https://code.google.com/p/sha2/

submodules = {
    'udp',
    'util',
    'router',

    'perm',
    'config',
    'admin'
}
for i=1,#submodules do
    require('cjdns/' .. submodules[i])
end
return require "cjdns/common"
