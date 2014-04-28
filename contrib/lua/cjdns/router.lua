-- Cjdns admin module for Lua
-- Written by Philip Horger
-- hacked up dumpTable, switchpinger, peerstats
-- and other oddities by William Fleurant
common = require 'cjdns/common'

RouterFunctions = {}
RouterFunctions.__index = RouterFunctions
common.RouterFunctions = RouterFunctions

function RouterFunctions.new(ai, config)
    properties = {
        ai     = ai,
        config = config or ai.config
    }

    return setmetatable(properties, RouterFunctions)
end

function RouterFunctions:dumpTable(page)
    local response, err = self.ai:auth({
        q = "NodeStore_dumpTable()",
        page = page,
    })
    return response.routingTable
end


function RouterFunctions:lookup(address)
    local response, err = self.ai:auth({
        q = "RouterModule_lookup",
        address = address
    })
    if not response then
        return nil, err
    elseif response.error ~= "none" then
        return nil, response.error
    elseif response.result then
        return response.result
    else
        return nil, "bad response format"
    end
end


function RouterFunctions:availableFunctions(page)
    local response, err = self.ai:auth({q = "Admin_availableFunctions"})
    print (err)
    print (response)
end

function RouterFunctions:switchpinger(path, data, timeout)
    local response, err = self.ai:auth({
        q = "SwitchPinger_ping",
        path = path,
        data = 0,
        timeout = ''
    })
    for k,v in pairs(response) do print(k,v) end
    return response
end

function RouterFunctions:peerStats(page)
    if page then page = page else page = 0 end
    while page do
        local response, err = self.ai:auth({
            q = "InterfaceController_peerStats",
            page = page,
        })
        for pubkey,switch in pairs(response.peers,response.peers) do
            print (response.peers[pubkey]['publicKey'],
                   response.peers[pubkey]['switchLabel'])
        end
        if response.more then
            page = page + 1
        else
            page = nil
        end
    end
    return response
end

function RouterFunctions:AuthorizedPasswords_list()
    local response, err = self.ai:auth({
        q = "AuthorizedPasswords_list()"
    })
    print("AuthorizedPasswords_list()")
    return reponse
end

function RouterFunctions:pingNode(path, timeout)
    local request = {
        q = "RouterModule_pingNode",
        path = path
    }
    if timeout then
        request.timeout = timeout
    end

    local response, err = self.ai:auth(request)
    if not response then
        return nil, err
    elseif response.error then
        return nil, response.error
    elseif response.result then
        if response.result == "timeout" then
            return nil, "timeout"
        else
            return response.ms
        end
    else
        return nil, "bad response format"
    end
end


function RouterFunctions:memory(page)
    local response, err = self.ai:call({q = "Admin_availableFunctions", page=page})
    for key,value in pairs(response.availableFunctions) do print(key) end
    print(response)
end