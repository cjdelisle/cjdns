Patches are very much welcome!
==============================

If you want to get a change included in cjdns, the best thing to do is start by
asking in IRC if the change fits the spirit of the project, then developing
your change in your own git tree and then asking for it to be merged in with
the others.

Small patches are easy to include and large ones are hard to validate. Consider
breaking your evil plans up into a bunch of nice little changes which are easy
to understand and prove safe.


Codestyle:
----------

* Indentation: 4 spaces, tabs are not in the codebase.
* Trailing whitespace is not in the codebase, Windows users make sure you have
  git configured to remove carriage return characters as lines in the codebase
  are \n delineated.
* File names and structures are CamelCase with first letter capital.
* All globally visible functions shall begin with the name of the file in which
  they are defined followed by an underscore and then the name of the function.
  AKA `ThingThatDoesStuff_doStuff()`
* Functions and local variables shall use camelCase names with first letter
  lowercase.
* Structures declared in header files must begin with, or be, the name of the
  header in which they are declared.
* Global variables as well as static local variables are forbidden. Constants
  are acceptable.
* All preprocessor definitions in header files must contain the name of the
  header file followed by an underscore and the definition name in all capitals
  AKA `#define SillyMath_VALUE_OF_PI 3` or `#define SillyMath_DIVIDE(a,b) (a /
  b)` it is sometimes acceptable for macros to use camel case as is done in
  Endian.h, use judgement.

If there is a better way, come to irc and announce it, this code style has
evolved to where it is now.

Assert.h
--------

Lining your code with assertions is great! You'll find a few macros in Assert.h
to help you. If an assertion is cheap or in a cold codepath or you otherwise feel
it's important that it's never skipped, use `Assert_true()`, if you want the
assertion to be skipped on small hardware where `-DPARANOIA` is disabled, use
`Assert_ifParanoid()`, if your assertion might be triggered by "bad nodes" in a
realistic network and is for simulation only, use `Assert_ifTesting()` and it
will only be included if `-DTESTING` is passed. If your assertion has
side-effects, you must use `Assert_true()` because these are macros which
are completely removed if they are disabled.


Testing
-------

Any file in a /test/ subdirectory which ends with `_test.c` will be compiled as
a test and added to the testing regime through some nodejs hackery. you might
Tests can fail by returning non-zero or using Assert_true() statements, whatever
makes sense.

*All patches which add tests will be addressed before any patches which don't.*


Debugging
---------

        sudo gdb ./cjdroute -ex 'set follow-fork-mode child' -ex 'r < /etc/cjdroute.conf'

If it crashes, type `backtrace` to get some useful information.
The backtrace will show where in the program it crashed and where called that
and where called that, etc. Down the left side of the screen are numbers next
to the function names, you can select one of these for further inspection using
the `select-frame` command. Once you've selected a frame you can print arguments
to the function or local variables (maybe not if compiler optimization was
enabled!). The `print` command will help you extract the value of a local variable,
an argument to the function or really anything. In gdb, tab complete works for both
commands and variables/arguments so if in doubt, hit tab :)

To stop the program in the debugger, use ctrl+c, this will put you in the debugger shell.

Once in the debugger shell, to quit the debugger use ctrl+d, if the program is
running it will prompt you, another ctrl+d will be taken as a "yes, please quit".

### Valgrind

Generally doesn't work because Poly1305 (on most processors) does crypto on floating point
registers and valgrind interprets the assembly and does not properly interpret floating
point instructions so under valgrind tests will fail and the engine will not run correctly.
See: http://www.valgrind.org/docs/manual/manual-core.html#manual-core.limits


Profiling
---------

The best way to profile cjdns is using Brendan Gregg's FlameGraph generator.
http://www.brendangregg.com/FlameGraphs/cpuflamegraphs.html
You can do this on Linux using the `perf` utility.

    sudo perf record -a -g -F 997 -p `pidof cjdroute`
    # let this run for a while, put cjdroute through some exercises
    <ctrl+c>
    sudo perf script | ../FlameGraph/stackcollapse-perf.pl > ./cjdns-stackcollapse.out
    ../FlameGraph/flamegraph.pl < ./cjdns-stackcollapse.out > ./cjdns-stackcollapse.svg
    chromium ./cjdns-stackcollapse.svg


Simulating
----------

Cjdns comes with it's own simulator, it will create *n* nodes and link them together
however you wish. It's like having many cjdns processes all running together but they're
all in the same process so it is much more efficient. You can set admin credentials on
one node and then use the admin tools to access it as you would an ordinary router.
You will however need private keys whose public keys hash to ip addresses beginning with
fc. To make these keys, use the `makekeys` utility.

    ./makekeys | head -n 32 > keys.txt

To convert the list of keys into a simulator configuration, use `makesim.js`, note there
are interesting constants inside of `makesim.js` which you might want to alter.

    node ./tools/lib/makesim.js keys.txt > ~/my-cjdns-simulation.json

Once you have a simulation setup, you may want to add your admin credentials to one of
the nodes so you can inspect it, dump the table, etc...

Example simulation config entry with added admin block:

```javascript
    "fc5c:0537:606a:3d7e:c9f0:2103:4dcd:6bc8": {
      "privateKey": "0dc3d33bbffc2d16c175df463110c6d164714a40d23db2f83539664b7365a5b6",
      "peers": [
        "fc1c:84bf:9557:1ce4:4862:fd82:5105:9984",
        "fc1c:90e4:2953:938b:47cc:65c3:6540:dff6",
        "fc1b:7ea7:911f:f2d5:7685:ac22:6c4f:8b15"
      ],
      "admin":
      {
          "bind": "127.0.0.1:11234",
          "password": "the_password_you_will_use_to_connect"
      }
    },
```

And to start it up (in the debugger):

    gdb ./sybilsim -ex 'r < ~/my-cjdns-simulation.json'

BUG: Sometimes the simulator doesn't really start up correctly! If you could figure out
what is going wrong, your help would be most appreciated, if not, you can just quit and
then restart it again and it should start up ok.
