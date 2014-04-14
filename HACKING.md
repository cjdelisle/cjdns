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
  git configured to remove carrage return characters as lines in the codebase
  are \n deliniated.
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
  AKA `define SillyMath_VALUE_OF_PI 3` or `#define SillyMath_DIVIDE(a,b) (a /
  b)` it is sometimes acceptable for macros to use camel case as is done in
  Endian.h, use judgement.

If there is a better way, come to irc and announce it, this code style has
evolved to where it is now.

Assert.h
--------

Lining your code with assertions is great! You'll find a few macros in Assert.h
to help you. If an assertion is cheap or in a cold codepath or you otherwise feel
it's important that it's never skipped, use `Assert_true()`, if you want the
asserion to be skipped on small hardware where `-DPARANOIA` is disabled, use
`Assert_ifParanoid()`, if your assertion might be triggered by "bad nodes" in a
realistic network and is for simulation only, use `Assert_ifTesting()` and it
will only be included if `-DTESTING` is passed. If your assertion has
side-effects, you must use `Assert_true()` because these are macros which
are completely removed if they are disabled.


Testing
-------

Any file in a /test/ subdirectory which ends with `_test.c` will be compiled as
a test and added to the testing regime through some cmake hackery. you might
have to cmake .. again before typing make. Tests can fail by returning non-zero
or using assert statements, whatever makes sense.  Tests are exempt from most
code style rules and checkfiles.pl is much easier on them.

*All patches which add tests will be addressed before any patches which don't.*
