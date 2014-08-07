# Building for Windows

* If you're trying to build **on** windows, forget it, you wouldn't build software for an iPhone
**on** an iPhone...

Install mingw32

    sudo apt-get install gcc-mingw32
    sudo apt-get install mingw-w64


Build

    SYSTEM=win32 CROSS_COMPILE=i686-w64-mingw32- ./cross-do

On Windows

    You need TAP driver, you can get it with OpenVPN https://openvpn.net/index.php/open-source/downloads.html
