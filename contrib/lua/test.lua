cjdns = require "cjdns/init"

-- CONFIG

confpath = "/etc/cjdroute.conf"

-- SETUP

print("Using config file: " .. confpath)
conf = cjdns.ConfigFile.new(confpath)
ai   = conf:makeInterface()

-- TESTS

function testInterface(iface)
    print("Can I ping the admin server?")
    print("ping", iface.util:ping())

    print("Memory usage in bytes.")
    print("bytes", iface.util:memory())

    print("Route lookup?")
    print("lookup", iface.router:lookup("fc5d:baa5:61fc:6ffd:9554:67f0:e290:7535"))
end

function testAdmin(ai)
    testInterface(ai)

    print("Can I get a cookie?")
    print("cookie", ai:getCookie())

    print("Can I ping via auth?")
    auth_ping = ai:auth({q = "ping"})
    print("ping", (auth_ping and auth_ping.q == "pong"))
end

function testPerm(perm)
    testInterface(perm)

    print("Attempting to open new UDP interface")
    print("serve", perm.udp:newBind("127.0.0.1:20"))

    print("Adding remote connection to UDP interface")
    print("connect", perm.udp:beginConnection(
        "v0zyvrjuc4xbzh4n9c4k3qpx7kg8xgndv2k45j9nfgb373m8sss0.k",
        "192.168.0.2:10000",
        "null"
    ))
end

print("\nAdminInterface ==============================================")
testAdmin(ai)

print("\nPermanence ==================================================")
testPerm(ai.perm)

print("\nConfigFile ==================================================")
print("Exporting compressed conf to test.conf...")
print("save", conf:save("test.conf"))
