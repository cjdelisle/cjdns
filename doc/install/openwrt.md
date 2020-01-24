# Running cjdns on commodity routers w/ OpenWrt

From: https://www.reddit.com/r/darknetplan/comments/2lmpqg/running_cjdns_on_commodity_routers_w_openwrt/

Since the assumption pops up every other day that cjdns wouldn't run on commodity routers:

Yes, it does run fine on OpenWrt and is used in production by the Seattle and New York City meshnet communities, as well as Freifunk Hamburg and many individuals.

- The source code, GPL licensed: https://github.com/SeattleMeshnet/meshbox#readme
- If you already have a router, check how well it is supported by OpenWrt. Look for anything supported by OpenWrt 14.07 a.k.a. Barrier Breaker a.k.a. BB: http://wiki.openwrt.org/toh/start
- If you plan to buy something, look for Ubiquiti and TP-Link routers/antennas.
- Follow the installation instructions in the Readme file mentioned above, to compile an OpenWrt firmware image including cjdns and the LuCI web interface.

We will soon be offering prebuilt firmware images for a few selected devices, including TP-Link TL-WDR3600/4300/4900, Netgear WNR2000v3, Raspberry Pi, and BeagleBone Black. There will also be Docker containers of the firmware image, for easy local development and spinning up of virtual test topologies.

You will find help in #cjdns on EFnet if you run into problems trying out cjdns with OpenWrt.
