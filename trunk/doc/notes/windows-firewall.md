# Purpose

With the Windows population of Hyperboria set to grow, some basic security advice may be needed. Because many of Windows' services cannot be disabled without sacrificing desktop functionality, we will rely on the Windows Firewall as a first line of defense,

## Configuration

On a general purpose desktop PC, it is possible for the firewall to become disabled for any number of reasons. This is generally considered bad practice but not fatal on a LAN. Cjdns will expose the PC to a large network where every node is considered local. The potential for hostile traffic is much higher. Therefore, we must make sure the firewall is enabled:


1. Right click network icon in the system tray and select "Open Network and Sharing Center".
2. On the left pane of the window, click "Change adapter settings".
3. Make note of the name of TAP adapter and hit the Back button.
4. Make sure the TAP adapter is listed as being in a "Public network" and that your LAN is listed as anything other than public. This procedure varies on different Windows versions:
  * On Windows 7, you should be able to click the "Private" or "Work" indicator and select a new network type.
  * On Windows 8, you need to enable Network Discovery to make a network "Private".
5. On the left pane of the window, click "Windows Firewall".
6. On the left pane of the window, click "Tuen Windows Firewall on or off".
7. Make sure that the firewall is on for Public networks.
8. Make sure that "Block all incoming connections..." is unchecked under Public networks.
9. Make sure that "Notify me..." is checked under Public networks.
10. Accept your changes, provide the Administrator password if needed.
11. On the left pane of the window, click "Advanced settings".
12. On the left pane of the window, click "Inbound Rules". (Here you will probably see an overwhelming amount of configuration options. Windows software is allowed to programmatically add firewall rules, sometimes without the approval of the user. Each of these is a potential attack vector, so we need to disable most of them.)
13. On the right pane of the window, "Filter by profile: Public" and "Filter by state: Enabled"
14. Sort by the "Action" column so that "Allowed" is at the top.
15. For each enabled allow rule, you must do the following:
  * Ignore "Core Networking - ...." rules.
  * Ignore "File and Print Sharing (Echo Request ...)" rules.
  * For any rule that is in more than one profile:
    + Double click it.
    + Switch to the Advanced tab.
    + Uncheck "Public".
  * For any rule that is in "Public" profile only, disable it.
    + This can be done in batches, so it's a little less tedious.
16. Done!

This leaves your PC able to participate in basic networking functions like responding to pings, but closes all services to the Cjdns network.

