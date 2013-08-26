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


checkfiles.pl
-------------

There is a simple perl script in /scripts/ which will look out for some of the
easy mistakes in code style, to invoke it use:

    find ./ -name '*.c' | perl ./scripts/checkfiles.pl
    find ./ -name '*.h' | perl ./scripts/checkfiles.pl

There should be no output, most importantly, your change should not introduce
any new output.  If there is a line which it complains about and you really
need it that way, add a trailing comment with:

    // CHECKFILES_IGNORE

as is done in Crypto.c


Testing
-------

Any file in a /test/ subdirectory which ends with `_test.c` will be compiled as
a test and added to the testing regime through some cmake hackery. you might
have to cmake .. again before typing make. Tests can fail by returning non-zero
or using assert statements, whatever makes sense.  Tests are exempt from most
code style rules and checkfiles.pl is much easier on them.

*All patches which add tests will be addressed before any patches which don't.*
