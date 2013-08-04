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
