cjdns = require "cjdns"

-- CONFIG

confpath = "/etc/cjdroute.conf"

-- SETUP

print("Using config file:", confpath)
conf = cjdns.ConfigFile.new(confpath)
ai   = conf:makeInterface()

-- TESTS

print("Can I ping the admin server?")
print("ping", ai:ping())

print("Memory usage in bytes.")
print("bytes", ai:memory())

print("Can I get a cookie?")
print("cookie", ai:getCookie())

print("Can I ping via auth?")
auth_ping = ai:auth({q = "ping"})
print("ping", (auth_ping and auth_ping.q == "pong"))

print("Exporting compressed conf to test.conf...")
print(conf:save("test.conf"), "done")
