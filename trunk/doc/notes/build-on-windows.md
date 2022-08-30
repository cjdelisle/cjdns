CultureSpy worked out how to compile cjdns for Windows for Windows. So if you happen to use this broken ass OS and want to use cjdns....

# Building cjdns On Windows For Windows


Install Cygwin from [here](https://cygwin.com/install.html)

Extra packages to install, if they're not installed by default:

```
gcc-core
gcc-g++
libltdl7
mingw-binutils
mingw-gcc-core
mingw-pthreads
mingw-w32api
mingw64-i686-binutils
ming64-i686-gcc
ming64-i686-gcc-core
ming64-i686-gcc-g++
ming64-i686-headers
ming64-i686-pthreads
ming64-i686-runtime
ming64-i686-windows-default-manifest
ming64-i686-winpthreads
python
w32api-headers
w32api-runtime
windows-default-manifest
make
```

[Install node.js](http://nodejs.org/download/)


Get the [cjdns source](https://github.com/cjdelisle/cjdns)


Create `C:\tmp`

Start a cygwin session and update your environment for mingw compilation:

```
export CC=i686-w64-mingw32-gcc
export CXX=i686-w64-mingw32-g++
```

Make a small edit to the cjdns JS build system:

```
--- a/node_build/make.js
+++ b/node_build/make.js
@@ -43,7 +43,7 @@ Builder.configure({
     systemName:     SYSTEM,
     crossCompiling: process.env['CROSS'] !== undefined,
     gcc:            GCC,
-    tempDir:        '/tmp',
+    tempDir:        'C:\\tmp',
     optimizeLevel:  '-O2',
     logLevel:       process.env['Log_LEVEL'] || 'DEBUG'
 }, function (builder, waitFor) {
```
(that is, replace the line that says `tempDir: '/tmp'` with `tempDir: 'C:\\tmp'`)

This works around an issue with the detection of the tmp directory.

Also, make sure libuv will build:

`cp node_build/dependencies/libuv/Makefile.mingw node_build/dependencies/libuv/Makefile`

Edit `node_build/dependencies/libuv/Makefile` so that it will use the mingw gcc instead of the cygwin gcc:

```
15c15
< CC = gcc
---
> #CC = gcc
```

(That is, comment out the line that sets the CC variable.)

Then run `./do` to build

The system churns for a while, hopefully producing no errors. It will eventually tell you it's time to run `cjdroute`. It lies.

`cp build_win32/admin_angel_cjdroute2_c.exe cjdroute.exe`

Or put cjdroute.exe wherever you like.

Continue following [instructions here](../windows.md#run-time-dependencies)

And obviously [secure your shit](windows-firewall.md)
