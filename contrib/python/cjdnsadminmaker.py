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

cjdroutelocations = ["/opt/cjdns",
    os.getenv("HOME") + "/cjdns",
    os.getenv("HOME") + "/cjdns-git",
    "/usr/local/opt/cjdns"]

cjdroutelocations += os.getenv("PATH").split(":")

cjdnsadmin_path = os.path.expanduser("~/.cjdnsadmin")

cjdnsadmin = {}


def ask(question, default):
    while True:
        r = raw_input("%s " % question).lower() or default

        if r in "yn":
            return r == "y"
        else:
            print "Invalid response, please enter either y or n"


def find_cjdroute_bin():
    for path in cjdroutelocations:
        path += "/cjdroute"
        if os.path.isfile(path):
            return path

    print "Failed to find cjdroute"
    print "Please tell me where it is"
    return raw_input("ie. <cjdns git>/cjdroute: ")


def find_cjdroute_conf():
    for path in conflocations:
        if os.path.isfile(path):
            return path

    return raw_input("Can't find cjdroute.conf, please give the path to it here: ")


if os.path.isfile(cjdnsadmin_path):
    validjson = False
    try:
        cjdnsadmin = json.load(open(cjdnsadmin_path))
        validjson = True
    except ValueError:
        pass
    if validjson:
        if not ask("%s appears to be a valid JSON file. Update? [Y/n]" % cjdnsadmin_path, "y"):
            sys.exit()
    else:
        if not ask("%s appears to be a file. Overwrite? [y/N]" % cjdnsadmin_path, "n"):
            sys.exit()
else:
    print "This script will attempt to create " + cjdnsadmin_path

def validjson(conf):
    print "Making valid JSON out of " + conf
    print "First, we need to find the cleanconfig program"
    cjdroute = find_cjdroute_bin()
    print "Using " + cjdroute
    process = subprocess.Popen([cjdroute, "--cleanconf"], stdin=open(conf), stdout=subprocess.PIPE)
    cleanconf = process.stdout.read()
    try:
        return json.loads(cleanconf)
    except ValueError:
        open("debug.log", "w+").write(cleanconf)
        print "Failed to parse! Check debug.log"
        sys.exit(1)

conf = find_cjdroute_conf()
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
    adminfile = open(cjdnsadmin_path, "w+")
    adminfile.write(json.dumps(cjdnsadmin, indent=4))
    adminfile.close()
    print "Done! Give it a shot, why dont ya"
