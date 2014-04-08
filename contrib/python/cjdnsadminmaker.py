#!/usr/bin/env python

import json
import os
import sys
import subprocess


# possibly search for running cjdroute processes and check the same folder as they're in
# and/or running find on the home folder

## Wanted: Everyone's favorite place to store their shit.
conflocations = ["/etc/cjdroute.conf",
    os.getenv("HOME") + "/cjdroute.conf",
    os.getenv("HOME") + "/cjdns/cjdroute.conf",
    "/usr/local/opt/cjdns/cjdroute.conf"]
    
cjdnslocations = ["/opt/cjdns",
    os.getenv("HOME") + "/cjdns",
    os.getenv("HOME") + "/cjdns-git",
    "/usr/local/opt/cjdns"]

cjdnsadmin = {}

if os.path.isfile(os.getenv("HOME") + "/.cjdnsadmin"):
    validjson = False
    try:
        cjdnsadmin = json.load(open(os.getenv("HOME") + "/.cjdnsadmin"))
        validjson = True
    except ValueError:
        pass
    if validjson:
        while True:
            r = raw_input(os.getenv("HOME") + "/.cjdnsadmin appears to be a valid JSON file. Update? [Y/n] ")
            if r.lower() == "n":
                sys.exit()
            elif r.lower() == "y" or r == "":
                break
            else:
                print "Invalid response, please enter either y or n"    
    else:
        while True:
            r = raw_input(os.getenv("HOME") + "/.cjdnsadmin appears to be a file. Overwrite? [y/N] ")
            if r.lower() == "n" or r == "":
                sys.exit()
            elif r.lower() == "y":
                break
            else:
                print "Invalid response, please enter either y or n"
else:
    print "This script will attempt to create " + os.getenv("HOME") + "/.cjdnsadmin"
    
def validjson(conf):
    print "Making valid JSON out of " + conf
    print "First, we need to find the cleanconfig program"
    cleanconfig = ""
    i = 0
    while not os.path.isfile(cleanconfig):
        if i < len(cjdnslocations):
            cleanconfig = cjdnslocations[i] + "/cleanconfig"
            i += 1
        else:
            print "Failed to find cleanconfig"
            print "Please tell me where it is"
            cleanconfig = raw_input("HINT: <cjdns git>/cleanconfig: ")
    print "Using " + cleanconfig
    process = subprocess.Popen([cleanconfig], stdin=open(conf), stdout=subprocess.PIPE)
    cleanconf = process.stdout.read()
    try:
        return json.loads(cleanconf)
    except ValueError:
        open("debug.log", "w+").write(cleanconf)
        print "Failed to parse! Check debug.log"
        sys.exit(1)
    
done = False
i = 0
while not done:
    if i <= len(conflocations):
        conf = conflocations[i]
        i += 1
    else:
        conf = raw_input("Can't find cjdroute.conf, please give the path to it here: ")
        sys.exit(1)
    if os.path.isfile(conf):
        print "Loading " + conf
        try:
            cjdrouteconf = json.load(open(conf))
        except ValueError:
            cjdrouteconf = validjson(conf)
        except IOError:
            print "Error opening " + conf + ". Do we have permission to access it?"
            print "Hint: Try running this as root"
            sys.exit(1)
        addr, port = cjdrouteconf['admin']['bind'].split(":")
        cjdnsadmin["addr"] = addr
        cjdnsadmin["port"] = int(port)
        cjdnsadmin["password"] = cjdrouteconf['admin']['password']
        cjdnsadmin["config"] = conf
        adminfile = open(os.getenv("HOME") + "/.cjdnsadmin", "w+")
        adminfile.write(json.dumps(cjdnsadmin, indent=4))
        adminfile.close()
        print "Done! Give it a shot, why dont ya"
        done = True
