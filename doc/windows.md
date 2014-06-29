# Building for Windows

* If you're trying to build **on** windows, forget it, you wouldn't build software for an iPhone
**on** an iPhone...

Install mingw32

    sudo apt-get install gcc-mingw32
    sudo apt-get install mingw-w64


Build

    SYSTEM=win32 CROSS=1 CC=i686-w64-mingw32-gcc AR=i686-w64-mingw32-ar RANLIB=i686-w64-mingw32-ranlib ./do
