cjdns = require "cjdns/init"

-- CONFIG

confpath = "/etc/cjdroute.conf"

-- SETUP

print("Using config file: " .. confpath)
conf = cjdns.ConfigFile.new(confpath)
ai   = conf:makeInterface()

-- TESTS

function testInterface(iface)
    local testip = "fc5d:baa5:61fc:6ffd:9554:67f0:e290:7535"

    print("Can I ping the admin server?")
    print("ping", iface.util:ping())

    print("Memory usage in bytes.")
    print("bytes", iface.util:memory())

    print("Route lookup?")
    print("lookup", iface.router:lookup(testip))
    print("Router ping?")
    print("ping", iface.router:pingNode(testip) .. "ms")
    print("Router ping by lookup address...")
    local testpath, err = iface.router:lookup(testip)
    if testpath then
        print("Path: " .. testpath)
        print("ping", iface.router:pingNode(testpath) .. "ms")
    else
        print("testpath failed: " .. err)
    end
end

print("\nAdminInterface ==============================================")
testInterface(ai)

print("Can I get a cookie?")
print("cookie", ai:getCookie())

print("Can I ping via auth?")
auth_ping = ai:auth({q = "ping"})
print("ping", (auth_ping and auth_ping.q == "pong"))


print("\nPermanence ==================================================")
testInterface(ai.perm)

print("\nConfigFile ==================================================")
print("Exporting compressed conf to test.conf...")
print("save", conf:save("test.conf"))
