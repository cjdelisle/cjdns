# GYP Hacking

Note that the instructions below assume that you have the Chromium
[depot tools](http://dev.chromium.org/developers/how-tos/depottools)
installed and configured.
If you don't, you do not pass go, and you cannot collect your $200.

## Getting the sources

Best is to use git to hack on anything, you can set up a git clone of GYP
as follows:

```
git clone https://chromium.googlesource.com/external/gyp.git
cd gyp
git svn init --prefix=origin/ -T trunk http://gyp.googlecode.com/svn/
git config --replace-all svn-remote.svn.fetch trunk:refs/remotes/origin/master
git svn fetch
```

At this point you should get a quick stream of output as git-svn rebuilds
it's SVN version->SHA1 database:

```
Rebuilding .git/svn/refs/remotes/origin/master/...
r1 = 880a7f243635258fa4938bb7ebf9b4d5f46e3e48
r2 = 24a2a4ceedce0ae16d0a107d9898e4828775c633
r3 = 41276136692399ac4bb84c6664d02384fcad70ad
```

If you get slow trickling output with a bunch of file names as such:

```
r1 = 880a7f243635258fa4938bb7ebf9b4d5f46e3e48 (refs/remotes/git-svn)
        A       xc.py
```

then these instructions are out of date, and you're re-downloading
the entire version history from SVN. Please update these instructions
once you've figured out what's up.

If you intend to commit your changes back to the GYP SVN repository,
you need to configure git-svn's commit url.

```
git config svn-remote.svn.commiturl https://gyp.googlecode.com/svn/trunk
```

## Testing your change

GYP has a suite of tests which you can run with the provided test driver
to make sure your changes aren't breaking anything important.

You run the test driver with e.g.

```
python gyptest.py
python gyptest.py test/win  # Only run Windows-related tests.
python gyptest.py -a -f ninja  # Only run ninja-related tests.
```

See [Testing](Testing) for more details on the test framework.

Note that it can be handy to look at the project files output by the tests
to diagnose problems. The easiest way to do that is by kindly asking the
test driver to leave the temporary directories it creates in-place.
This is done by setting the enviroment variable "PRESERVE", e.g.

```
set PRESERVE=all     # On Windows
export PRESERVE=all  # On saner platforms.
```

## Reviewing your change

All changes to GYP must be code reviewed before submission, GYP uses rietveld.

Upload your change with:

```
git cl upload
```

## Submitting

Once you're ready to submit, you can use the GYP try bots to test your change
with e.g.

```
git try
```

Once the change has been approved (LGTMed) and passes trybots, you can submit
it with:

```
git cl dcommit
```

To be allowed to submit, you will need committer rights in the project, and
your password for the SVN server.  You can get that password here:
https://code.google.com/hosting/settings
