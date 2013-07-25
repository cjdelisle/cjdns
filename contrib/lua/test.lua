cjdns = require "cjdns"

confpath = "/etc/cjdroute.conf"

print("Using config file:", confpath)
conf = cjdns.ConfigFile.new(confpath)
ai   = conf:makeInterface()

print("Can I ping the admin server?")
print("ping", ai:ping())
print("Exporting compressed conf to test.conf...")
print(conf:save("test.conf"), "done")
