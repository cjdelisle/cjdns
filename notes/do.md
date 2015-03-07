## What happens when you call './do'

**do** is a bash script, and is therefore at least somewhat readable by humans.

The script starts off by initializing some variables:

* The output of **uname** is trimmed and assigned to the environment variable **PLATFORM**
* The output of **uname** is then piped through **sed** to replace 'i' and another character with 'x', such that i686 becomes x86. The returned value is assigned to the environment variable MARCH. Architectures like **x86_64** are unaffected
* The environment variable BUILDDIR is initialized to equal "build_${PLATFORM}
* The environment variable NODE_MIN_VER is initialized to equal "v0.8.15"

## Function Definitions

* **hasOkNode** Checks to see if your version of nodejs (if any) is sufficient for the build tasks. If it does, then it should (as a side effect) initialize the environment variable **NODE** to equal the path to your nodejs executable.
* **getNode** Fetches an up to date version of nodejs.
* **die** prints an error and kills the script.
* **getsha256sum** finds a working sha256sum program. It tries a number of options, and returns 0 if none are successful.
* **main** The main function, which I'll describe below.

## Call main

The main function starts by changing into the directory with a name equivalent to the output of `"$(dirname $0)". If that fails, then the process dies with the argument "failed to set directory".

Next, we create the build directory (if it does not exist). If this fails, the process dies with the error "failed to create build dir ...".

Next, we call getsha256sum. If its output is falsey, then the script dies with the error "couldn't find working sha256 hasher"

Next, we call **hasOkNode**. If this returns 0, then we call **getNode**, if this is unsuccessful, then the script dies with the error "could not get working nodejs impl".

Finally, we use the environment variable **$NODE** to call **cjdns/node_build/make.js** with the arguments "${@}".


