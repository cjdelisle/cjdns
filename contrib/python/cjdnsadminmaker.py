#!/usr/bin/env python2

"""
Automagically generate a .cjdnsadmin file.

Searches around for cjdroute.conf and the cjdns executable, cleans the config
into proper JSON, and saves just the RPC admin info to a file. By default this
is ~/.cjdnsadmin, but you can specify any file you want.
"""

import json
import os
import sys
import subprocess


# possibly search for running cjdroute processes and check the same folder as they're in
# and/or running find on the home folder

## Wanted: Everyone's favorite place to store their shit.
conflocations = ["/etc/cjdroute.conf",
    "~/cjdroute.conf",
    "~/cjdns/cjdroute.conf",
    "/usr/local/opt/cjdns/cjdroute.conf"]

cjdroutelocations = ["/opt/cjdns",
    "~/cjdns",
    "~/cjdns-git",
    "/usr/local/opt/cjdns"]

cjdroutelocations += os.getenv("PATH").split(":")

if len(sys.argv) == 0:
    # Write the file in the default location
    cjdnsadmin_path = os.path.expanduser("~/.cjdnsadmin") 
else:
    # Write the file in some other location
    cjdnsadmin_path = sys.argv[1]


def ask(question, default):
    while True:
        r = raw_input("%s " % question).lower() or default

        if r in "yn":
            return r == "y"
        else:
            print "Invalid response, please enter either y or n"


def find_cjdroute_bin():
    for path in cjdroutelocations:
        path = os.path.expanduser(path) + "/cjdroute"
        if os.path.isfile(path):
            return path

    print "Failed to find cjdroute"
    print "Please tell me where it is"
    return raw_input("ie. <cjdns git>/cjdroute: ")


def find_cjdroute_conf():
    for path in conflocations:
        path = os.path.expanduser(path)
        if os.path.isfile(path):
            return path

    return raw_input("Can't find cjdroute.conf, please give the path to it here: ")


def load_cjdroute_conf(conf):
    print "Loading " + conf
    try:
        with open(conf) as conffile:
            return json.load(conffile)
    except ValueError:
        return cleanup_config(conf)
    except IOError:
        print "Error opening " + conf + ". Do we have permission to access it?"
        print "Hint: Try running this as root"
        sys.exit(1)


def cleanup_config(conf):
    print "Making valid JSON out of " + conf
    print "First, we need to find the cleanconfig program"
    cjdroute = find_cjdroute_bin()
    print "Using " + cjdroute
    process = subprocess.Popen([cjdroute, "--cleanconf"], stdin=open(conf), stdout=subprocess.PIPE)
    try:
        return json.load(process.stdout)
    except ValueError:
        print "Failed to parse! Check:"
        print "-" * 8
        print "{} --cleanconf < {}".format(cjdroute, conf)
        print "-" * 8
        sys.exit(1)


try:
    with open(cjdnsadmin_path) as cjdnsadmin_file:
        json.load(cjdnsadmin_file)

    if not ask("%s appears to be a valid JSON file. Update? [Y/n]" % cjdnsadmin_path, "y"):
        sys.exit()
except ValueError:
    if not ask("%s appears to be a file. Overwrite? [y/N]" % cjdnsadmin_path, "n"):
        sys.exit()
except IOError:
    print "This script will attempt to create " + cjdnsadmin_path


conf = find_cjdroute_conf()
cjdrouteconf = load_cjdroute_conf(conf)

addr, port = cjdrouteconf['admin']['bind'].split(":")

cjdnsadmin = {}
cjdnsadmin["addr"] = addr
cjdnsadmin["port"] = int(port)
cjdnsadmin["password"] = cjdrouteconf['admin']['password']
cjdnsadmin["config"] = conf
with open(cjdnsadmin_path, "w+") as adminfile:
    json.dump(cjdnsadmin, adminfile, indent=4)
print "Done! Give it a shot, why dont ya"
