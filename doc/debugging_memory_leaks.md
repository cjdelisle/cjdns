# Debugging memory leaks

Ok, so cjdns just crashed on you and printed some shit like
`Fatal error: [Out of memory, limit exceeded]` what do you do.

## Solution 1: cry
The bdfl will fix it when it happens on his laptop.

## Solution 2: find the cause
Before crashing, cjdns prints a tree containing every memory allocation, its allocator, the
parent of that allocator and so on back up to the root allocator.

Memory in cjdns is allocated in a tree structure similar to the directory structure on a filesystem.
In order to allocate memory, you need an **allocator**, allocators can spawn child allocators
and allocate memory. When an allocator is *freed*, all of its memory and all of the memory of
its children is freed in turn.

If you want to see the memory tree while cjdns is running, you can trigger a print of the tree using
the RPC call `Allocator_snapshot()`, the parameter specifies whether the individual memory
allocations should be shown or just the allocators, if cjdns has an OOM crash, it shows everything.

Example: `./tools/cexec 'Allocator_snapshot(1)'`
Note that this will print the tree to stdout because it is far too large to return in a UDP packet
and sending it in multiple requests would require taking and storing (in memory) a snapshot to
be retrieved by further requests (patches for this would be appreciated).

The memory tree will look something like the following:

        Core.c:496 [119096] bytes
          Ducttape.c:982 [1088] bytes
          | SessionManager.c:242 [864] bytes at [0x5555560579e0]
          | SessionManager.c:241 [64] bytes at [0x555555fe0690]
          | Ducttape.c:982 [160] bytes at [0x555555fe0f10]
          Timeout.c:84 [488] bytes
          | Allocator.c:657 [104] bytes at [0x555555e429e0]
          | Timeout.c:85 [224] bytes at [0x555555e8cc50]
          | Timeout.c:84 [160] bytes at [0x555555e7c460]
          Janitor.c:671 [360] bytes
          | Pinger.c:106 [1296] bytes
          | | RouterModule.c:412 [2976] bytes
          | | | UDPAddrInterface.c:96 [64] bytes at [0x55555605b710]
          | | | UDPAddrInterface.c:96 [64] bytes at [0x55555605b670]
          | | | Dict.c:132 [72] bytes at [0x555556058360]
          | | | Dict.c:150 [64] bytes at [0x555556058310]
          | | | Dict.c:132 [72] bytes at [0x5555560582c0]
          | | | Dict.c:166 [64] bytes at [0x555556058270]
          | | | Dict.c:132 [72] bytes at [0x555556058220]
          | | | Dict.c:150 [64] bytes at [0x5555560581d0]
          | | | RouterModule.c:414 [104] bytes at [0x555556058160]
          | | | Message.h:58 [80] bytes at [0x555556058100]
          | | | Message.h:57 [2096] bytes at [0x55555605aca0]
          | | | RouterModule.c:412 [160] bytes at [0x555556058050]
          | | Timeout.c:84 [488] bytes
          | | | Allocator.c:657 [104] bytes at [0x555555ff5040]
          | | | Timeout.c:85 [224] bytes at [0x555555fb2d00]
          | | | Timeout.c:84 [160] bytes at [0x555555ff4f90]

In this simplified example, Core.c:496 is the root allocator, the grand-daddy of all memory
allocations. The RPC call `Core_exit()` actually works by freeing this allocator. Everything
`Janitor.c:84` is a child of the root allocator and `Pinger.c:106` a child of that.
`UDPAddrInterface.c:96 [64] bytes at [0x55555605b710]` is an allocation of a block of memory
which you can see from its size (64 bytes) and its memory address.

In each case, the allocator and the memory block, the filename and line number (eg: `Core.c:496`)
is a place in a file where one can find the call to the memory operation.
Usually either `Allocator_child()` or `Allocator_malloc()`.

### Finding memory leaks
If you encounter an OOM crash, there is probably a leak. The easiest way to find this is to take
the entire memory tree trace (probably huge) and copy it into a text document. Remove everything
below `----- end cjdns memory snapshot -----` and everything above
`----- cjdns memory snapshot -----` then save the file (perhaps using the name `~/cjdns_memdump`).

On the command line, run the following command:

        cat ~/cjdns_memdump | sed -n -e 's/.* \([a-zA-Z0-9_]*\.[ch]\:[0-9]*\) .*$/\1/p' | sort | uniq -c | sort -n

This will show a list of each location (in the code) where memory is allocated and the number of
currently active allocations (or allocators) which were allocated in that spot.

Here is the bottom of the output from the above command. The top is mostly locations which only
have one currently active allocation.

            8 RumorMill.c:136
           57 Message.h:57
           57 Message.h:58
           57 Pinger.c:116
           57 RouterModule.c:414
           57 RouterModule.c:580
           66 Dict.c:199
          114 Pinger.c:106
          114 RouterModule.c:412
          114 UDPAddrInterface.c:96
          118 NodeStore.c:1461
          121 EncodingScheme.c:377
          121 EncodingScheme.c:383
          172 Dict.c:95
          180 Dict.c:150
          204 SessionManager.c:183
          206 CryptoAuth.c:1207
          236 NodeStore.c:1460
          249 String.c:31
          249 String.c:39
          293 Dict.c:166
          408 SessionManager.c:172
          539 Dict.c:132
          725 NodeStore.c:113
         2392 Timeout.c:85
         2470 Allocator.c:657
         4784 Timeout.c:84
         7428 SessionManager.c:241
         7428 SessionManager.c:242
        14856 Ducttape.c:982

All the way at the bottom is an anomaly, a single location in the source code which contains
twice as many allocations as the next greatest number. Investigating that I found a dumb mistake
which [some idiot](https://github.com/cjdelisle/cjdns/commit/507223dac10690f562b91d8ec84ce2f7a41df5ad)
made while working on the source.
